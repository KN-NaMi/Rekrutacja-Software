#include "main.h"

typedef enum {
    DIR_LEFT,
    DIR_RIGHT
} motor_direction;

void stepper_init(void);

void stepper_move(motor_direction dir, uint32_t steps);
