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

typedef struct{
	uint32_t magicnum;
	double warning_threshhold;
	double critical_threshhold;
}_SETTINGS;
class Monitor{
private:
	struct thresholds{
		double warning = 26;
		double critical = 28;
	};
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
};

#endif /* INC_TEMPERTURE_MONITOR_H_ */
