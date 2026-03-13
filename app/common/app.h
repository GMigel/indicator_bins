#pragma once

#define APP_DEBUG 0
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

//--------------------------------------
// версия ПО
const uint8_t MajVersion = 0;
const uint8_t MinVersion = 1;
const uint8_t Build = 1;
//--------------------------------------

#define PI			3.141592653f

void app_run(void);
void app_on_timer(void);

#ifdef __cplusplus
} // extern "C"
#endif
