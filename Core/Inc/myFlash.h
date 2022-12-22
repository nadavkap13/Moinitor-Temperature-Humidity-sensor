#ifndef INC_MYFLASH_H_
#define INC_MYFLASH_H_
#include <stdint.h>
#include "main.h"

class Flash{
private:
	static Flash * _Instance;
	FLASH_EraseInitTypeDef * _page = nullptr;
public:
	Flash();
	static Flash * Instance();
	void erase();
	void program(void * obj);
};
#endif /* INC_MYFLASH_H_ */
