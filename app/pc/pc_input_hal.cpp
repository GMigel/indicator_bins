#include <SDL2/SDL.h>
#include "input_hal.h"

static int8_t enc_delta = 0;
static bool   btn_state = false;

void input_init(void)
{
    enc_delta = 0;
    btn_state = false;
}

void input_poll(void)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            exit(0);
        }

        if (e.type == SDL_MOUSEWHEEL)
        {
            if (e.wheel.y > 0)
                enc_delta += 1;
            else if (e.wheel.y < 0)
                enc_delta -= 1;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_MIDDLE)
                btn_state = true;
        }

        if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_MIDDLE)
                btn_state = false;
        }
    }
}

int8_t input_get_enc(void)
{
    int8_t d = enc_delta;
    enc_delta = 0;      // 🔥 ВАЖНО: возвращаем дельту и обнуляем
    return d;
}

bool input_get_btn(void)
{
    return btn_state;
}