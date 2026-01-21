#ifndef _MTQDRIVER_H_
#define _MTQDRIVER_H_
#include "main.h"
#include "newpwm.h"
#include <stdlib.h>



/*
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef *en_port;
    uint16_t en_pin;
    GPIO_TypeDef *stby_port;
    uint16_t stby_pin;
    TIM_HandleTypeDef *tim_port;
    uint16_t tim_ch;
    TIM_HandleTypeDef *ref_port;
    uint16_t ref_ch;
*/
typedef struct
{
    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef *en_port;
    uint16_t en_pin;
    GPIO_TypeDef *stby_port;
    uint16_t stby_pin;
    TIM_HandleTypeDef *tim_port;
    uint16_t tim_ch;
    TIM_HandleTypeDef *ref_port;
    uint16_t ref_ch;
} MTQpin_t;


/*
    uint32_t Rsense;
    MTQpin_t *pin;
    NEWPWM *ref_timer,*pwm_timer;
*/
typedef struct {
	uint32_t Rsense;
    MTQpin_t *pin;
    NEWPWM *ref_timer,*pwm_timer;
} mtq_driver_info_t;


class MTQDRIVER
{
private:
    mtq_driver_info_t *_info;
    MTQpin_t *_pin;
    int32_t _cur_pwm;
    uint32_t _pwm_period;
    
public:
    MTQDRIVER(mtq_driver_info_t *info,int32_t pwm_period = 0x7FFF)
    {
        set_info(info);
        _pwm_period = pwm_period;
    }

    
    void set_info(mtq_driver_info_t *info){
        _info = info;
        _pin = _info->pin;
    }

    /*
    need to set mtq_driver_info_t->ref_timer before use
    */
    void begin()
    {
        _info->pwm_timer->set_info(0,(uint32_t)_pwm_period);
        _info->pwm_timer->init(1);
        _info->ref_timer->reinit(_info->ref_timer->get_prescaler()
                                    ,_info->ref_timer->get_period());
        HAL_GPIO_WritePin(_pin->stby_port, _pin->stby_pin, GPIO_PIN_SET);
    }
    
    void set_pwm_timer(uint32_t cur_pwm,uint32_t volt_pwm){
        _info->ref_timer->run_ch_pwm(_pin->ref_ch,cur_pwm);
        _info->pwm_timer->run_ch_pwm(_pin->tim_ch,volt_pwm);
    }

    void run_current_pwm(int32_t current_pwm, uint32_t voltage_pwm)
    {
        _cur_pwm = current_pwm;
        static uint32_t ab_cur_pwm = 0;
        static uint16_t ab_voltage = 0;
        static int dir_state = 0;
        set_pwm_timer(0,0);
        HAL_GPIO_WritePin(_pin->en_port, _pin->en_pin, GPIO_PIN_RESET);
        if (current_pwm == 0 || voltage_pwm == 0)
            return;
        // set direction from pwm signed
        dir_state = (_cur_pwm > 0) ? 0 : 1;
        ab_cur_pwm = (!dir_state) ? _cur_pwm : -_cur_pwm;
        // saturating pwm
        ab_cur_pwm = (ab_cur_pwm > _pwm_period) ? _pwm_period : ab_cur_pwm;
        ab_voltage = voltage_pwm;

        HAL_GPIO_WritePin(_pin->dir_port, _pin->dir_pin, (GPIO_PinState)dir_state);
        set_pwm_timer(ab_cur_pwm,ab_voltage);
        HAL_GPIO_WritePin(_pin->en_port, _pin->en_pin, GPIO_PIN_SET);
    }   


    MTQpin_t *get_MTQpin() { return _pin; }
    uint8_t reinit(uint32_t Prescaler,uint32_t Period)
    {
       _info->ref_timer->reinit(Prescaler,Period);
       return 1;
    }
};

#endif
