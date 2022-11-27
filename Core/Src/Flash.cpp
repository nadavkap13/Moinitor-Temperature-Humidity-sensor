#include "Flash.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mymain.h"
#include "temperture_monitor.h"


#define MAX_LENGTH 256
static int flashindex = 0;
#define ALARM_MAX 32
const uint32_t ADRESS = (0x08080000UL);


//void getPageInfo(uint32_t addr)
//{
//	addr = addr - 0x08000000UL;
//	uint32_t pageNum = addr / 2048;
//	uint32_t bank = page > 255 ? FLASH_BANK_2 : FLASH_BANK_1;
//}

void pageSelect (FLASH_EraseInitTypeDef * page)
{
	page->TypeErase = FLASH_TYPEERASE_PAGES;
	page->Banks = FLASH_BANK_2;
	page->Page = 256;
	page->NbPages = 1;
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

//void pageProgram ()
//{
//	uint32_t flashindex = 0;
//	uint8_t* ptr = (uint8_t*)(ALARMS);
//
//	for (int i = 0 ; i<(32*8); i++)
//	{
//		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADRESS+flashindex, *(uint64_t*)(ptr+flashindex)) != HAL_OK) {
//			printf("Program failed\r\n");
//			break;
//		}
//		flashindex+=sizeof(uint64_t);
//	}
//	HAL_FLASH_Lock();
//}

void eraseIT()
{
	FLASH_EraseInitTypeDef page;
	pageSelect(&page);
	HAL_FLASH_Unlock();

	if (HAL_FLASHEx_Erase_IT(&page) != HAL_OK) {
		printf("Erase failed\r\n");
	}

}
void programIT()
{
	if (flashindex <= MAX_LENGTH ){

		if (HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_DOUBLEWORD, ADRESS+flashindex, *(uint64_t*)(flashindex)) != HAL_OK) {
			printf("Program failed\r\n");
		}
			flashindex+=sizeof(uint64_t);
	}
	HAL_FLASH_Lock();
}
