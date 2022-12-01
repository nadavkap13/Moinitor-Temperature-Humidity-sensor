#ifndef INC_MYFLASH_H_
#define INC_MYFLASH_H_
#include <stdint.h>
#include "main.h"

//typedef struct
//{
//  uint32_t TypeErase;   /*!< Mass erase or page erase.
//                             This parameter can be a value of @ref FLASH_Type_Erase */
//  uint32_t Banks;       /*!< Select bank to erase.
//                             This parameter must be a value of @ref FLASH_Banks
//                             (FLASH_BANK_BOTH should be used only for mass erase) */
//  uint32_t Page;        /*!< Initial Flash page to erase when page erase is disabled
//                             This parameter must be a value between 0 and (max number of pages in the bank - 1)
//                             (eg : 255 for 1MB dual bank) */
//  uint32_t NbPages;     /*!< Number of pages to be erased.
//                             This parameter must be a value between 1 and (max number of pages in the bank - value of initial page)*/
//} FLASH_EraseInitTypeDef;

class _Flash{
private:
	static _Flash * _Instance;
	FLASH_EraseInitTypeDef * _page = nullptr;
public:
	_Flash();
	static _Flash * Instance();
	void erase();
	void program(void * obj);
};
#endif /* INC_MYFLASH_H_ */
