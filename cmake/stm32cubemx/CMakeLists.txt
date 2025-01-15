cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	MBEDTLS_CONFIG_FILE=<mbedtls_config.h> 
	USE_HAL_DRIVER 
	STM32F401xE
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../MBEDTLS/App
    ../../Core/Inc
    ../../Drivers/STM32F4xx_HAL_Driver/Inc
    ../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ../../Middlewares/Third_Party/mbedTLS/include/mbedtls
    ../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../../Middlewares/Third_Party/mbedTLS/include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/stm32f4xx_it.c
    ../../Core/Src/stm32f4xx_hal_msp.c
    ../../MBEDTLS/App/mbedtls.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
    ../../Core/Src/system_stm32f4xx.c
    ../../Middlewares/Third_Party/mbedTLS/library/aes.c
    ../../Middlewares/Third_Party/mbedTLS/library/asn1parse.c
    ../../Middlewares/Third_Party/mbedTLS/library/asn1write.c
    ../../Middlewares/Third_Party/mbedTLS/library/base64.c
    ../../Middlewares/Third_Party/mbedTLS/library/bignum.c
    ../../Middlewares/Third_Party/mbedTLS/library/camellia.c
    ../../Middlewares/Third_Party/mbedTLS/library/certs.c
    ../../Middlewares/Third_Party/mbedTLS/library/chacha20.c
    ../../Middlewares/Third_Party/mbedTLS/library/chachapoly.c
    ../../Middlewares/Third_Party/mbedTLS/library/cipher.c
    ../../Middlewares/Third_Party/mbedTLS/library/cipher_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/ctr_drbg.c
    ../../Middlewares/Third_Party/mbedTLS/library/dhm.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecdh.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecdsa.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecp.c
    ../../Middlewares/Third_Party/mbedTLS/library/ecp_curves.c
    ../../Middlewares/Third_Party/mbedTLS/library/entropy.c
    ../../Middlewares/Third_Party/mbedTLS/library/entropy_poll.c
    ../../Middlewares/Third_Party/mbedTLS/library/gcm.c
    ../../Middlewares/Third_Party/mbedTLS/library/hkdf.c
    ../../Middlewares/Third_Party/mbedTLS/library/md.c
    ../../Middlewares/Third_Party/mbedTLS/library/md_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/memory_buffer_alloc.c
    ../../Middlewares/Third_Party/mbedTLS/library/oid.c
    ../../Middlewares/Third_Party/mbedTLS/library/pem.c
    ../../Middlewares/Third_Party/mbedTLS/library/pk.c
    ../../Middlewares/Third_Party/mbedTLS/library/pkparse.c
    ../../Middlewares/Third_Party/mbedTLS/library/pk_wrap.c
    ../../Middlewares/Third_Party/mbedTLS/library/platform.c
    ../../Middlewares/Third_Party/mbedTLS/library/platform_util.c
    ../../Middlewares/Third_Party/mbedTLS/library/poly1305.c
    ../../Middlewares/Third_Party/mbedTLS/library/rsa_internal.c
    ../../Middlewares/Third_Party/mbedTLS/library/sha256.c
    ../../Middlewares/Third_Party/mbedTLS/library/sha512.c
    ../../Middlewares/Third_Party/mbedTLS/library/ssl_ciphersuites.c
    ../../Middlewares/Third_Party/mbedTLS/library/version_features.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509.c
    ../../Middlewares/Third_Party/mbedTLS/library/x509_crt.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32f401xe.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


