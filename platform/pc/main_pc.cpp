#include <SDL.h>
#include "gui.h"

int main()
{
    gui_state_t state;

    gui_init(&state);

    while(1)
    {
        lv_timer_handler();
        SDL_Delay(5);
    }
}