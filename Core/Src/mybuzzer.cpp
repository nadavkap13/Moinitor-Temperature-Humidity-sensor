#include <stdio.h>
#include "main.h"
#include "mymain.h"
#include "mybuzzer.h"
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern int MainTimer_funcounter;


int count = 0;
int frame = 0;

BUZZER::BUZZER(TIM_HandleTypeDef *htim,int *song,int *frames)
{
	htim = htim;
	song = song;
	frames = frames;
}
void BUZZER::buzzerStartPlay()
{
	__HAL_TIM_SET_AUTORELOAD(htim,song[count]/2);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,((song[count]/2)/2));
	HAL_TIM_Base_Start(htim);
	HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);

}
void BUZZER::buzzerStopPlay()
{
	HAL_TIM_Base_Stop(htim);
	HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
	printf("count is %d\n\r",MainTimer_funcounter);
}

void BUZZER::buzzerOnInterrupt()
{
	frame++;
	if(frame >= frames[count] ){
		__HAL_TIM_SET_COUNTER(htim,0);
		__HAL_TIM_SET_AUTORELOAD(htim,song[count]/2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,((song[count]/2)/2));
		count++;
		frame = 0;
	}
	if(song[count]==0 && frames[count]== 0){
		count=0;
	}
}

















