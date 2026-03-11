file(GLOB_RECURSE LVGL_SRC
    src/*.c
)

set(LV_PORT_FILE)

if(BUILD_PC_SIM)
    #set(LV_PORT_FILE lv_port_pc.c)
#    add_library(lvgl
#    ../platform/pc/lv_port_pc.c
#)
endif()

if(BUILD_STM32)
    set(LV_PORT_FILE lv_port_stm32.c)
endif()

add_library(lvgl STATIC
    ${LVGL_SRC}
    ${LV_PORT_FILE}
)

target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(lvgl PUBLIC
    LV_CONF_INCLUDE_SIMPLE
#    PLATFORM_PC
)

if(BUILD_PC_SIM)
target_compile_definitions(lvgl PUBLIC
#    LV_CONF_INCLUDE_SIMPLE
    PLATFORM_PC
)
endif()
