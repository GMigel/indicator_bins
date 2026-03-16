#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void disp_bright_init(void);
void disp_bright_set(uint8_t brg);
uint8_t disp_bright_get(void);

#ifdef __cplusplus
}
#endif
