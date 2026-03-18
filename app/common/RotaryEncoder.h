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

  explicit RotaryEncoder(listener_t &l);

  void change_listener(listener_t &l);
  void serve_input(int8_t delta, bool pressed, uint32_t now);

private:
  listener_t listener;

  bool btn_prev = false;
  bool hold_fired = false;

  uint32_t press_time = 0;
  uint32_t last_activity = 0;

  static constexpr uint32_t HoldTime = 800;  // ms
  static constexpr uint32_t Timeout  = 5000; // ms
};