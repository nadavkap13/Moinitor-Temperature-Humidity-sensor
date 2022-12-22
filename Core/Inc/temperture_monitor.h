#ifndef INC_TEMPERTURE_MONITOR_H_
#define INC_TEMPERTURE_MONITOR_H_
#include "main.h"
#include "stdint.h"
enum MointorState{
	NORMAL_STATE,
	WARRNING_STATE,
	CRITICAL_STATE,
	CRITICAL_STATE_NO_BUZZER
};

struct thresHolds{
	uint32_t _magicnum = 123456;
	double _warning = 100;
	double _critical = 100;
};
class Monitor{
private:
	thresHolds _values;
	MointorState _state;
public:
	Monitor(){}
	void setWarningValue(double num)
	{
		_values._warning = num;
	}
	void setCriticalValue(double num)
	{
		_values._critical = num;
	}
	double getWarning()
	{
		return _values._warning;
	}
	double getCritical()
	{
		return _values._critical;
	}
	MointorState getState()
	{
		return _state;
	}
	void setState(MointorState State)
	{
		_state = State;
	}
	thresHolds * getThresHolds()
	{
		return  &_values;
	}
};

#endif /* INC_TEMPERTURE_MONITOR_H_ */
