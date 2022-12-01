#ifndef INC_TEMPERTURE_MONITOR_H_
#define INC_TEMPERTURE_MONITOR_H_
#include "main.h"
#include "stdint.h"
enum dht_STATE{
	NORMAL_STATE,
	WARRNING_STATE,
	CRITICAL_STATE,
	CRITICAL_STATE_NO_BUZZER
};

typedef struct thresholds{
	uint32_t magicnum = 123456;
	double warning = 100;
	double critical = 100;
}_SETTINGS;
class Monitor{
private:
	thresholds values;
	dht_STATE state;
public:
	Monitor(){}
	void SetWarningValue(double num)
	{
		values.warning = num;
	}
	void SetCriticalValue(double num)
	{
		values.critical = num;
	}
	double getwarning()
	{
		return values.warning;
	}
	double getcritical()
	{
		return values.critical;
	}
	dht_STATE getstate()
	{
		return state;
	}
	void setstate(dht_STATE State)
	{
		state = State;
	}
	thresholds * getthresholds()
	{
		return  &values;
	}
};

#endif /* INC_TEMPERTURE_MONITOR_H_ */
