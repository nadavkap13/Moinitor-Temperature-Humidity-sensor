#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include "main.h"
#include "Cli.h"
#include "LED.h"
#include "Rtc.h"
#include "comtask.h"
#include "mybuzzer.h"
#include "temperture_monitor.h"
#include "SD_FILE.h"
#include "myFlash.h"

extern Led ledblue;
extern int blinkOn;
extern Rtc rtc;
extern CliContainer container;
extern Buzzer buzzer;
extern File event_file;
extern File temperature_file;
extern Flash flash;
extern Monitor monitor;

class LedOn : public Cli{
private:
	Led * _led;
public:
	LedOn(Led * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->ledOn();
	}
};
class LedOff : public Cli{
private:
	Led * _led;
public:
	LedOff(Led * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->ledOff();
	}
};
class LedBlink : public Cli{
private:
	Led * _led;
public:
	LedBlink(Led * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		_led->ledBlink();
	}
};
class LedSetDelay : public Cli{
private:
	Led * _led;
public:
	LedSetDelay(Led * led){
		_led = led;
	}
	void doCommand(const char * param) override{
		int _param = atoi(param);
		_led->ledDelay(_param);
	}
};

class RtcStart : public Cli{
private:
	Rtc * _rtc;
public:
	RtcStart(Rtc * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStart();
	}
};
class RtcGetTime : public Cli{
private:
	Rtc * _rtc;
public:
	RtcGetTime(Rtc * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcGetTime();

	}
};
class RtcSetTime : public Cli{
private:
	Rtc * _rtc;
public:
	RtcSetTime(Rtc * rtc){
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
class RtcStop : public Cli{
private:
	Rtc * _rtc;
public:
	RtcStop(Rtc * rtc){
		_rtc = rtc;
	}
	void doCommand(const char * param) override{

		_rtc->rtcStop();
	}
};
class BuzzerOn : public Cli{
private:
	Buzzer * _buzzer;
public:
	BuzzerOn(Buzzer * buzzer){
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStartPlay();
	}
};
class BuzzerOff : public Cli{
private:
	Buzzer * _buzzer;
public:
	BuzzerOff(Buzzer * buzzer){
		_buzzer = buzzer;
	}
	void doCommand(const char * param) override{
		_buzzer->buzzerStopPlay();
	}
};
class SetWarning : public Cli{
private:
	Monitor * _monitor;
public:
	SetWarning(Monitor * monitor){
		_monitor = monitor;
	}
	void doCommand(const char * param) override{
		int _param = atoi(param);
		_monitor->setWarningValue(_param);
		flash.erase();
		flash.program(_monitor->getThresHolds());
	}
};
class SetCritical : public Cli{
private:
	Monitor * _monitor;
public:
	SetCritical(Monitor * monitor){
		_monitor = monitor;
	}
	void doCommand(const char * param) override{
		int _param = atoi(param);
		_monitor->setCriticalValue(_param);
		flash.erase();
		flash.program(_monitor->getThresHolds());
	}
};
class PrintFile : public Cli{
private:
	File * _file;
public:
	PrintFile(File * file){
		_file = file;
	}
	void doCommand(const char * param) override{
		_file->read();
	}
};
class DeleteFile : public Cli{
private:
	File * _file;
public:
	DeleteFile(File * file){
		_file = file;
	}
	void doCommand(const char * param) override{
		_file->deleteFile();
	}
};

void CliContainer::initCliContainer(){
	container.registerCommand("ledon",new LedOn(&ledblue));
	container.registerCommand("ledoff",new LedOff(&ledblue));
	container.registerCommand("settime",new RtcSetTime(&rtc));
	container.registerCommand("gettime",new RtcGetTime(&rtc));
	container.registerCommand("play",new BuzzerOn(&buzzer));
	container.registerCommand("stop",new BuzzerOff(&buzzer));
	container.registerCommand("printtemp",new PrintFile(&temperature_file));
	container.registerCommand("printlog",new PrintFile(&event_file));
	container.registerCommand("deletetemp",new DeleteFile(&temperature_file));
	container.registerCommand("deletelog",new DeleteFile(&event_file));
	container.registerCommand("setwarrning",new SetWarning(&monitor));
	container.registerCommand("setcritical",new SetCritical(&monitor));

}






















