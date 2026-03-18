#pragma once

#ifdef __cplusplus
extern "C" {
#endif

float pres2alt(float pressure, float ref);
float pres2vspeed(float pressure, float ref);

#ifdef __cplusplus
} // extern "C"
#endif