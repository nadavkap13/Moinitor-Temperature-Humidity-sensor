#include <stdio.h>
#include "mymain.h"
#include "main.h"
#include "LED.h"

Led::Led(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin):
		_GPIOx(GPIOx),_GPIO_Pin(GPIO_Pin)
{
}

void Led::ledOn()
{
	_STATE = LED_STATE_ON;
	HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_SET);

}
void Led::ledOff()
{
	_STATE = LED_STATE_OFF;
	HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_RESET);

}
void Led::ledDelay(int num){
	_delay = num;
}
void Led::ledBlink()
{
	HAL_GPIO_TogglePin(_GPIOx, _GPIO_Pin);
	osDelay(_delay);

}
void Led::isBlink()
{
	_STATE = LED_STATE_BLINK;
}
LedState Led::getState()
{
	return _STATE;
}

