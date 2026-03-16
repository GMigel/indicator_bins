#include "disp_bright.h"
#include "tim.h"

void disp_bright_init(void) {
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // enable display backlight
}

void disp_bright_set(uint8_t brg) { __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, brg); }

uint8_t disp_bright_get(void) { return __HAL_TIM_GET_COMPARE(&htim12, TIM_CHANNEL_1); }
