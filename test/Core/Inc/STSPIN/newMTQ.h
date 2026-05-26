#ifndef _NEWMTQ_H_
#define _NEWMTQ_H_
#include "main.h"

#include "INA219/newINA.h"
#include "STSPIN/MTQdriver.h"
#include "newpwm.h"

#include <stdlib.h>

typedef struct {
	float resistant;
} mtq_profile_t;

mtq_profile_t mtq_default = { .resistant = 43.3, };

class NEWMTQ
{
private:
    MTQDRIVER *_driver;
    newINA *_current;
    mtq_profile_t *_coil;
    int16_t mtq_error_offset;

public:
    NEWMTQ(MTQDRIVER *driver,newINA *current,mtq_profile_t *coil = &mtq_default)
    {
        _driver = driver;
        _current = current;
        _coil = coil;
        mtq_error_offset = 0;//&_current->get_info()->offset_bit;
    }
    void set_current_sen(newINA *current) { _current = current; }
    void set_MTQDRIVER(MTQDRIVER *driver) { _driver = driver; }
    void set_coil(mtq_profile_t *coil) { _coil = coil; }

    //current_mA * Rsense = Vref
    //Vref / VpwmMax(3.3V) = PWM ratio
    //PWM cmd = PWM_ratio * PWM_maxcmd
//    float get_cmd_mA_const() const{
//        return (_info->Rsense/_info->VpwmMax*
//                _info->max_pwm_period*0.001);
//    }

    int16_t get_pwm_from_mA(int16_t current_cmd) {
//        return (int16_t)(current_cmd*get_cmd_mA_const());
    	return current_cmd;
    }
//
//    float get_mA_from_pwm(int16_t pwm) {
//        return (float)(pwm/get_cmd_mA_const());
//    }

    void run_current_mA(float cur_mA)
    {
        //todo
    }
    void run_raw_pwm(int16_t cur_pwm,uint32_t vol_pwm = 0x7FFF-2)
    {
        _driver->run_current_pwm(cur_pwm, vol_pwm);
    }
    float read_loadV()
    {
        float loadvoltage = _current->getBusVoltage_V() + (_current->getShuntVoltage_mV() / 1000);
        return loadvoltage;
    }
    float read_mA() { return read_raw()/_current->get_info()->current_div; }
    float read_Power() { return _current->getPower_mW(); }
    float read_Vsh() { return _current->getShuntVoltage_mV(); }
    uint8_t get_data_success() { return _current->get_success(); }
    int16_t read_raw() { return _current->getCurrent_raw()-mtq_error_offset; }
    
    void init()
    {
        _driver->begin();
        _current->begin(1); //1 for re-calibation
    }

    uint8_t reinit_cur_sen(current_info_t *info,uint8_t need_stop = 0)
    {
        if(need_stop == 1){
            run_raw_pwm(0,0);
        }
        _current->begin_calibration(info,need_stop); //1 for find offset_bit
        mtq_error_offset = _current->getCurrent_raw();
        return get_data_success();
        
    }

    void reinit_driver(uint32_t prescaler, uint32_t Period)
    {
        _driver->reinit(prescaler,Period);
    }

    
};

#endif
