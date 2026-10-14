#pragma once

#include <stdint.h>
#include "main.h"

typedef enum
{
    DIR_LEFT,
    DIR_RIGHT
} MotorDirection;

void stepper_init(void);

void stepper_move(MotorDirection dir, uint32_t steps);
