#include "main.h"
#include "DHT.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

GPIO_InitTypeDef gpioStruct;
static uint8_t DHT_data[5];
static uint8_t dht_byte= 0;
static	uint8_t m= 0;
static	uint8_t p= 0;
extern TIM_HandleTypeDef htim1;

Dht::Dht(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	_GPIOx = GPIOx;
	_GPIO_Pin = GPIO_Pin;
	_dhtPin = WAKING;
	_bitcount=0;
}

 void Dht::setGpioExti()
{
	GPIO_InitTypeDef gpioStruct = {0};
	gpioStruct.Pin = _GPIO_Pin;
	gpioStruct.Mode = GPIO_MODE_IT_FALLING;
	gpioStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(_GPIOx, &gpioStruct);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void Dht::dhtReadAsync()
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

void Dht::dhtOnGpioInterrupt(uint16_t pin)
{
	uint32_t counter;
	if(_GPIO_Pin==pin){
		switch (_dhtPin){
			case WAKING:
				_dhtPin=WAIT_RESPONSE_START;
				break;
			case WAIT_RESPONSE_START:
				_dhtPin=WAIT_RESPONSE_STOP;
				break;
			case WAIT_RESPONSE_STOP:
				counter = __HAL_TIM_GET_COUNTER(&htim1);
				_dhtPin=RECEIVING_BITS;
				break;
			case RECEIVING_BITS:
				counter = __HAL_TIM_GET_COUNTER(&htim1);
				if(counter<=100)  {
					dht_byte&= ~(1<<(7-p));
				}
				else {
					dht_byte|= (1<<(7-p));
				}
				p++;
				_bitcount++;
				if(_bitcount%8==0){
					DHT_data[m]=dht_byte;
					dht_byte=0;
					m++;
					p=0;
				}

				if(_bitcount>=40){
					_bitcount=0;
					m=0;
				_dhtPin=DATA_RECEIVED;
				}
				break;
			case DATA_RECEIVED:
				counter = 0;
				break;
		}
				__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
}

int Dht::dhtHasData(){
	if (_dhtPin==DATA_RECEIVED){
		_Temperature = (double)DHT_data[2];
		_dhtPin=WAKING;
		return 1;
	}
	return 0;
}
double Dht::getTemperature()
{
	return _Temperature;
}
