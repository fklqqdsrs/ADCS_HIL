#ifndef _MTQ_pinG4_h
#define _MTQ_pinG4_h
#include "main.h"
#include "driver/MTDrive/STSPIN/MTQdriver.h"

MTQpin_t MTQpin_X = {
 MTQ1_DIR_GPIO_Port, MTQ1_DIR_Pin,
 MTQ1_EN_GPIO_Port ,MTQ1_EN_Pin,
 MTQ1_STBY_GPIO_Port, MTQ1_STBY_Pin,
   &htim20, TIM_CHANNEL_2,
   &htim1, TIM_CHANNEL_1

};
MTQpin_t MTQpin_Y = {
 MTQ2_DIR_GPIO_Port, MTQ2_DIR_Pin,
 MTQ2_EN_GPIO_Port ,MTQ2_EN_Pin,
 MTQ2_STBY_GPIO_Port, MTQ2_STBY_Pin,
   &htim20, TIM_CHANNEL_3,
   &htim1, TIM_CHANNEL_2
};
MTQpin_t MTQpin_Z = {
 MTQ3_DIR_GPIO_Port, MTQ3_DIR_Pin,
 MTQ3_EN_GPIO_Port ,MTQ3_EN_Pin,
 MTQ3_STBY_GPIO_Port, MTQ3_STBY_Pin,
   &htim20, TIM_CHANNEL_1,
   &htim1, TIM_CHANNEL_3
};

#endif // !_MTQ_pin_h

