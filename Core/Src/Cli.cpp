#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include "main.h"
#include "Cli.h"
#include "LED.h"
#include "Rtc.h"
#include "comtask.h"

extern LED ledblue;
extern int blinkOn;
extern _RTC rtc;
class ledOn : public Cli{
private:
	LED * _led;
public:
	ledOn(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		blinkOn = 0;
		_led->Led_On();
	}
};
class ledOff : public Cli{
private:
	LED * _led;
public:
	ledOff(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		blinkOn = 0;
		_led->Led_Off();
	}
};
class ledBlink : public Cli{
private:
	LED * _led;
public:
	ledBlink(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		blinkOn = 1;
		_led->Led_Blink();
	}
};
class ledSetDelay : public Cli{
private:
	LED * _led;
public:
	ledSetDelay(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		blinkOn = 0;
		int _param = atoi(param);
		_led->LED_delay(_param);
	}
};

class rtcstart : public Cli{
private:
	_RTC * _rtc;
public:
	rtcstart(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStart();
	}
};
class rtcsettime : public Cli{
private:
	_RTC * _rtc;
public:
	rtcsettime(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{
		char s[3] = ":";
		char *token;
		DateTime tempdate;
			token = strtok(NULL, s);
			tempdate.min = atoi(token);
			token = strtok(NULL, s);

			tempdate.hours = atoi(token);
			token = strtok(NULL, s);

			tempdate.day = atoi(token);
			token = strtok(NULL, s);

			tempdate.month = atoi(token);
			token = strtok(NULL, s);

			tempdate.year = atoi(token);
			token = strtok(NULL, s);
		_rtc->rtcSetTime(&tempdate);
	}
};
class rtcstop : public Cli{
private:
	_RTC * _rtc;
public:
	rtcstop(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStop();
	}
};
class rtcgettime : public Cli{
private:
	_RTC * _rtc;
public:
	rtcgettime(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcGetTime();
	}
};

void initCLI(){
//	RegisterCommand("ledon",new ledOn(&ledblue));
//	RegisterCommand("ledoff",new ledOff(&ledblue));
//	RegisterCommand("ledblink",new ledBlink(&ledblue));
//	RegisterCommand("setdelay",new ledSetDelay(&ledblue));
	RegisterCommand("settime",new rtcsettime(rtc))
}






















