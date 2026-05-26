#include "main.h"
#ifndef _NEWPWM_H_
#define _NEWPWM_H_

class NEWPWM
{
private:
    TIM_HandleTypeDef *_tim;
    uint32_t _Period;
    uint32_t _prescaler;
    uint8_t is_init = 1;
    float _Vmag;     

public:
    NEWPWM(TIM_HandleTypeDef *tim)
    {
        _tim = tim;
    }
    NEWPWM(TIM_HandleTypeDef *tim,uint32_t Period,uint32_t prescaler)
    {
        _tim = tim;
        _Period = Period;
        _prescaler = prescaler;
    }
    TIM_HandleTypeDef *getHandleTypeDef() { return _tim; }

    NEWPWM &operator=(TIM_HandleTypeDef *tim){
        _tim = tim;
        is_init = 1;
        return *this;
    }
    
    uint32_t &period(){
        return _Period;
    }

    uint32_t get_period(){
        return _Period;
    }

    uint32_t &prescaler(){
        return _prescaler;
    }

    uint32_t get_prescaler(){
        return _prescaler;
    }

    float &Vmag(){
        return _Vmag;
    }

    float get_Vmag(){
        return _Vmag;
    }

    uint8_t init(uint8_t is_init = 0)
    {   
        if(is_init == 1){ //check if normal init
            deinit();
        }
        if (HAL_TIM_PWM_Init(_tim) != HAL_OK)
        {

            Error_Handler();
        }
        return HAL_OK;
    }
    uint8_t reinit(uint32_t prescaler, uint32_t Period)
    {   
        set_info(prescaler,Period);
        return init(1);
    }

    uint8_t deinit()
    {
        return HAL_TIM_PWM_DeInit(_tim);
    }

    uint8_t set_info(uint32_t prescaler, uint32_t Period)
    {
        _Period = Period;
        _prescaler = prescaler;
        _tim->Init.Prescaler = _prescaler;
        _tim->Init.Period = _Period;
    }

    void run_ch_pwm(uint32_t channel, uint32_t pwm)
    {
        if (channel > TIM_CHANNEL_4)
        {
            return; // error
        }

        HAL_TIM_PWM_Stop(_tim, channel);
        switch (channel)
        {
        case TIM_CHANNEL_1:
            _tim->Instance->CCR1 = pwm;
            break;
        case TIM_CHANNEL_2:
            _tim->Instance->CCR2 = pwm;
            break;
        case TIM_CHANNEL_3:
            _tim->Instance->CCR3 = pwm;
            break;
        case TIM_CHANNEL_4:
            _tim->Instance->CCR4 = pwm;
            break;
        }
        HAL_TIM_PWM_Start(_tim, channel);
    }
    

};
#endif
