project(pc_sim)

find_package(SDL2 REQUIRED)

# --- GUI ---
file(GLOB_RECURSE GUI_SRC
    ${CMAKE_SOURCE_DIR}/gui/*.c
)

add_library(gui STATIC
    ${GUI_SRC}
    ${CMAKE_SOURCE_DIR}/app/pc/lv_port_pc.c
)

target_include_directories(gui PUBLIC
    ${CMAKE_SOURCE_DIR}/gui
    ${CMAKE_SOURCE_DIR}/app/pc
    ${CMAKE_SOURCE_DIR}/app/common
)

target_link_libraries(gui
    lvgl
)

set(APP_PC_SRC
    app_pc.cpp
    app_pc_stubs.cpp
    encoder_sim.cpp
    input_sim.cpp
    pc_input_hal.cpp
    timer_sim.cpp
    disp_bright.c
)

add_executable(pc_sim
    ${APP_PC_SRC}
)

target_include_directories(pc_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/app/pc
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(pc_sim
    app
    gui
    lvgl
    SDL2
)

# Platform define
target_compile_definitions(app PRIVATE PLATFORM_PC)
target_compile_definitions(gui PRIVATE PLATFORM_PC)
target_compile_definitions(pc_sim PRIVATE PLATFORM_PC)