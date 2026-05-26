#include "main.h"
#ifndef _NEWPWM_H_
#define _NEWPWM_H_

class NEWPWM
{
private:
    TIM_HandleTypeDef *_tim;
    uint32_t _Period;
    uint32_t _channel;

public:
    NEWPWM(TIM_HandleTypeDef *tim,uint32_t Period = 255)
    {
        _tim = tim;
        _Period = Period;
    }
    TIM_HandleTypeDef *getHandleTypeDef() { return _tim; }

    void set_Period(uint32_t period)
    {
        _Period = period;
    }
    void run_ch_pwm(uint32_t channel, uint16_t pwm)
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
