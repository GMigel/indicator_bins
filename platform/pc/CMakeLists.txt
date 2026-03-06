project(pc_sim)

find_package(SDL2 REQUIRED)

# --- App ---
add_library(app STATIC
    ${CMAKE_SOURCE_DIR}/App/app.cpp
)

target_include_directories(app PUBLIC
    ${CMAKE_SOURCE_DIR}/App
)

# --- GUI ---
file(GLOB_RECURSE GUI_SRC
    ${CMAKE_SOURCE_DIR}/GUI/*.c
)

add_library(gui STATIC ${GUI_SRC})

target_include_directories(gui PUBLIC
    ${CMAKE_SOURCE_DIR}/GUI
)

target_link_libraries(gui
    lvgl
)

# --- PC executable ---
add_executable(pc_sim
    main_pc.cpp
)

target_include_directories(pc_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/platform/pc
)

target_link_libraries(pc_sim
    app
    gui
    lvgl
    SDL2::SDL2
)