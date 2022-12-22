#include "Rtc.h"
#include "mymain.h"
#include <stdio.h>
#include "main.h"
#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0

static const int _daysUntilMonth[] = {
		0,
		31,
		59,
		90,
		120,
		151,
		181,
		212,
		243,
		273,
		304,
		334,
		365
};

Rtc::Rtc(I2C_HandleTypeDef * hi2c, uint32_t devAddr)
{
  _hi2c = hi2c;
  _devAddr = devAddr;
}

void Rtc::rtcStart()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	sec &= ~RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}

void Rtc::rtcStop()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c,_devAddr, 0, 1, &sec, 1, 0xFF);
	sec |= RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}

int Rtc::rtcIsRunning()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	return (sec & RTC_START_STOP) == 0;
}

static int bcdToInt(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t intToBcd(int value, int minVal, int maxVal)
{
	if (value < minVal || value > maxVal) {
		return 0;
	}

	return ((value / 10) << 4) | (value % 10);
}

void Rtc::rtcGetTime()
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];

	if(HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
		//printf("read is good\r\n");
	}
	else{
		printf("write is bad\r\n");
	}

	// remove stop bit if set
	buffer[0] &= ~RTC_START_STOP;
	_dateTime.sec = bcdToInt(buffer[0]);
	_dateTime.min = bcdToInt(buffer[1]);
	_dateTime.hours = bcdToInt(buffer[2]);
	_dateTime.weekDay = buffer[3] & 0x07;
	_dateTime.day = bcdToInt(buffer[4]);
	_dateTime.month = bcdToInt(buffer[5]);
	_dateTime.year = bcdToInt(buffer[6]);
	printf("date is %02d:%02d:%02d  %02d %02d/%02d/%02d \r\n ",_dateTime.hours,_dateTime.min,_dateTime.sec,_dateTime.weekDay,_dateTime.day,_dateTime.month,_dateTime.year);
}


DateTime * Rtc::rtcTimeToString(){
	uint8_t buffer[RTC_DATE_TIME_SIZE];

		if(HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
			//printf("read is good\r\n");
		}
		else{
			printf("write is bad\r\n");
		}

		// remove stop bit if set
		buffer[0] &= ~RTC_START_STOP;
		_dateTime.sec = bcdToInt(buffer[0]);
		_dateTime.min = bcdToInt(buffer[1]);
		_dateTime.hours = bcdToInt(buffer[2]);
		_dateTime.weekDay = buffer[3] & 0x07;
		_dateTime.day = bcdToInt(buffer[4]);
		_dateTime.month = bcdToInt(buffer[5]);
		_dateTime.year = bcdToInt(buffer[6]);
		return &_dateTime;
}
void Rtc::rtcSetTime(DateTime * _dateTime)
{

	uint8_t buffer[RTC_DATE_TIME_SIZE];

	buffer[0] = intToBcd(_dateTime->sec, 0, 59);
	buffer[1] = intToBcd(_dateTime->min, 0, 59);
	buffer[2] = intToBcd(_dateTime->hours, 0, 59);
	buffer[3] = _dateTime->weekDay < 1 || _dateTime->weekDay > 7 ? 0 : _dateTime->weekDay;
	buffer[4] = intToBcd(_dateTime->day, 1, 31);
	buffer[5] = intToBcd(_dateTime->month, 1, 12);
	buffer[6] = intToBcd(_dateTime->year, 1, 99);

	if(HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
		printf("write is good\r\n");
	}
	else{
		printf("write is bad\r\n");
	}
}
