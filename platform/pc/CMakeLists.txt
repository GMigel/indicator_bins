project(pc_sim)

find_package(SDL2 REQUIRED)

add_compile_definitions(PLATFORM_PC)

# --- App ---
add_library(app STATIC
    ${CMAKE_SOURCE_DIR}/App/app.cpp
)

target_include_directories(app PUBLIC
    ${CMAKE_SOURCE_DIR}/App
    ${CMAKE_SOURCE_DIR}/GUI
    ${CMAKE_SOURCE_DIR}/lvgl
    ${CMAKE_SOURCE_DIR}/lvgl/src
)
# --- GUI ---
file(GLOB_RECURSE GUI_SRC
    ${CMAKE_SOURCE_DIR}/GUI/*.c
)

#add_library(gui STATIC ${GUI_SRC})
add_library(gui STATIC
    ${GUI_SRC}
    ${CMAKE_SOURCE_DIR}/platform/pc/lv_port_pc.c
)

target_include_directories(gui PUBLIC
    ${CMAKE_SOURCE_DIR}/GUI
    ${CMAKE_SOURCE_DIR}/platform/pc
)

target_link_libraries(gui
    lvgl
)

# --- PC executable ---
add_executable(pc_sim
    main_pc.cpp
)

set_target_properties(pc_sim PROPERTIES
    WIN32_EXECUTABLE OFF
)

target_include_directories(pc_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/platform/pc
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(pc_sim
    app
    gui
    lvgl
#    ${SDL2_LIBRARIES}
    SDL2
)