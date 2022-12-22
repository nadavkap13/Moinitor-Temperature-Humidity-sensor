#include <stdio.h>
#include "mymain.h"
#include "main.h"
#include "BUTTON.h"

Button::Button(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin):
				_GPIOx(GPIOx),_GPIO_Pin(GPIO_Pin)
{
}


int Button::changeState()
{
	if(HAL_GPIO_ReadPin(_GPIOx,_GPIO_Pin)==0){
		return 1;
	}
	return 0;
}
