#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void   input_init(void);
void   input_poll(void);        // PC: обрабатывает события
int8_t input_get_enc(void);     // возвращает ДЕЛЬТУ (+1 / -1 / 0)
bool   input_get_btn(void);     // текущее состояние кнопки

#ifdef __cplusplus
}
#endif