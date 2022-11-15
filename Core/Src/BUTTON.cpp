#include <stdio.h>
#include "mymain.h"
#include "main.h"
#include "BUTTON.h"

BUTTON::BUTTON(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin):
				GPIOx(GPIOx),GPIO_Pin(GPIO_Pin)
{
}


int BUTTON::Change_State()
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==0){
		return 1;
	}
	return 0;
}
