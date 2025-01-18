/* Run the boot image. */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "sysflash/sysflash.h"
#include "flash_map_backend/flash_map_backend.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "mcuboot_config/mcuboot_assert.h"

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(arr[0])

MCUBOOT_LOG_MODULE_DECLARE(mcuboot);

// STM32F401RE Flash Memory Sectors
// Sector 0: 16 KB (0x0800 0000 - 0x0800 3FFF)
// Sector 1: 16 KB (0x0800 4000 - 0x0800 7FFF)
// Sector 2: 16 KB (0x0800 8000 - 0x0800 BFFF)
// Sector 3: 16 KB (0x0800 C000 - 0x0800 FFFF)
// Sector 4: 64 KB (0x0801 0000 - 0x0801 FFFF)
// Sector 5: 128 KB (0x0802 0000 - 0x0803 FFFF)
// Sector 6: 128 KB (0x0804 0000 - 0x0805 FFFF)
// Sector 7: 128 KB (0x0806 0000 - 0x0807 FFFF)

// Memory Allocation
// | Memory Area         | Size    | Start Address | End Address   | Sectors Covered                          |
// |---------------------|---------|---------------|---------------|------------------------------------------|
// | Bootloader          | 128 KB  | 0x0800 0000   | 0x0800 BFFF   | Sector 0, Sector 1, Sector 2 (16 KB each)|
// |                                                               | Sector 3 (16 KB), Sector 4 (64 KB)       |
// | PRIMARY_SLOT        | 128 KB  | 0x0802 0000   | 0x0803 9FFF   | Sector 5 (128 KB)                        |
// | SECONDARY_SLOT      | 128 KB  | 0x0804 0000   | 0x0807 9FFF   | Sector 6 (128 KB),                       |


#define BOOTLOADER_SIZE (128 * 1024)
#define APPLICATION_SIZE (128 * 1024)
#define BOARD_FLASH_SIZE (512 * 1024)

#define BOOTLOADER_START_ADDRESS 0x08000000UL
#define APPLICATION_PRIMARY_START_ADDRESS (BOOTLOADER_START_ADDRESS + BOOTLOADER_SIZE)
#define APPLICATION_SECONDARY_START_ADDRESS (APPLICATION_PRIMARY_START_ADDRESS + APPLICATION_SIZE)


static const struct flash_area bootloader = {
  .fa_id = FLASH_AREA_BOOTLOADER,
  .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
  .fa_off = BOOTLOADER_START_ADDRESS,
  .fa_size = BOOTLOADER_SIZE,
};

static const struct flash_area primary_img0 = {
  .fa_id = FLASH_AREA_IMAGE_PRIMARY(0),
  .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
  .fa_off = APPLICATION_PRIMARY_START_ADDRESS,
  .fa_size = APPLICATION_SIZE,
};

static const struct flash_area secondary_img0 = {
  .fa_id = FLASH_AREA_IMAGE_SECONDARY(0),
  .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
  .fa_off = APPLICATION_SECONDARY_START_ADDRESS,
  .fa_size = APPLICATION_SIZE,
};

static const struct flash_area *s_flash_areas[] = {
  &bootloader,
  &primary_img0,
  &secondary_img0,
};

/*!< flash parameters for internal use */
static const uint32_t sector_size[] = {
  // Bootloader (48 KB)
  16 * 1024, // Sector 0
  16 * 1024, // Sector 1
  16 * 1024, // Sector 2
  16 * 1024, // Sector 3
  64 * 1024, // Sector 4
  // PRIMARY_SLOT (128 KB)
  128 * 1024, // Sector 5
  // SECONDARY_SLOT (128 KB)
  128 * 1024,  // Part of Sector 6
  // spare
  128 * 1024 // Sector 7
};


static const uint32_t tf_sector_count = ARRAY_SIZE(sector_size);
static uint8_t erased_sectors[ARRAY_SIZE(sector_size)] = { 0 };


static bool is_blank(uint32_t addr, uint32_t size)
{
    for (uint32_t i = 0; i < size; i += sizeof(uint32_t)) {
        if (*(uint32_t *)(addr + i) != 0xffffffff) {
            return false;
        }
    }
    return true;
}


static bool flash_erase(uint32_t addr)
{
    // starting address from 0x08000000
    uint32_t sector_addr = BOOTLOADER_START_ADDRESS;
    bool erased = false;

    uint32_t sector = 0;
    uint32_t size = 0;

    for (uint32_t i = 0; i < tf_sector_count; i++) {
        if (sector_addr < BOOTLOADER_START_ADDRESS + BOARD_FLASH_SIZE) {
            size = sector_size[i];
            if (sector_addr + size > addr) {
                sector = i;
                erased = erased_sectors[i];
                erased_sectors[i] = 1; // don't erase anymore - we will continue writing here!
                break;
            }
            sector_addr += size;
        }
    }

    if (!erased && !is_blank(sector_addr, size)) {
        MCUBOOT_LOG_DBG("Erase: %08lX  Sector =  %d size = %lu KB ... \r\n", sector_addr, sector, size / 1024);
        FLASH_Erase_Sector(sector, FLASH_VOLTAGE_RANGE_3);
        FLASH_WaitForLastOperation(HAL_MAX_DELAY);
        MCUBOOT_LOG_DBG("OK\r\n");
    }

    return true;
}


static void flash_write(uint32_t dst, const uint8_t *src, int len)
{
    flash_erase(dst);

    MCUBOOT_LOG_DBG("Write flash at address %08lX\r\n", dst);
    for (int i = 0; i < len; i += 4) {
        uint32_t data = *((uint32_t *)((void *)(src + i)));

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, dst + i, (uint64_t)data) != HAL_OK) {
            MCUBOOT_LOG_ERR("Failed to write flash at address %08lX\r\n", dst + i);
            break;
        }

        if (FLASH_WaitForLastOperation(HAL_MAX_DELAY) != HAL_OK) {
            MCUBOOT_LOG_ERR("Waiting on last operation failed\r\n");
            return;
        }
    }

  }

static void mcuboot_flash_write(uint32_t start_add, void *buffer, uint32_t size)
{
    HAL_FLASH_Unlock();    
    flash_write(start_add, buffer, size);
    HAL_FLASH_Lock();
}

void mcuboot_flash_erase(uint32_t start_add, uint32_t len)
{
    (void)len;
    if(HAL_FLASH_Unlock() == HAL_OK)
    {
      MCUBOOT_LOG_DBG("Flash unlocked\n");
    }
    flash_erase(start_add);
    HAL_FLASH_Lock();
    
}

/**
 * @brief Get the flash Area Id for a given image index and slot.
 *
 * This function determines the flash Area Id based on the image index and slot.
 * The image index indicates how many images are present, as it is possible to 
 * have two images that need to be upgraded. The slot indicates whether we are 
 * referring to the primary or secondary slot of a particular image. The primary 
 * slot is where the image is typically run from, and the secondary slot is used 
 * for the upgrade image.
 *
 * @param image_index The index of the image.
 * @param slot The slot of the image (primary or secondary).
 * @return The defined flash Area Id, which can be used to get more information 
 *         about the particular flash Area.
 *
 * @note If you are using the MCUBOOT_SWAP_USING_SCRATCH=1 configuration, the 
 * FLASH_AREA_IMAGE_SCRATCH slot identifier may also be passed in the “slot” 
 * argument and will need to be handled.
 */
int flash_area_id_from_multi_image_slot(int image_index, int slot) {
    switch (slot) {
      case 0:
        return FLASH_AREA_IMAGE_PRIMARY(image_index);
      case 1:
        return FLASH_AREA_IMAGE_SECONDARY(image_index);
    }

    MCUBOOT_LOG_ERR("Unexpected Request: image_index=%d, slot=%d", image_index, slot);
    return -1; /* flash_area_open will fail on that */
}

int flash_area_id_from_image_slot(int slot) {
  return flash_area_id_from_multi_image_slot(0, slot);
}

static const struct flash_area *prv_lookup_flash_area(uint8_t id) {
  for (size_t i = 0; i < ARRAY_SIZE(s_flash_areas); i++) {
    const struct flash_area *area = s_flash_areas[i];
    if (id == area->fa_id) {
      return area;
    }
  }
  return NULL;
}

int flash_area_open(uint8_t id, const struct flash_area **area_outp) {
  MCUBOOT_LOG_DBG("%s: ID=%d", __func__, (int)id);

  const struct flash_area *area = prv_lookup_flash_area(id);
  *area_outp = area;
  return area != NULL ? 0 : -1;
}

void flash_area_close(const struct flash_area *fa) {
  // no cleanup to do for this flash part
}

int flash_area_read(const struct flash_area *fa, uint32_t off, void *dst,
                    uint32_t len) {
  if (fa->fa_device_id != FLASH_DEVICE_INTERNAL_FLASH) {
    return -1;
  }

  const uint32_t end_offset = off + len;
  if (end_offset > fa->fa_size) {
    MCUBOOT_LOG_ERR("%s: Out of Bounds (0x%x vs 0x%x)", __func__, end_offset, fa->fa_size);
    return -1;
  }

  // internal flash is memory mapped so just dereference the address
  void *addr = (void *)(fa->fa_off + off);
  memcpy(dst, addr, len);

  return 0;
}



int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src,
                     uint32_t len) {
  if (fa->fa_device_id != FLASH_DEVICE_INTERNAL_FLASH) {
    return -1;
  }

  const uint32_t end_offset = off + len;
  if (end_offset > fa->fa_size) {
    MCUBOOT_LOG_ERR("%s: Out of Bounds (0x%x vs 0x%x)", __func__, end_offset, fa->fa_size);
    return -1;
  }

  const uint32_t addr = fa->fa_off + off;
  MCUBOOT_LOG_DBG("%s: Addr: 0x%08x Length: %d", __func__, (int)addr, (int)len);
  mcuboot_flash_write(addr, src, len);

#if VALIDATE_PROGRAM_OP
  if (memcmp((void *)addr, src, len) != 0) {
    MCUBOOT_LOG_ERR("%s: Program Failed", __func__);
    assert(0);
  }
#endif

  return 0;
}

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len)
{
    if (fa->fa_device_id != FLASH_DEVICE_INTERNAL_FLASH) {
        return -1;
    }

    const uint32_t start_addr = fa->fa_off + off;

    MCUBOOT_LOG_DBG("%s: Addr: 0x%08x Length: %d", __func__, (int)start_addr, (int)len);

    mcuboot_flash_erase(start_addr, len);

    return 0;
}

size_t flash_area_align(const struct flash_area *area) {
  return 4;
}

uint8_t flash_area_erased_val(const struct flash_area *area) {
  // the value a byte reads when erased on storage.
  return 0xff;
}

int flash_area_to_sectors(int idx, int *cnt, struct flash_area *fa)
{
    return -1;
    
}
/**
 * @brief Retrieves the sectors of a specified flash area.
 * @param fa_id The ID of the flash area to retrieve sectors for.
 * @param count Pointer to a variable where the number of sectors will be stored.
 * @param sectors Pointer to an array where the sector information will be stored.
 * @return 0 on success, -1 if the flash area is not internal flash.
 */
int flash_area_get_sectors(int fa_id, uint32_t *count,
                           struct flash_sector *sectors) {
  
  const struct flash_area *fa = prv_lookup_flash_area(fa_id);
  if (fa->fa_device_id != FLASH_DEVICE_INTERNAL_FLASH) {
    return -1;
  }

  // Sectors for the stm32f401re are not of the same size
  // so we need to calculate the sectors based on the size of the flash area

  uint32_t fa_sector_index = 0; // Index for sectors within the flash area
  uint32_t tf_sector_offset = 0; // Offset to keep track of the current sector position
  size_t tf_sector_index = 0; // Index for the total flash sectors

  // Loop through the total flash sectors
  while(tf_sector_index < tf_sector_count) {        
    // Check if the current sector offset matches the flash area offset
    if(fa->fa_off == (BOOTLOADER_START_ADDRESS + tf_sector_offset)) {
      // Loop through the flash area size and assign sectors
      size_t off = 0;
      while( off < fa->fa_size) {
        // Note: Offset here is relative to flash area, not device
        sectors[fa_sector_index].fs_off = fa->fa_off + off; // Set the sector offset
        sectors[fa_sector_index].fs_size = sector_size[tf_sector_index]; // Set the sector size
        
        off += sector_size[tf_sector_index];
        fa_sector_index++; // Increment the flash area sector index
        tf_sector_index++; // Increment the total flash sector index
        if(tf_sector_index > tf_sector_count){
          return -1;
        }
      }
      break; // Exit the loop once the sectors are assigned
    }
    else {      
      tf_sector_offset += sector_size[tf_sector_index]; // Update the sector offset
      tf_sector_index++; // Increment the total flash sector index
    }
  }

  *count = fa_sector_index; // Set the count to the number of sectors in the flash area
  return 0;

}


void mcuboot_assert_handler(const char *file, int line) {
  MCUBOOT_LOG_ERR("ASSERT: File: %s Line: %d", file, line);
  Error_Handler();
}