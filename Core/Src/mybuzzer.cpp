#include <stdio.h>
#include "main.h"
#include "mymain.h"
#include "mybuzzer.h"

extern TIM_HandleTypeDef htim3;
int count = 0;
int frame = 0;

Buzzer::Buzzer(TIM_HandleTypeDef *htim)
{
	htim = htim;

}
void Buzzer::buzzerStartPlay()
{
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_AUTORELOAD(&htim3,150);
}
void Buzzer::buzzerStopPlay()
{
	HAL_TIM_Base_Stop(&htim3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

















