#ifndef INC_DHA_11_H_
#define INC_DHA_11_H_
#include "main.h"
#include <stdint.h>

enum  DhtState{
	WAKING,
	WAIT_RESPONSE_START,
	WAIT_RESPONSE_STOP,
	RECEIVING_BITS,
	DATA_RECEIVED
};

class Dht{
private:
	GPIO_TypeDef* _GPIOx;
	uint16_t _GPIO_Pin;
	DhtState _dhtPin;
	double _Temperature;
	int _bitcount;
	int _delay=0;

public:

	 Dht(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void setGpioExti();
	void dhtReadAsync();
	void dhtOnGpioInterrupt(uint16_t pin);
	int dhtHasData();
	double getTemperature();


};



#endif /* INC_DHA_11_H_ */
