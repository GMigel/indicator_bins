#pragma once

#include "rotary_encoder.h"
#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

void encoder_sim_handle_event(const SDL_Event& e);
int8_t encoder_sim_get_enc();
bool encoder_sim_get_btn();

// Initialize the encoder simulation with a RotaryEncoder instance
void encoder_sim_init(RotaryEncoder* enc);

// Poll SDL events and update the encoder simulation
void encoder_sim_poll();

