#include "app.h"

int main()
{
    app_run();
    return 0;
}

#if 0
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "app.h"
#include "app_cpp.h"
#include "gui.h"
#include "lvgl.h"
#include "lv_port_pc.h"
#include "input_hal.h"

extern gui_state_t gui_state;

void app_run()
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

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    lv_init();
    /* Initialize SDL + LVGL display driver */
    lv_port_pc_init();

    gui_init(&gui_state);
    disp_bright_init();
    input_init();
    can_init();

    uint32_t last_tick = SDL_GetTicks();

    while (1)
    {
        /* ---- timing ---- */
        uint32_t now = SDL_GetTicks();
        uint32_t diff = now - last_tick;

        if (diff >= 1)
        {
            last_tick = now;
            app_on_timer();
        }

        /* ---- main loop ---- */
        can_serve();
        gui_refresh(&gui_state);
        lv_timer_handler();

        SDL_Delay(1);
    }

//     while (1)
//   {
//     SDL_Event e;

//     while (SDL_PollEvent(&e))
//     {
//         if (e.type == SDL_QUIT)
//             exit(0);

//         encoder_sim_handle_event(e);
//     }

//     encoder.serve_input(
//         encoder_sim_get_enc(),
//         encoder_sim_get_btn()
//     );

//     gui_refresh(&gui_state);
//     lv_timer_handler();
//     lv_tick_inc(5);

//     SDL_Delay(5);
//   }
    return 0;
}

#endif