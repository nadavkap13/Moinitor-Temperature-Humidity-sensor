#ifndef INC_LED_ADSD_H_
#define INC_LED_ADSD_H_
#include "main.h"

typedef enum _LED_STATE{
	LED_STATE_OFF,
	LED_STATE_ON,
	LED_STATE_BLINK
}LED_STATE;

class LED {
private:
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	int blinkcount = 0;
	int delay = 50 ;
	LED_STATE STATE;

public:
	LED(GPIO_TypeDef* GPIOx,
	uint16_t GPIO_Pin);

	void Led_On();
	void Led_Off();
	void LED_delay(int num);
	void Led_Blink();
	void Is_blink();
	void LEDchangeState();
};


#endif /* INC_LED_ADSD_H_ */
