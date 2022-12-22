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
#include "temperture_monitor.h"
#include "SD_FILE.h"
#include "myFlash.h"

Dht dht = Dht(GPIOB,pin_4_Pin);
Rtc rtc = Rtc(&hi2c1,0xD0);
CliContainer container = CliContainer();
Led ledblue = Led(LD2_GPIO_Port, LD2_Pin);
Buzzer buzzer = Buzzer(&htim3);
Monitor monitor = Monitor();
FATFS FatFs; 	//Fatfs handle
FIL event_fil;//File handle
FIL temp_fil;//File handle
FRESULT fres; //Result after operations
File event_file = File(event_fil, FatFs, fres,"event.txt");
File temperature_file = File(temp_fil, FatFs, fres,"temp.txt");
Flash flash = Flash();
ComTask comtask = ComTask();
int buzz_off = 0;
int write_count = 0;


void mySDInit(){
		printf("\r\n~ SD card demo by nadav ~\r\n\r\n");
		HAL_Delay(1000); //a short delay is important to let the SD card settle
		//some variables for FatFs
		      FATFS _FatFs; 	//Fatfs handle
		      FRESULT _fres; //Result after operations

		//Open the file system
		_fres = f_mount(&_FatFs, "", 1); //1=mount now
		if (_fres != FR_OK) {
		printf("f_mount error (%i)\r\n", _fres);
		while(1);
		}
		printf("mount is good\r\n");
	}

int _write(int fd, char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	dht.dhtOnGpioInterrupt(pin_4_Pin);
	if(HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin) == 0){
		monitor.setState(CRITICAL_STATE_NO_BUZZER);
		buzzer.buzzerStopPlay();
	}

}

void myMainInit()
{
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim1);
	container.initCliContainer();
	mySDInit();

}

void myloop()
{

}

void monitorFunc(void *argument)
{
  /* USER CODE BEGIN 5 */
	 MointorState currentState;
	  /* Infinite loop */
  for(;;)
  {
  osSemaphoreAcquire(DHT_MONITORHandle, 0xFF);
  char data[200];
  char temperature_data[200];
  DateTime * currtime = rtc.rtcTimeToString();

	  if(dht.dhtHasData()){
		 if(monitor.getState() == CRITICAL_STATE_NO_BUZZER && dht.getTemperature()>= monitor.getCritical() && currentState != CRITICAL_STATE_NO_BUZZER ){
			 ledblue.isBlink();
			 		 }
		 else if(dht.getTemperature()>= monitor.getCritical() && currentState != CRITICAL_STATE && currentState != CRITICAL_STATE_NO_BUZZER){
			 monitor.setState(CRITICAL_STATE);
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is CRICTICAL- %f the current temp is %f, the temp is %f above the threshold\r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,monitor.getCritical(),dht.getTemperature(),(dht.getTemperature() - monitor.getCritical()) );
			 event_file.write(data);
			 ledblue.isBlink();
			 buzzer.buzzerStartPlay();

		 }
		 else if(dht.getTemperature()>= monitor.getWarning() && dht.getTemperature()< monitor.getCritical() && currentState != WARRNING_STATE){
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is WARNING- %f the current temp is %f, the temp is %f above the threshold\r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,monitor.getWarning(),dht.getTemperature(),(dht.getTemperature() - monitor.getWarning()) );
			 event_file.write(data);
			 monitor.setState(WARRNING_STATE);
			 ledblue.ledOn();
		 }
		 else if (dht.getTemperature()< monitor.getWarning() && currentState != NORMAL_STATE){
			 monitor.setState(NORMAL_STATE);
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is NORMAL current temp is %f \r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,dht.getTemperature());
			 event_file.write(data);
		 }
		 currentState = monitor.getState();
	   }

	  if(write_count == 59){
			 sprintf(temperature_data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d current temp is %f \r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,dht.getTemperature());
			 temperature_file.write(temperature_data);
			 write_count = 0;

	  }

  }
  /* USER CODE END 5 */
}

void comtaskFunc(void *argument)
{
	/* USER CODE BEGIN comtask_func */

	/* Infinite loop */
	for (;;) {

		if (comtask.commTask()) {
			comtask.handleCommand();
		}
	}
	/* USER CODE END comtask_func */
}
void readTempFunc(void *argument)
{
 /* USER CODE BEGIN monitor_func */
	int ticks = 0;
 /* Infinite loop */
 for(;;)
 {
	 ticks = xTaskGetTickCount();
	 ticks += 1000;
	 dht.dhtReadAsync();
	 write_count++;
	 osDelayUntil(ticks);
	 osSemaphoreRelease(DHT_MONITORHandle);

 }

 /* USER CODE END monitor_func */
}
void blinkFunc(void *argument)
{
 /* USER CODE BEGIN blink_func */
 /* Infinite loop */
 for(;;)
 {
	 if(ledblue.getState() == LED_STATE_BLINK){
		 ledblue.ledBlink();
	 }
	 else {
		 osThreadYield();
	 }
 }
 /* USER CODE END blink_func */
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
