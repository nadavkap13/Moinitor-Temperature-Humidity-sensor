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

DHT dht = DHT(GPIOB,pin_4_Pin);
_RTC rtc = _RTC(&hi2c1,0xD0);
CliContainer container = CliContainer();
LED ledblue = LED(LD2_GPIO_Port, LD2_Pin);
BUZZER buzzer = BUZZER(&htim3);
Monitor monitor = Monitor();
FATFS FatFs; 	//Fatfs handle
FIL event_fil;//File handle
FIL temp_fil;//File handle
FRESULT fres; //Result after operations
_FILE event_file = _FILE(event_fil, FatFs, fres,"event.txt");
_FILE temperature_file = _FILE(temp_fil, FatFs, fres,"temp.txt");
_Flash flash = _Flash();
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

		//Let's get some statistics from the SD card
//		DWORD free_clusters, free_sectors, total_sectors;
//
//		FATFS* getFreeFs;
//
//		_fres = f_getfree("", &free_clusters, &getFreeFs);
//		if (_fres != FR_OK) {
//		printf("f_getfree error (%i)\r\n", _fres);
//		while(1);
//		}
//
//		//Formula comes from ChaN's documentation
//		total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
//		free_sectors = free_clusters * getFreeFs->csize;

		//printf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);
		printf("mount is good\r\n");
	}

int _write(int fd, char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	dht.Dht_onGpioInterrupt(pin_4_Pin);
	if(HAL_GPIO_ReadPin(button_GPIO_Port, button_Pin) == 0){
		monitor.setstate(CRITICAL_STATE_NO_BUZZER);
		buzzer.buzzerStopPlay();
	}

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
	mySDInit();

}

void myloop()
{

}

void monitor_func(void *argument)
{
  /* USER CODE BEGIN 5 */
	 dht_STATE currentState;
	  /* Infinite loop */
  for(;;)
  {
  osSemaphoreAcquire(DHT_MONITORHandle, 0xFF);
  char data[200];
  char temperature_data[200];
  DateTime * currtime = rtc.rtcTimeToString();

	  if(dht.Dht_hasData()){
		 if(monitor.getstate() == CRITICAL_STATE_NO_BUZZER && dht.get_temperature()>= monitor.getcritical() && currentState != CRITICAL_STATE_NO_BUZZER ){
			 ledblue.Is_blink();
			 		 }
		 else if(dht.get_temperature()>= monitor.getcritical() && currentState != CRITICAL_STATE && currentState != CRITICAL_STATE_NO_BUZZER){
			 monitor.setstate(CRITICAL_STATE);
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is CRICTICAL- %f the current temp is %f, the temp is %f above the threshold\r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,monitor.getcritical(),dht.get_temperature(),(dht.get_temperature() - monitor.getcritical()) );
			 event_file.write(data);
			 ledblue.Is_blink();
			 buzzer.buzzerStartPlay();

		 }
		 else if(dht.get_temperature()>= monitor.getwarning() && dht.get_temperature()< monitor.getcritical() && currentState != WARRNING_STATE){
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is WARNING- %f the current temp is %f, the temp is %f above the threshold\r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,monitor.getwarning(),dht.get_temperature(),(dht.get_temperature() - monitor.getwarning()) );
			 event_file.write(data);
			 monitor.setstate(WARRNING_STATE);
			 ledblue.Led_On();
		 }
		 else if (dht.get_temperature()< monitor.getwarning() && currentState != NORMAL_STATE){
			 monitor.setstate(NORMAL_STATE);
			 sprintf(data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d the log severity is NORMAL current temp is %f \r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,dht.get_temperature());
			 event_file.write(data);
		 }
		 currentState = monitor.getstate();
	   }

	  if(write_count == 59){
			 sprintf(temperature_data,"date is %02d:%02d:%02d  %02d %02d/%02d/%02d current temp is %f \r\n",currtime->hours,currtime->min,currtime->sec,currtime->weekDay,currtime->day,currtime->month,currtime->year,dht.get_temperature());
			 temperature_file.write(temperature_data);
			 write_count = 0;

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
void READ_TEMP_func(void *argument)
{
 /* USER CODE BEGIN monitor_func */
 /* Infinite loop */
 for(;;)
 {

	 dht.Dht_readAsync();
	 write_count++;
	 osDelay(1000);
	 osSemaphoreRelease(DHT_MONITORHandle);

 }

 /* USER CODE END monitor_func */
}
void blink_func(void *argument)
{
 /* USER CODE BEGIN blink_func */
 /* Infinite loop */
 for(;;)
 {
	 if(ledblue.getState() == LED_STATE_BLINK){
		 ledblue.Led_Blink();
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
