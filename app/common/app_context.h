#pragma once

#include <stdint.h>
#include "gui.h"
#include "RotaryEncoder.h"

struct AppContext
{
    gui_state_t gui;
    RotaryEncoder encoder;
    volatile uint32_t systime;

    gui_menu_t* menu_main;
    gui_menu_t* menu_pres;
    gui_menu_t* menu_alt;
    gui_menu_t* menu_ver;

    AppContext(RotaryEncoder::listener_t listener)
        : encoder(listener), systime(0),
          menu_main(nullptr),
          menu_pres(nullptr),
          menu_alt(nullptr),
          menu_ver(nullptr)
    {}
};