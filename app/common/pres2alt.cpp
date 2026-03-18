#include "pres2alt.h"
#include <math.h>

// Constants (internal linkage)
static const float T0 = 288.15f;
static const float L  = 6.5e-3f;
static const float R  = 8.31446f;
static const float G  = 9.80665f;
static const float M  = 28.98e-3f;

float pres2alt(float pressure, float ref)
{
    return T0 / L * (1.0f - powf((pressure / ref), (R * L / (G * M))));
}

float pres2vspeed(float pressure, float ref)
{
    static float prev_pres = 0.0f;
    static const float dt = 0.1f;

    float diff_pres = (pressure - prev_pres) * dt;
    prev_pres = pressure;

    return -T0 * powf(pressure / ref, R * L / (G * M)) *
           R * diff_pres / (G * M * pressure);
}