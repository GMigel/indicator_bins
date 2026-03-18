add_library(app STATIC
    common/app_common.cpp
    common/pres2alt.cpp
    common/RotaryEncoder.cpp
)

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app PUBLIC gui)

if(BUILD_PC_SIM)
    target_sources(app PRIVATE
        pc/app_pc.cpp
        pc/app_pc_stubs.cpp
    )
endif()

if(BUILD_STM32)
    target_sources(app PRIVATE
        stm32/app_stm32.cpp
    )
endif()