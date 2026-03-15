#pragma once

#include <cstdint>
#include <functional>

class RotaryEncoder {
public:
  struct listener_t {
    std::function<void(int8_t)> on_rotate;
    std::function<void()> on_click;
    std::function<void()> on_hold;
    std::function<void()> on_timeout;
  };

  RotaryEncoder(listener_t &l) : listener(l) {}

  void change_listener(listener_t &l) {
    listener = l;

    btn_prev = false;
    hold_fired = false;
    press_time = 0;
    last_activity = 0;
  }

  void serve_input(int8_t delta, bool pressed, uint32_t now) {
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

private:
  listener_t listener;

  bool btn_prev = false;
  bool hold_fired = false;

  uint32_t press_time = 0;
  uint32_t last_activity = 0;

  static constexpr uint32_t HoldTime = 800; // ms
  static constexpr uint32_t Timeout = 5000; // ms
};