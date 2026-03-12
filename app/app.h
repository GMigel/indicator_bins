#pragma once

#define APP_DEBUG 0
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef PLATFORM_PC
//#include "main.h"
#include "fdcan.h"
#else
#include <cstdint>
#endif


//--------------------------------------
// версия ПО
const uint8_t MajVersion = 0;
const uint8_t MinVersion = 1;
const uint8_t Build = 1;
//--------------------------------------

void app_run(void);
void app_on_timer(void);

//--------------------------------------

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
} // extern "C"
#endif
