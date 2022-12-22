#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"
#include <stdint.h>
class Button {
private:
	GPIO_TypeDef* _GPIOx;
	uint16_t _GPIO_Pin;
public:
	Button(GPIO_TypeDef* GPIOx,
	uint16_t GPIO_Pin);

	int changeState();
};

#endif /* INC_BUTTON_H_ */
