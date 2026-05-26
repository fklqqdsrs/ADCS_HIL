#ifndef _MTQ_header_h
#define _MTQ_header_h

#include "main.h"
#include "INA219/newINA.h"
#include "STSPIN/MTQdriver.h"
//#include "@config/current_header.h"
#include "MTQpin.h"
//#if defined(__STM32G4xx_H)
//#include "driver/MTDrive/MTQpinG4.h"
//#elif defined(__STM32F7xx_HAL_CONF_H)
//#include "driver/MTDrive/MTQpin.h"
//#endif

#define MTQ_REF_PERIOD 2562 //ARR
#define MTQ_REF_PRESCLER 0
#define MTQ_DRIVER_RSENSE 5

NEWPWM mtq_ref_timer(MTQpin_X.ref_port,MTQ_REF_PERIOD,MTQ_REF_PRESCLER);
NEWPWM mtq_pwm_timer(MTQpin_X.tim_port,0xFFFF,0);

mtq_profile_t mtq_profile_x = {34};
mtq_profile_t mtq_profile_y = {34};
mtq_profile_t mtq_profile_z = {34};

mtq_driver_info_t mtq_driver_info_x ={
	.Rsense = MTQ_DRIVER_RSENSE,
	.pin = &MTQpin_X,
    .ref_timer = &mtq_ref_timer,
	.pwm_timer = &mtq_pwm_timer,
};

mtq_driver_info_t mtq_driver_info_y ={
	.Rsense = MTQ_DRIVER_RSENSE,
	.pin = &MTQpin_Y,
    .ref_timer = &mtq_ref_timer,
	.pwm_timer = &mtq_pwm_timer,
};

mtq_driver_info_t mtq_driver_info_z ={
	.Rsense = MTQ_DRIVER_RSENSE,
	.pin = &MTQpin_Z,
    .ref_timer = &mtq_ref_timer,
	.pwm_timer = &mtq_pwm_timer,
};

MTQDRIVER mtq_driver_x(&mtq_driver_info_x);
MTQDRIVER mtq_driver_y(&mtq_driver_info_y);
MTQDRIVER mtq_driver_z(&mtq_driver_info_z);

//NEWMTQ MTQx(&mtq_driver_x,&x_sensor,&mtq_fercoil);
//NEWMTQ MTQy(&mtq_driver_y,&y_sensor,&mtq_fercoil);
//NEWMTQ MTQz(&mtq_driver_z,&z_sensor,&mtq_aircoil);

uint8_t test_control = 0;
uint8_t state_mtqcc = 0;
uint32_t pwm_arr=65534;
int16_t mtq_max_pwm=MTQ_REF_PERIOD/5;
#endif
