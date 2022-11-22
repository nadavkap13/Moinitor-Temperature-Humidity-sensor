#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include "main.h"
#include "Cli.h"
#include "LED.h"
#include "Rtc.h"
#include "comtask.h"
#include "mybuzzer.h"

extern LED ledblue;
extern int blinkOn;
extern _RTC rtc;
extern CliContainer container;
extern BUZZER buzzer;
class ledOn : public Cli{
private:
	LED * _led;
public:
	ledOn(LED * led){
		_led = led;
	}
	void doCommand(const char * param) override{
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
class rtcsettime : public Cli{
private:
	_RTC * _rtc;
public:
	rtcsettime(_RTC * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		char * token;
		char s[3] = ":";
		char temp[50];
		strcpy(temp,param);
		DateTime tempdate;

		token = strtok(temp, s);
		tempdate.hours = atoi(token);
		token = strtok(NULL, s);

		tempdate.min = atoi(token);
		token = strtok(NULL, s);

		tempdate.sec = atoi(token);
		token = strtok(NULL, s);

		tempdate.weekDay = atoi(token);
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
class buzzeron : public Cli{
private:
	BUZZER * _buzzer;
public:
	buzzeron(BUZZER * buzzer){
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStartPlay();
	}
};
class buzzeroff : public Cli{
private:
	BUZZER * _buzzer;
public:
	buzzeroff(BUZZER * buzzer){
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStopPlay();
	}
};


void CliContainer::initCLIcontainer(){
	container.RegisterCommand("ledon",new ledOn(&ledblue));
	container.RegisterCommand("ledoff",new ledOff(&ledblue));
	container.RegisterCommand("ledblink",new ledBlink(&ledblue));
	container.RegisterCommand("settime",new rtcsettime(&rtc));
	container.RegisterCommand("gettime",new rtcgettime(&rtc));
	container.RegisterCommand("rtcstart",new rtcstart(&rtc));
	container.RegisterCommand("rtcstop",new rtcstop(&rtc));
	container.RegisterCommand("play",new buzzeron(&buzzer));
	container.RegisterCommand("stop",new buzzeroff(&buzzer));
}






















