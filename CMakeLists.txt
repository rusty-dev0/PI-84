# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(PI-84 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(PI-84
    main.c
    ST7735_TFT.c
    hw.c
)

pico_set_program_name(PI-84 "PI-84")
pico_set_program_version(PI-84 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(PI-84 0)
pico_enable_stdio_usb(PI-84 1)

# Add the standard library to the build
target_link_libraries(PI-84
    pico_stdlib
    hardware_clocks
    hardware_spi
    hardware_pwm
    hardware_irq
    hardware_sync
)

# Add the standard include files to the build
target_include_directories(PI-84 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Adafruit 1.8" ST7735 wiring on SPI0
target_compile_definitions(PI-84 PRIVATE
    SPI_TFT_PORT=spi0
    SPI_TFT_CS=5
    SPI_TFT_DC=6
    SPI_TFT_RST=0
    TFT_ENABLE_RED
    TFT_ENABLE_RESET
    TFT_ENABLE_SHAPES
    TFT_ENABLE_ROTATE
)

pico_add_extra_outputs(PI-84)