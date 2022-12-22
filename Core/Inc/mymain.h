#ifndef INC_MYMAIN_H_
#define INC_MYMAIN_H_
#include "main.h"
#include "cmsis_os.h"
#ifdef __cplusplus
extern "C" {
#endif

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart2;
extern osSemaphoreId_t DHT_MONITORHandle;
extern I2C_HandleTypeDef hi2c1;
void myMainInit();
void myLoop();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void mySDInit();
int write(int fd, char* ptr, int len);
void myCallback();
void readTempFunc(void *argument);
void comtaskFunc(void *argument);
void monitorFunc(void *argument);
void blinkFunc(void *argument);
#ifdef __cplusplus
}
#endif

#endif /* INC_MYMAIN_H_ */
