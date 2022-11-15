#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"
#include <stdint.h>
class BUTTON {
private:
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
public:
	BUTTON(GPIO_TypeDef* GPIOx,
	uint16_t GPIO_Pin);

	int Change_State();
};

#endif /* INC_BUTTON_H_ */
