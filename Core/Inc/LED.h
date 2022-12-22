#ifndef INC_LED_ADSD_H_
#define INC_LED_ADSD_H_
#include "main.h"

 enum LedState{
	LED_STATE_OFF,
	LED_STATE_ON,
	LED_STATE_BLINK
};

class Led {
private:
	GPIO_TypeDef* _GPIOx;
	uint16_t _GPIO_Pin;
	int _blinkcount = 0;
	int _delay = 50 ;
	LedState _STATE;

public:
	Led(GPIO_TypeDef* GPIOx,
	uint16_t GPIO_Pin);

	void ledOn();
	void ledOff();
	void ledDelay(int num);
	void ledBlink();
	void isBlink();
	LedState getState();
	void ledChangeState();
};


#endif /* INC_LED_ADSD_H_ */
