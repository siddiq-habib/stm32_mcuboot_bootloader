#------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

add_library(bootutil STATIC)

target_include_directories(bootutil
    PUBLIC
        ${PROJECT_SOURCE_DIR}/ThirdParty/mcuboot/boot/bootutil/include
        ${PROJECT_SOURCE_DIR}/Core/Inc
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ThirdParty/mcuboot/ext/tinycrypt/lib/include
        ../../ThirdParty/mcuboot/boot/bootutil/src
)

target_sources(bootutil
    PRIVATE
        ../../ThirdParty/mcuboot/boot/bootutil/src/boot_record.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/bootutil_misc.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/bootutil_public.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/caps.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/encrypted.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/fault_injection_hardening.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/image_ecdsa.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/image_ed25519.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/image_rsa.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/image_validate.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/loader.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/swap_misc.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/swap_move.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/swap_scratch.c
        ../../ThirdParty/mcuboot/boot/bootutil/src/tlv.c
)
if(CONFIG_BOOT_RAM_LOAD)
    target_sources(bootutil
        PRIVATE
            ../../ThirdParty/mcuboot/boot/bootutil/src/ram_load.c
    )
endif()
