#ifndef INC_MYBUZZER_H_
#define INC_MYBUZZER_H_
#include "main.h"


class Buzzer{
private:
	TIM_HandleTypeDef * _htim;
public:
	Buzzer(TIM_HandleTypeDef * htim);
	void buzzerStartPlay();
	void buzzerStopPlay();
	//void buzzerOnInterrupt();
};

enum NOTES{
   NOTE_1 = 381,
   NOTE_2 = 340,
   NOTE_3 = 322,
   NOTE_4 = 286,
   NOTE_5 = 255,
   NOTE_6 = 227,
   NOTE_7 = 202,

};
enum FRAME{
	FRAME_0 = 1000,
	FRAME_1 = 500,
	FRAME_2 = 255
};




#endif /* INC_MYBUZZER_H_ */
