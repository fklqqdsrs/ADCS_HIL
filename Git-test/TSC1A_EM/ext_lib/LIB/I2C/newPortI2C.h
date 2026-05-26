#ifndef _newPortI2C_h_
#define _newPortI2C_h_

#include "NEWIO/newPort.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
#include "NEWIO/newHeader.h"



enum I2C_state
{
    stop,
    waiting,
    getRegisterAddress,
    getData,
    sendData,
    needReset,
};

class newPortI2C : public newPort
{
private:
    I2C_HandleTypeDef *_i2c;
    I2C_state _state = I2C_state::stop;
    uint8_t _read_reg_id = 0;
    KMUTNB_ADCS_REG *_reg;
    uint32_t I2C_TICK_CHECK,I2C_TICK_CALL;

public:
    newPortI2C(I2C_HandleTypeDef *i2c, uint8_t mode_t = AS_MASTER) : newPort(mode_t)
    {
        _i2c = i2c;
        this->mode(mode_t);
    }

    I2C_HandleTypeDef *getHandleTypeDef() { return _i2c; }

    void writeBytes(const uint8_t addr, uint8_t data[], uint8_t len)
    {
        HAL_I2C_Master_Transmit(_i2c, addr, data, len, get_timeout());
    }

    uint8_t readByte(const uint8_t addr, uint8_t *buffer)
    {
        return HAL_I2C_Master_Receive(_i2c, addr, buffer, 1, get_timeout());
    }
    uint8_t readBytes(const uint8_t addr, uint8_t *buffer, uint8_t len)
    {
        return HAL_I2C_Master_Receive(_i2c, addr, buffer, len, get_timeout());
    }
    uint8_t readMEMs(const uint8_t addr, const uint16_t reg, uint8_t *buffer, const uint8_t len)
    {
        return HAL_I2C_Mem_Read(_i2c, addr, reg, 1, buffer, len, get_timeout());
    }
    uint8_t writeMEMs(const uint8_t addr, const uint16_t reg, uint8_t *buffer, const uint8_t len)
    {
        return HAL_I2C_Mem_Write(_i2c, addr, reg, 1, buffer, len, get_timeout());
    }

    uint8_t check_restart(uint32_t timeout = 1000){
        I2C_TICK_CHECK = HAL_GetTick();
        if (I2C_TICK_CHECK - I2C_TICK_CALL < timeout)
        {
            return 0;
        }

        //need restart
        _state = I2C_state::needReset;
        I2C_TICK_CALL = HAL_GetTick();
        return 1;
    }
    uint8_t deinit(){
        uint8_t ret = 0;
        ret = HAL_I2C_DeInit(_i2c);
        if (ret != HAL_OK)
        {
            Error_Handler();
        }
        return ret;
    }
    uint8_t init()
    {
        uint8_t ret = 0;

        ret = HAL_I2C_Init(_i2c);
        if (ret != HAL_OK)
        {
            Error_Handler();
        }
        ret = HAL_I2CEx_ConfigAnalogFilter(_i2c, I2C_ANALOGFILTER_ENABLE);
        if (ret != HAL_OK)
        {
            Error_Handler();
        }
        ret = HAL_I2CEx_ConfigDigitalFilter(_i2c, 0);
        if (ret != HAL_OK)
        {
            Error_Handler();
        }
        HAL_I2C_EnableListen_IT(_i2c);
        
        return ret;
    }

    uint8_t slave_transmission(I2C_state state, uint8_t *p_buf, uint8_t len)
    {
        switch (state)
        {
        case I2C_state::getRegisterAddress:
            return HAL_I2C_Slave_Seq_Receive_IT(_i2c, p_buf, len, I2C_FIRST_FRAME);
            break;
        case I2C_state::sendData:
            return HAL_I2C_Slave_Seq_Transmit_IT(_i2c, p_buf, len, I2C_LAST_FRAME);
            break;
        case I2C_state::getData:
            return HAL_I2C_Slave_Seq_Receive_IT(_i2c, p_buf, len, I2C_LAST_FRAME);
            break;
        default:
            break;
        }
        return HAL_ERROR;
    }

    uint8_t slave_Addr_IRQ(uint8_t direct, KMUTNB_ADCS_REG *p_reg)
    {
        I2C_TICK_CALL = HAL_GetTick();
        _state = I2C_state::stop;
        switch (direct)
        {
        case I2C_DIRECTION_TRANSMIT:
            _state = I2C_state::getRegisterAddress;
            return slave_transmission(_state, &_read_reg_id, 1);
        case I2C_DIRECTION_RECEIVE:
            _state = I2C_state::sendData;
            return slave_transmission(_state, p_reg[_read_reg_id].get_header(), p_reg[_read_reg_id].get_len());
        default:
            break;
        }
        return HAL_ERROR;
    }
    uint8_t slave_Cplt_Addr(KMUTNB_ADCS_REG *p_reg)
    {

        switch (_state)
        {
        case I2C_state::getRegisterAddress:
            _state = I2C_state::getData;
            p_reg[_read_reg_id].set_update_flag(1);
            return slave_transmission(_state, p_reg[_read_reg_id].get_header(), p_reg[_read_reg_id].get_len());
        default:
            break;
        }
        return HAL_ERROR;
    }
};

#endif
