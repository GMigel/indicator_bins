#pragma once

#include "rotary_encoder.h"

extern RotaryEncoder encoder;

extern RotaryEncoder::listener_t enc_brg;
extern RotaryEncoder::listener_t enc_ref;
extern RotaryEncoder::listener_t enc_pres;
extern RotaryEncoder::listener_t enc_menu;