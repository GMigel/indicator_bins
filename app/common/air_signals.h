#pragma once

#include <math.h>
#include <stdint.h>

float altitude(float pres, float ref);
float air_speed(float pres);
float vert_speed(float pres, float ref, float dp);

