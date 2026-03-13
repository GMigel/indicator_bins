#pragma once

#include "RotaryEncoder.h"

// Initialize the encoder simulation with a RotaryEncoder instance
void encoder_sim_init(RotaryEncoder* enc);

// Poll SDL events and update the encoder simulation
void encoder_sim_poll();
