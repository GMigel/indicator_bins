/*
 * lv_port.h
 *
 *  Created on: Jul 20, 2023
 *      Author: Иван
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/* Initialize low level display driver */
void lv_port_disp_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif
