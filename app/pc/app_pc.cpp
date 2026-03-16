#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "app.h"
#include "app_cpp.h"
#include "disp_bright.h"
#include "gui.h"
#include "input_hal.h"
#include "lv_port_pc.h"
#include "lvgl.h"

static int enc_pos = 0;
static bool btn = false;

// Helper function: map SDL mouse wheel & button to encoder input
static void handle_sdl_event(SDL_Event &e) {
  switch (e.type) {
  case SDL_MOUSEWHEEL:
    enc_pos -= e.wheel.y;
    break;

  case SDL_MOUSEBUTTONDOWN:
    if (e.button.button == SDL_BUTTON_MIDDLE || e.button.button == SDL_BUTTON_RIGHT)
      btn = true;
    break;

  case SDL_MOUSEBUTTONUP:
    if (e.button.button == SDL_BUTTON_MIDDLE || e.button.button == SDL_BUTTON_RIGHT)
      btn = false;
    break;

  case SDL_KEYDOWN:
    if (e.key.keysym.sym == SDLK_UP)
      enc_pos++;
    if (e.key.keysym.sym == SDLK_DOWN)
      enc_pos--;
    if (e.key.keysym.sym == SDLK_RETURN)
      btn = true;
    break;

  case SDL_KEYUP:
    if (e.key.keysym.sym == SDLK_RETURN)
      btn = false;
    break;
  }
}

void app_run() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  lv_init();
  lv_port_pc_init();

  gui_init(&gui_state);
  disp_bright_init();
  input_init();
  can_init();

  uint32_t last_tick = SDL_GetTicks();

  while (1) {
    /* -------- SDL events -------- */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        return;

      handle_sdl_event(e);
    }
    // encoder.serve_input(enc_pos, btn);
    // static int prev = 0;
    // int delta = enc_pos - prev;
    // prev = enc_pos;
    // encoder.serve_input(delta, btn);
    static int prev = 0;
    int delta = enc_pos - prev;
    prev = enc_pos;
    encoder.serve_input(delta, btn, SDL_GetTicks());

    /* -------- LVGL tick -------- */
    uint32_t now = SDL_GetTicks();
    uint32_t diff = now - last_tick;
    if (diff > 0) {
      lv_tick_inc(diff);
      last_tick = now;
    }

    /* -------- Application -------- */
    app_on_timer();

    can_serve();
    gui_refresh(&gui_state);

    lv_timer_handler();

    SDL_Delay(1);
  }
}