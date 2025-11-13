#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define STEP_DIR_GPIO_Port  GPIOB
#define STEP_DIR_Pin        GPIO_PIN_0

#define STEP_PULSE_GPIO_Port GPIOB
#define STEP_PULSE_Pin       GPIO_PIN_1

#define USER_BUTTON_GPIO_Port GPIOA
#define USER_BUTTON_Pin       GPIO_PIN_0


void SystemClock_Config(void);
void Error_Handler(void);

extern bool button_pressed;

#endif
