#include <stdio.h>
#include "main.h"
#include "mymain.h"
#include "mybuzzer.h"
extern TIM_HandleTypeDef htim3;



int count = 0;
int frame = 0;

BUZZER::BUZZER(TIM_HandleTypeDef *htim)
{
	htim = htim;

}
void BUZZER::buzzerStartPlay()
{
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_AUTORELOAD(&htim3,150);
}
void BUZZER::buzzerStopPlay()
{
	HAL_TIM_Base_Stop(&htim3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

//void BUZZER::buzzerOnInterrupt()
//{
//	frame++;
//	if(frame >= frames[count] ){
//		__HAL_TIM_SET_COUNTER(htim,0);
//		__HAL_TIM_SET_AUTORELOAD(htim,song[count]/2);
//		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,((song[count]/2)/2));
//		count++;
//		frame = 0;
//	}
//	if(song[count]==0 && frames[count]== 0){
//		count=0;
//	}
//}

















