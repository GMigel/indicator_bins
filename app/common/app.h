#pragma once

#include <stdint.h>
#include "gui.h"

//--------------------------------------
const uint8_t MajVersion = 0;
const uint8_t MinVersion = 1;
const uint8_t Build = 1;
//--------------------------------------

#define APP_DEBUG 0
#define PI 3.141592653f

#ifdef __cplusplus
extern "C" {
#endif

extern gui_state_t gui_state;
extern volatile uint32_t systime;

extern gui_menu_t menu_main;
extern gui_menu_t menu_pres;
extern gui_menu_t menu_alt;
extern gui_menu_t menu_ver;

void app_run(void);
void app_on_timer(void);

void can_init(void);
void can_serve(void);
bool can_send_dat(uint16_t id, const void* data, uint8_t len);
void brg_rotate(int8_t delta);
void brg_click();
void brg_hold();
void ref_rotate(int8_t delta);
void ref_click();
void pres_rotate(int8_t delta);
void pres_click();
void menu_rotate(int8_t delta);
void menu_click();
void menu_hold();
void menu_timeout();
void menu_select(uint8_t item);
void pres_select(uint8_t item);
void alt_select(uint8_t item);
void ver_select(uint8_t item);

#ifdef __cplusplus
}
#endif
