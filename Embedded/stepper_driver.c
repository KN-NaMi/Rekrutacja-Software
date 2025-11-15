#include "stm32f4xx_hal.h"

#include "stepper_driver.h"

static volatile uint32_t steps_to_move = 0;

TIM_HandleTypeDef htim2;

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

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1;
    htim2.Init.Period = 1000 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&htim2);

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void
stepper_move(struct StepperMotor *step_motor, MotorDirection dir, uint32_t steps)
{
    if (button_pressed)
    {
        if (dir == DIR_RIGHT)
        {
            HAL_GPIO_WritePin(step_motor->dir_port, step_motor->dir_pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(step_motor->dir_port, step_motor->dir_pin, GPIO_PIN_RESET);
        }

        steps_to_move = steps * 2;

        HAL_TIM_Base_Start_IT(&htim2);

        button_pressed = false;
    }
}

void
TIM2_IRQHandler(stryct StepperMotor *step_motor)
{
    if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)
    {
        if (steps_to_move > 0)
        {
            HAL_GPIO_TogglePin(step_motor->pulse_port, step_motor->pulse_pin);
            steps_to_move--;
        }
        else
        {

            HAL_GPIO_WritePin(step_motor->pulse_port, step->motorpulse_pin, GPIO_PIN_RESET);
            HAL_TIM_Base_Stop_IT(&htim2);
        }
    }

    __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
}
