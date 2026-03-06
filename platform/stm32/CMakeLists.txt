project(stm32_bins C CXX ASM)

set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/STM32H743ZITX_FLASH.ld)

file(GLOB_RECURSE CORE_SRC
    ${PROJECT_SOURCE_DIR}/Core/Src/*.c
)

file(GLOB_RECURSE HAL_SRC
    ${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Src/*.c
)

add_executable(stm32_bins
    ${CORE_SRC}
    ${HAL_SRC}
)

target_include_directories(stm32_bins PRIVATE

    ${PROJECT_SOURCE_DIR}/Core/Inc

    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include

    ${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc

    ${PROJECT_SOURCE_DIR}/App
    ${PROJECT_SOURCE_DIR}/GUI
    ${PROJECT_SOURCE_DIR}/lvgl
)

target_link_libraries(stm32_bins
    app
    gui
    lvgl
)

target_compile_definitions(stm32_bins PRIVATE
    STM32H743xx
    USE_HAL_DRIVER
)

target_link_options(stm32_bins PRIVATE
    -T${LINKER_SCRIPT}
)