#ifndef INC_MYMAIN_H_
#define INC_MYMAIN_H_
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart2;
void mymaininit();
void myloop();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
int _write(int fd, char* ptr, int len);
void mycallback();
#ifdef __cplusplus
}
#endif

#endif /* INC_MYMAIN_H_ */
