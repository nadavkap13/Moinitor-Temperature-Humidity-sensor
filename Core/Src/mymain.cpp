#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mymain.h"
#include "main.h"
#include "mybuzzer.h"
#include "DHT.h"
#include "comtask.h"
#include "cmsis_os.h"
#include "Rtc.h"
#include "LED.h"

DHT dht = DHT(GPIOB,pin_4_Pin);
_RTC rtc = _RTC(&hi2c1,0xD0);
CliContainer container = CliContainer();
LED ledblue = LED(LD2_GPIO_Port, LD2_Pin);
BUZZER buzzer = BUZZER(&htim3);
int alarm_on = 0;

int _write(int fd, char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	dht.Dht_onGpioInterrupt(pin_4_Pin);
}
void mycallback(){

}

void mymaininit()
{
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim1);
	container.initCLIcontainer();
}

void myloop()
{
	if(dht.Dht_hasData()){

		}
}

void READ_TEMP_func(void *argument)
{
  /* USER CODE BEGIN 5 */
	  mymaininit();

  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(DHT_MONITORHandle, 0xFF);
	  if(dht.Dht_hasData()){

	  }

  }
  /* USER CODE END 5 */
}

void comtask_func(void *argument)
{
	/* USER CODE BEGIN comtask_func */

	/* Infinite loop */
	for (;;) {

		if (commTask()) {
			handleCommand();
		}
	}
	/* USER CODE END comtask_func */
}
void monitor_func(void *argument)
{
 /* USER CODE BEGIN monitor_func */
 /* Infinite loop */
 for(;;)
 {
	 osSemaphoreRelease(DHT_MONITORHandle);
	 dht.Dht_readAsync();
	 osDelay(1000);
 }

 /* USER CODE END monitor_func */
}

int song[] = {
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_5,NOTE_1,NOTE_3,NOTE_4,
		NOTE_3,NOTE_3,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_4,NOTE_5,
		NOTE_1,NOTE_3,NOTE_4,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_4,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_4,
		NOTE_4,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_7,NOTE_7,
		NOTE_1,NOTE_1,
		NOTE_2,NOTE_4,NOTE_7,
		NOTE_1,NOTE_3,NOTE_4,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_2,NOTE_2,
		NOTE_4,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_4,
		NOTE_4,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_4,
		NOTE_1,NOTE_1,
		NOTE_3,NOTE_2,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_1,NOTE_1,
		NOTE_7,NOTE_7,
		NOTE_1,NOTE_1,
		NOTE_2,NOTE_4,NOTE_7,
		0
	};
int length[]={
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_1,FRAME_1,FRAME_2,FRAME_2,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_2,FRAME_2,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_2,FRAME_2,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_0,FRAME_1,
		FRAME_2,FRAME_2,FRAME_0,
		0

};
