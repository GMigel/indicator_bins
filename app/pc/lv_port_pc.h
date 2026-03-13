/*
 * lv_port_pc.h
 */

#pragma once

#ifndef LV_PORT_H_
#define LV_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DISP_HOR_RES    320
#define DISP_VER_RES    240

#include "lvgl.h"

/* Initialize low level display driver */
void lv_port_pc_init(void);
void lv_port_disp_init(void);
void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_PORT_H_ */
