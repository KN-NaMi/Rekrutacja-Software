#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

typedef enum {
    DIR_LEFT,
    DIR_RIGHT
} MotorDirection;

struct StepperMotor {
	GPIO_TypeDef *dir_port;
	uint32_t dir_pin;
	GPIO_TypeDef *pulse_port;
	uint32_t pulse_pin;
	TIM_HandleTypeDef htim2;
};

void stepper_init(struct StepperMotor *step_motor, GPIO_TypeDef *dir_port, uint32_t dir_pin, GPIO_TypeDef *pulse_port, uint32_t pulse_pin);

void stepper_move(struct StepperMotor *step_motor, MotorDirection dir, uint32_t steps);

#endif

