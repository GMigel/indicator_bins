cmake_minimum_required(VERSION 3.20)

project(indicator_bins)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_PC_SIM "Build PC simulator" ON)
option(BUILD_STM32 "Build STM32 firmware" OFF)

# Tell LVGL where lv_conf.h is
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
include_directories(${CMAKE_SOURCE_DIR}/lvgl)

add_subdirectory(lvgl)
add_subdirectory(app)
add_subdirectory(gui)

if(BUILD_PC_SIM)
    add_subdirectory(app/pc)
endif()

if(BUILD_STM32)
    add_subdirectory(app/stm32)
endif()