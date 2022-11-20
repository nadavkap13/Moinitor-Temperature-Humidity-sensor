#ifndef INC_DHA_11_H_
#define INC_DHA_11_H_
#include "main.h"
#include <stdint.h>

typedef enum  {
	WAKING,
	WAIT_RESPONSE_START,
	WAIT_RESPONSE_STOP,
	RECEIVING_BITS,
	DATA_RECEIVED
}DHT_STATE;

class DHT{
private:
	GPIO_TypeDef* _GPIOx;
	uint16_t _GPIO_Pin;
	DHT_STATE dhtPin;
	int bitcount;
	int delay=0;
	uint8_t dht_byte= 0;
	uint8_t m= 0;
	uint8_t p= 0;
public:

	 DHT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void setGpioExti();
	void Dht_readAsync();
	void Dht_onGpioInterrupt(uint16_t pin);
	int Dht_hasData();
	void DHT_onTimerInteruppt();

};



#endif /* INC_DHA_11_H_ */
