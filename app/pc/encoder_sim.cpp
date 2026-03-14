#include <SDL2/SDL.h>
#include "encoder_sim.h"

static int32_t enc_value = 0;
static bool button_pressed = false;

int8_t encoder_sim_get_enc()
{
    return enc_value;
}

bool encoder_sim_get_btn()
{
    return button_pressed;
}

void encoder_sim_handle_event(const SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
{
    if (e.key.keysym.sym == SDLK_RETURN)
        button_pressed = true;
}

if (e.type == SDL_KEYUP)
{
    if (e.key.keysym.sym == SDLK_RETURN)
        button_pressed = false;
}

    if (e.type == SDL_MOUSEBUTTONDOWN)
{
    printf("button %d\n", e.button.button);
}

if (e.type == SDL_MOUSEWHEEL)
{
    printf("wheel %d\n", e.wheel.y);
}

    if (e.type == SDL_MOUSEWHEEL)
    {
        if (e.wheel.y > 0)
            enc_value++;
        else if (e.wheel.y < 0)
            enc_value--;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE)
            button_pressed = true;
    }

    if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (e.button.button == SDL_BUTTON_MIDDLE)
            button_pressed = false;
    }
}