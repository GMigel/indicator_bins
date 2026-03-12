#file(GLOB APP_SRC
#    *.cpp
#    *.c
#)

add_library(app STATIC)

target_sources(app PRIVATE
    app_common.cpp
)

if(BUILD_PC_SIM)
    target_sources(app PRIVATE app_pc.cpp)
endif()

if(BUILD_STM32)
    target_sources(app PRIVATE app_stm32.cpp)
endif()

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)