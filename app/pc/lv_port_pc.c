/*
 * lv_port.c
 *
 *  Created on: Jul 20, 2023
 *      Author: Иван
 */

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "lvgl.h"
#include "lv_port_pc.h"
#include "display_config.h"

static lv_color_t display_buf[2][DISP_HOR_RES * DISP_VER_RES] /* __attribute__ ((section(".video_ram")))*/;
volatile void *buf_addr = display_buf[0];
lv_disp_drv_t disp_drv;
// void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

void lv_port_pc_init(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "LVGL simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISP_HOR_RES,
        DISP_VER_RES,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      DISP_HOR_RES,
      DISP_VER_RES
    );

    lv_port_disp_init();
}

//------------------------------------------------------------------------------
void lv_port_disp_init(void)
{
  static lv_disp_draw_buf_t draw_buf;
  lv_disp_draw_buf_init(&draw_buf, display_buf[0], display_buf[1], DISP_HOR_RES*DISP_VER_RES);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.flush_cb = disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.full_refresh = true;
  disp_drv.antialiasing = 1;
  lv_disp_drv_register(&disp_drv);
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when it's finished.*/
void disp_flush(lv_disp_drv_t *disp_drv,
                const lv_area_t *area,
                lv_color_t *color_p)
{
    SDL_UpdateTexture(texture, NULL, color_p, DISP_HOR_RES * sizeof(lv_color_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    lv_disp_flush_ready(disp_drv);
}