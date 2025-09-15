cmake_minimum_required(VERSION 3.22)

# Set project name and language
project(Lab2 LANGUAGES C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
enable_language(C ASM)

# Define debug build type by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

# Include toolchain file for STM32 cross compilation
# Adjust the path to your arm-none-eabi toolchain if needed
include("/home/administrator/st/stm32cubeclt_1.19.0/CMake/gcc-arm-none-eabi.cmake")

# STM32CubeMX include directories
set(MX_Include_Dirs
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

# STM32CubeMX defines
set(MX_Defines
    USE_HAL_DRIVER
    STM32F303xC
    $<$<CONFIG:Debug>:DEBUG>
)

# STM32CubeMX application sources
set(MX_Application_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/main.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f3xx_it.c
    ${CMAKE_SOURCE_DIR}/Core/Src/stm32f3xx_hal_msp.c
    ${CMAKE_SOURCE_DIR}/Core/Src/sysmem.c
    ${CMAKE_SOURCE_DIR}/Core/Src/syscalls.c
    ${CMAKE_SOURCE_DIR}/startup_stm32f303xc.s
)

# STM32 HAL drivers sources
set(STM32_Drivers_Src
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32f3xx.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c
)

# Create STM32CubeMX interface library
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines})

# Create static library for HAL drivers
add_library(STM32_Drivers OBJECT ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)

# Create the main executable
add_executable(${PROJECT_NAME} ${MX_Application_Src})
target_link_libraries(${PROJECT_NAME} PRIVATE STM32_Drivers)

# Optional: add extra clean file for map
set_target_properties(${PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${PROJECT_NAME}.map)
