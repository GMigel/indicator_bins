#include "rotary_encoder.h"

RotaryEncoder::RotaryEncoder(listener_t &l)
    : listener(l)
{
}

void RotaryEncoder::change_listener(listener_t &l)
{
  listener = l;

  btn_prev = false;
  hold_fired = false;
  press_time = 0;
  last_activity = 0;
}

void RotaryEncoder::serve_input(int8_t delta, bool pressed, uint32_t now)
{
  /* ---------- ROTATE ---------- */

  if (delta != 0) {
    if (listener.on_rotate)
      listener.on_rotate(delta);

    last_activity = now;
  }

  /* ---------- BUTTON DOWN ---------- */

  if (pressed && !btn_prev) {
    press_time = now;
    hold_fired = false;
  }

  /* ---------- HOLD ---------- */

  if (pressed && !hold_fired) {
    if ((now - press_time) >= HoldTime) {
      if (listener.on_hold)
        listener.on_hold();

      hold_fired = true;
      last_activity = now;
    }
  }

  /* ---------- BUTTON RELEASE ---------- */

  if (!pressed && btn_prev) {
    if (!hold_fired) {
      if (listener.on_click)
        listener.on_click();
    }

    last_activity = now;
  }

  /* ---------- TIMEOUT ---------- */

  if ((now - last_activity) >= Timeout) {
    if (listener.on_timeout)
      listener.on_timeout();

    last_activity = now;
  }

  btn_prev = pressed;
}