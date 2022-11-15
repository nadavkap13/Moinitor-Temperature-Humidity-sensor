#ifndef INC_MYMAIN_H_
#define INC_MYMAIN_H_
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart2;
void mymain();
void myloop();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
int _write(int fd, char* ptr, int len);
#ifdef __cplusplus
}
#endif

#endif /* INC_MYMAIN_H_ */
