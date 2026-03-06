cmake_minimum_required(VERSION 3.20)

project(indicator_bins)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_PC_SIM "Build PC simulator" ON)
option(BUILD_STM32 "Build STM32 firmware" OFF)

add_subdirectory(lvgl)

if(BUILD_PC_SIM)
    add_subdirectory(platform/pc)
    endif()
    
    if(BUILD_STM32)
        add_subdirectory(platform/stm32)
        endif()