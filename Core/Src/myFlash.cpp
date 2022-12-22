#include <myFlash.h>
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mymain.h"
#include "temperture_monitor.h"


#define MAX_LENGTH 256
const uint32_t ADRESS = (0x08080000UL);
Flash * Flash:: _Instance = 0;
Flash * Flash::Instance()
 {
	 if(_Instance == 0){
		 _Instance = new Flash;
	 }
	 return _Instance;
 }
static void pageSelect (FLASH_EraseInitTypeDef * page)
{
	page->TypeErase = FLASH_TYPEERASE_PAGES;
	page->Banks = FLASH_BANK_2;
	page->Page = 256;
	page->NbPages = 1;
}
Flash::Flash()
{
	pageSelect(_page);
}
void pageErase()
{
	FLASH_EraseInitTypeDef page;
	uint32_t pageerror = 0;
	pageSelect(&page);
	HAL_FLASH_Unlock();
	if (HAL_FLASHEx_Erase(&page, &pageerror) != HAL_OK) {
		printf("Erase failed\r\n");
	}
}

void Flash::erase()
{
	HAL_FLASH_Unlock();
	static uint32_t pageerror = 0;
		if (HAL_FLASHEx_Erase(_page, &pageerror) != HAL_OK) {
			printf("Erase failed\r\n");
		}
}

void Flash::program(void * obj)
{
	uint32_t flashindex = 0;
	uint8_t* ptr = (uint8_t*)(obj);
	for (size_t i = 0 ; i < sizeof(thresHolds) ; i+=sizeof(uint64_t))
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADRESS+flashindex, *(uint64_t*)(ptr+flashindex)) != HAL_OK) {
			printf("Program failed\r\n");
			break;
		}
		flashindex+=sizeof(uint64_t);
	}
	HAL_FLASH_Lock();
}
