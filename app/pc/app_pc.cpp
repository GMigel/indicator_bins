#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "app.h"
#include "app_cpp.h"
#include "gui.h"
#include "lvgl.h"
#include "lv_port_pc.h"
#include "input_hal.h"
#include "disp_bright.h"

void app_run()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    lv_init();
    lv_port_pc_init();

    gui_init(&gui_state);
    disp_bright_init();
    input_init();
    can_init();

    uint32_t last_tick = SDL_GetTicks();

    while (1)
    {
        /* -------- SDL events -------- */
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return;
        }

        /* -------- LVGL tick -------- */
        uint32_t now = SDL_GetTicks();
        uint32_t diff = now - last_tick;

        if(diff > 0)
        {
            lv_tick_inc(diff);
            last_tick = now;
        }

        /* -------- Application -------- */
        app_on_timer();

        can_serve();
        gui_refresh(&gui_state);

        lv_timer_handler();

        SDL_Delay(1);
    }
}