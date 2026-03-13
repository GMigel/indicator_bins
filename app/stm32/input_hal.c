#include "input_hal.h"

#include "tim.h"
#include "main.h"

void input_init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
}

int8_t input_get_enc(void)
{
    return (__HAL_TIM_GET_COUNTER(&htim2) >> 1) & 0xFF;
}

bool input_get_btn(void)
{
    return (HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == GPIO_PIN_RESET);
}