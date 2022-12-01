#include "Rtc.h"
#include "mymain.h"
#include <stdio.h>
#include "main.h"
#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0

const uint32_t DaysInYear    = 365;
const uint32_t SecondsInMin  = 60;
const uint32_t SecondsInHour = 3600;
const uint32_t SecondsInDay  = 86400;

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

_RTC::_RTC(I2C_HandleTypeDef * hi2c, uint32_t devAddr)
{
  _hi2c = hi2c;
  _devAddr = devAddr;
}

void _RTC::rtcStart()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	sec &= ~RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}

void _RTC::rtcStop()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c,_devAddr, 0, 1, &sec, 1, 0xFF);
	sec |= RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}

int _RTC::rtcIsRunning()
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

void _RTC::rtcGetTime()
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
	dateTime.sec = bcdToInt(buffer[0]);
	dateTime.min = bcdToInt(buffer[1]);
	dateTime.hours = bcdToInt(buffer[2]);
	dateTime.weekDay = buffer[3] & 0x07;
	dateTime.day = bcdToInt(buffer[4]);
	dateTime.month = bcdToInt(buffer[5]);
	dateTime.year = bcdToInt(buffer[6]);
	printf("date is %02d:%02d:%02d  %02d %02d/%02d/%02d \r\n ",dateTime.hours,dateTime.min,dateTime.sec,dateTime.weekDay,dateTime.day,dateTime.month,dateTime.year);
}

//uint32_t _RTC::rtcGetSeconds()
//{
//	// calculate seconds from 00:00:00 1/1/2020
//	dateTime;
//	rtcGetTime(&dateTime);
//
//	uint32_t seconds = dateTime.sec +
//			dateTime.min * SecondsInMin +
//			dateTime.hours * SecondsInHour +
//			dateTime.day * SecondsInDay +
//			_daysUntilMonth[dateTime.month - 1] * SecondsInDay +
//			dateTime.year * DaysInYear * SecondsInDay;
//	if (dateTime.year % 4 == 0 && dateTime.month > 2) {
//		// if current year is a leap year and month is after February
//		// add seconds for February 29
//		seconds += SecondsInDay;
//	}
//	// add seconds for all previous leap years
//	seconds += (dateTime.year / 4) * SecondsInDay;
//	return seconds;
//}

DateTime * _RTC::rtcTimeToString(){
	uint8_t buffer[RTC_DATE_TIME_SIZE];

		if(HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF) == HAL_OK){
			//printf("read is good\r\n");
		}
		else{
			printf("write is bad\r\n");
		}

		// remove stop bit if set
		buffer[0] &= ~RTC_START_STOP;
		dateTime.sec = bcdToInt(buffer[0]);
		dateTime.min = bcdToInt(buffer[1]);
		dateTime.hours = bcdToInt(buffer[2]);
		dateTime.weekDay = buffer[3] & 0x07;
		dateTime.day = bcdToInt(buffer[4]);
		dateTime.month = bcdToInt(buffer[5]);
		dateTime.year = bcdToInt(buffer[6]);
		return &dateTime;
}
void _RTC::rtcSetTime(DateTime * _dateTime)
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
