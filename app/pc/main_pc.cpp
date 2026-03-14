#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdio>
#include "lvgl.h"
#include "lv_port_pc.h"
#include "gui.h"
#include "encoder_sim.h"
#include "app_cpp.h"

extern gui_state_t gui_state;

int main()
{
    #ifdef LV_MEM_SIZE
        printf("LV_MEM_SIZE=%d\n", LV_MEM_SIZE);
    #else
        printf("LVGL using system malloc\n");
    #endif

    #ifdef PLATFORM_PC
        printf("Running on PC platform\n");
    #else
        printf("Running on embedded platform STM32\n");
    #endif

    lv_init();

    /* Initialize SDL + LVGL display driver */
    lv_port_pc_init();

    gui_init(&gui_state);

    while (1)
  {
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            exit(0);

        encoder_sim_handle_event(e);
    }

    encoder.serve_input(
        encoder_sim_get_enc(),
        encoder_sim_get_btn()
    );

    gui_refresh(&gui_state);
    lv_timer_handler();
    lv_tick_inc(5);

    SDL_Delay(5);
  }
    return 0;
}