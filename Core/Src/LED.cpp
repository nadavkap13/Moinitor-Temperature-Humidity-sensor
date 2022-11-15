#include <stdio.h>
#include "mymain.h"
#include "main.h"
#include "LED.h"

LED::LED(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,int blinkcount,int delay,int blink,LED_STATE STATE):
		GPIOx(GPIOx),GPIO_Pin(GPIO_Pin),blinkcount(blinkcount),delay(delay),blink(blink),STATE(STATE)
{
}

void LED::Led_On()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

}
void LED::Led_Off()
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

}
void LED::LED_delay(int num){
	delay = num;
}
void LED::Led_Blink()
{
	if (blinkcount >= delay){
		HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
		blinkcount = 0;
	}
		blinkcount++;
}
int LED::Is_blink()
{
	return blink;
}

void LED::LEDchangeState()
{
	switch (STATE){
	case LED_STATE_OFF:
		STATE = LED_STATE_ON;
		Led_On();
		break;
	case LED_STATE_ON:
		STATE = LED_STATE_BLINK;
		blink = 1;
			break;
	case LED_STATE_BLINK:
		STATE = LED_STATE_OFF;
		blink = 0;
		Led_Off();
			break;
	}
}
