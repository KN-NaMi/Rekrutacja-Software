#include "stm32f4xx_hal.h"

#include "stepper_driver.h"

static volatile uint32_t steps_to_move = 0;
static struct StepperMotor *active_motor = NULL;

void stepper_init(struct StepperMotor *step_motor, GPIO_TypeDef *dir_port, uint32_t dir_pin, GPIO_TypeDef *pulse_port, uint32_t pulse_pin);
void stepper_move(struct StepperMotor *step_motor, MotorDirection dir, uint32_t steps);
void TIM2_IRQHandler();

void
stepper_init(struct StepperMotor *step_motor, GPIO_TypeDef *dir_port, uint32_t dir_pin, GPIO_TypeDef *pulse_port, uint32_t pulse_pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();

	step_motor->dir_port = dir_port;
	step_motor->dir_pin = dir_pin;
	step_motor->pulse_port = pulse_port;
	step_motor->pulse_pin = pulse_pin;

	GPIO_InitStruct.Pin = dir_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dir_port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = pulse_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pulse_port, &GPIO_InitStruct);

	step_motor->htim2.Instance = TIM2;
	step_motor->htim2.Init.Prescaler = 84 - 1;
	step_motor->htim2.Init.Period = 1000 - 1;
	step_motor->htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&step_motor->htim2);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void
stepper_move(struct StepperMotor *step_motor, MotorDirection dir, uint32_t steps)
{
	active_motor = step_motor;
	if (dir == DIR_RIGHT) {
		HAL_GPIO_WritePin(step_motor->dir_port, step_motor->dir_pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(step_motor->dir_port, step_motor->dir_pin, GPIO_PIN_RESET);
	}

	steps_to_move = steps * 2;

	HAL_TIM_Base_Start_IT(&step_motor->htim2);
}

void
TIM2_IRQHandler()
{
	if (active_motor && __HAL_TIM_GET_FLAG(&active_motor->htim2, TIM_FLAG_UPDATE) != RESET) {
		if (steps_to_move > 0) {
			HAL_GPIO_TogglePin(active_motor->pulse_port, active_motor->pulse_pin);
			steps_to_move--;
		} else {

			HAL_GPIO_WritePin(active_motor->pulse_port, active_motor->pulse_pin, GPIO_PIN_RESET);
			HAL_TIM_Base_Stop_IT(&active_motor->htim2);
		}
	}

	__HAL_TIM_CLEAR_IT(&active_motor->htim2, TIM_IT_UPDATE);
}
