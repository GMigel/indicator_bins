#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "lvgl.h"
#include "gui.h"

int main()
{
    lv_init();

    gui_state_t state;
    gui_init(&state);

    while(1)
    {
        lv_timer_handler();
        SDL_Delay(5);
    }
}