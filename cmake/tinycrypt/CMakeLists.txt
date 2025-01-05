#------------------------------------------------------------------------------
# Copyright (c) 2020-2023, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)

# Add the library
add_library(tinycrypt STATIC)

target_include_directories(tinycrypt
        PUBLIC
            ${PROJECT_SOURCE_DIR}/ThirdParty/mcuboot/ext/tinycrypt/lib/include
)

        


# Set the source files
target_sources(tinycrypt
    PRIVATE
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/aes_decrypt.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/aes_encrypt.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/cbc_mode.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ctr_mode.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ctr_prng.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/hmac.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/hmac_prng.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/sha256.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ecc.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ecc_dh.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ecc_dsa.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/ccm_mode.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/cmac_mode.c
    ../../ThirdParty/mcuboot/ext/tinycrypt/lib/source/utils.c
)


