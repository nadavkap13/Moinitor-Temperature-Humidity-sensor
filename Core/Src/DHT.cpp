#include "main.h"
#include "DHT.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint8_t dht_byte= 0;
//static int dht_count=0;
static uint8_t m= 0;
static uint8_t p= 0;
GPIO_InitTypeDef gpioStruct;
extern uint8_t DHT_data[5];
 int delay=0;
//extern int startdelay;
extern TIM_HandleTypeDef htim1;

DHT::DHT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	_GPIOx = GPIOx;
	_GPIO_Pin = GPIO_Pin;
	dhtPin = WAKING;
	bitcount=0;

}

 void DHT::setGpioExti()
{
	GPIO_InitTypeDef gpioStruct = {0};
	gpioStruct.Pin = _GPIO_Pin;
	gpioStruct.Mode = GPIO_MODE_IT_FALLING;
	gpioStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(_GPIOx, &gpioStruct);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void DHT::Dht_readAsync()
{
	GPIO_InitTypeDef gpioStruct;
	gpioStruct.Pin = pin_4_Pin;
	gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioStruct.Pull = GPIO_NOPULL;
	gpioStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pin_4_GPIO_Port, &gpioStruct);

	HAL_GPIO_WritePin(pin_4_GPIO_Port, pin_4_Pin,GPIO_PIN_RESET);
	HAL_Delay(18);
	HAL_GPIO_WritePin(pin_4_GPIO_Port, pin_4_Pin,GPIO_PIN_SET);
	setGpioExti();

}

void DHT::Dht_onGpioInterrupt(uint16_t pin)
{
	uint32_t counter;
	if(_GPIO_Pin==pin){
		switch (dhtPin){
		case WAKING:
			dhtPin=WAIT_RESPONSE_START;
			break;
		case WAIT_RESPONSE_START:
			dhtPin=WAIT_RESPONSE_STOP;
			break;
		case WAIT_RESPONSE_STOP:
			dhtPin=RECEIVING_BITS;
			break;
		case RECEIVING_BITS:
			counter = __HAL_TIM_GET_COUNTER(&htim1);
			if(counter<=100)
			{
				dht_byte&= ~(1<<(7-p));
			}
			else dht_byte|= (1<<(7-p));

			p++;
			bitcount++;

			if(bitcount%8==0){
				DHT_data[m]=dht_byte;
				dht_byte=0;
				m++;
				p=0;
			}

			if(bitcount>=40){
				bitcount=0;
				m=0;

				dhtPin=DATA_RECEIVED;
			}
			break;
		case DATA_RECEIVED:
			counter = 0;
			break;
		}
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
}

int DHT::Dht_hasData(){
	if (dhtPin==DATA_RECEIVED){
//		startdelay=0;
//		delay =0;
		dhtPin=WAKING;
		return 1;
	}
	return 0;
}


//void DHT_onTimerInteruppt(DHT* dht){
// if(startdelay==1){
//	if (dht_count>=18){
//		delay=1;
//		dht_count=0;
//	}
//		dht_count++;
// }
//}















































