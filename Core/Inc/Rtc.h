#ifndef INC_RTC_H_
#define INC_RTC_H_
#include "main.h"

struct DateTime
{
	int sec;
	int min;
	int hours;
	int weekDay;
	int day;
	int month;
	int year;
};

class Rtc{
private :
	I2C_HandleTypeDef * _hi2c;
	uint8_t _devAddr;
	DateTime _dateTime;
public:

	 Rtc(I2C_HandleTypeDef * hi2c, uint32_t devAddr);

	void rtcStart();

	void rtcStop();

	int rtcIsRunning();

	void rtcGetTime();

	DateTime * rtcTimeToString();

	void rtcSetTime(DateTime * _datetime);
};




#endif /* INC_RTC_H_ */
