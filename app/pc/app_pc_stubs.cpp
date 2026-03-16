// #ifdef PLATFORM_PC

#define HAL_TICK_FREQ_DEFAULT 5

#include <cmath>
#include <cstdint>
#include <cstdio>

#include "app.h"
#include "gui.h"

extern "C" {

// Fake systime from PC main loop
extern volatile uint32_t systime;

// ---------------- CAN simulation ----------------

void can_init() { printf("[PC] CAN init (simulated)\n"); }

void rs422_serve() {}

/*
 * Generate smooth aircraft motion for GUI simulation
 */
void can_serve() {
  float t = systime * 0.001f;

  gui_state.valid_roll = true;
  gui_state.roll = sinf(t * 0.7f) * 0.6f;

  gui_state.valid_pitch = true;
  gui_state.pitch = sinf(t * 0.5f) * 0.3f;

  gui_state.valid_heading = true;
  gui_state.heading = fmodf(t * 0.3f, 6.2831853f);

  gui_state.valid_altitude = true;
  gui_state.altitude = 1200.0f + sinf(t * 0.1f) * 200.0f;

  gui_state.valid_speed = true;
  gui_state.speed = 220.0f + sinf(t * 0.4f) * 30.0f;

  gui_state.valid_ver_speed = true;
  gui_state.vert_speed = sinf(t * 0.6f) * 8.0f;
}

bool can_send_dat(uint16_t id, const void *data, const uint8_t len)
// void can_send_dat(int id, const void* data, size_t len)
{
  (void)data;

#ifndef PLATFORM_PC
  printf("[PC] CAN TX id=%d len=%zu\n", id, len);
#else
  printf("[PC] CAN TX id=%d len=%u\n", id, len);
#endif

  return false;
}

// ---------------- Display brightness ----------------

static uint8_t bright = 128;

void disp_bright_init() { printf("[PC] Display brightness init\n"); }

void disp_bright_set(uint8_t val) { bright = val; }

uint8_t disp_bright_get() { return bright; }

// ---------------- Encoder / button simulation ----------------

static int8_t enc_val = 0;
static bool btn = false;

void input_init() { printf("[PC] Input init\n"); }

void input_poll() {
  /*
   * Automatic encoder demo
   * Slowly rotates encoder to demonstrate menus
   */

  static uint32_t last = 0;

  if (systime - last > 200) {
    enc_val++;
    last = systime;
  }

  if (enc_val > 10)
    enc_val = -10;
}

int8_t input_get_enc() {
  int8_t v = enc_val;
  enc_val = 0;
  return v;
}

bool input_get_btn() { return btn; }

} // extern "C"

// #endif