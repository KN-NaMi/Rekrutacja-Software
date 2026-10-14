#pragma once

#include "stm32f4xx_hal.h"  //? pewnie tu bylo sporo deklaracji wiec próbuję się nie czepiać rzeczy które dają error
#include <stdbool.h>
#include <string.h>

extern volatile bool button_pressed;

#define STEP_DIR_GPIO_Port GPIOB
#define STEP_DIR_Pin       GPIO_PIN_0

#define STEP_PULSE_GPIO_Port GPIOB  //! to samo STEP_DIR_GPIO_Port
#define STEP_PULSE_Pin       GPIO_PIN_1 

#define USER_BUTTON_GPIO_Port GPIOA
#define USER_BUTTON_Pin       GPIO_PIN_0 //! to samo STEP_DIR_Pin


void SystemClock_Config(void); //! NO IMPLEMENT WTF