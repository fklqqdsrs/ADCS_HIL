#ifndef _newPortI2C_h_
#define _newPortI2C_h_

#include "../newPort.h"
#include "REG_MAP/REG_ADCS_MAIN/ADCS_MAIN_REG.h"
#include "NEWIO/newHeader.h"

int i2ce = 0 ;

enum I2C_state
{
    stop,
    waiting,
    getRegisterAddress,
    getData,
    sendData
};

//enum new_Mode_type
//{
//    AS_SLAVE,
//    AS_MASTER
//};

class newPortI2C : public newPort
{
private:
    I2C_HandleTypeDef *_i2c;
    I2C_state _state = I2C_state::stop;
    uint8_t _read_reg_id = 0;
    KMUTNB_ADCS_REG *_reg;

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
        default: break;
        }
        return HAL_ERROR;
    }

    uint8_t slave_Addr_IRQ(uint8_t direct, KMUTNB_ADCS_REG *p_reg)
    {
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
            return slave_transmission(_state, p_reg[_read_reg_id].get_header(), p_reg[_read_reg_id].get_len());
        default:
            break;
        }
        return HAL_ERROR;
    }
};

#endif
