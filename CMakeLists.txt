cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME BootLoader)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Set the output directory for the list files
set(LIST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/listings)

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${LIST_OUTPUT_DIR})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

add_subdirectory(cmake/mcuboot)
add_subdirectory(cmake/mbedTLS)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)


# List of source files
set(SOURCE_FILES
    # Add other source files here
    Core/Src/mcuboot_port.c
    Core/Src/log.c
    Core/Src/pubkey.c
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCE_FILES})

# Add compiler flags to generate assembly listings for each source file
foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(REL_PATH ${SOURCE_FILE} DIRECTORY)
    get_filename_component(FILE_NAME ${SOURCE_FILE} NAME_WE)
    set(LIST_FILE ${LIST_OUTPUT_DIR}/${REL_PATH}/${FILE_NAME}.list)
    file(MAKE_DIRECTORY ${LIST_OUTPUT_DIR}/${REL_PATH})
    set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-Wa,-adhln=${LIST_FILE}")
endforeach()

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths    
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Core/Inc
    ${PROJECT_SOURCE_DIR}/MBEDTLS/App
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    bootutil
    mbedtls
    # Add user defined libraries
)


# Add compile definitions based on build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DEBUG)
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Add custom commands or targets if needed
# For example, to generate a binary file from the ELF file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating binary file from ELF file"
)