/*
 * RWCtrl.h
 *
 *  Created on: May 30, 2023
 *      Author: Carb0n
 */

#ifndef INC_STM32NEWFNC_RWCTRL_H_
#define INC_STM32NEWFNC_RWCTRL_H_
#include "string.h"
#include <stdio.h>
#include "DataConverter.h"
#include "main.h"

#define RW_address 0x01
// Motor control commands
#define cmd_writeReg 0x02
#define cmd_readReg 0x03
#define cmd_startMotor 0x04
#define cmd_stopMotor 0x05
// response commands
#define res_readReg 0x11
// Data registers address
#define reg_status 0x21
#define reg_busVoltage 0x22
#define reg_temp 0x23
#define reg_mPower 0x24
#define reg_speed 0x25
#define reg_faults 0x26
#define reg_speedRamp 0x27

//#define NEW_EDIT_RWA

#ifndef NEW_EDIT_RWA
#include "RingBufferUART_IT.h"
RingBufferUART_IT Main2Wheel(&huart2);

class RWCtrl
{
private:
	UART_HandleTypeDef *_uart;
	uint8_t _error = 1;
public:
	// reg values
	uint8_t status;
	uint16_t busVoltage;
	uint16_t temp;
	uint16_t mPower;
	int speed_set = 0;
	int16_t speed_mes = 0;
	uint32_t faults;
	uint16_t speedRamp;
	int16_t data;
	uint8_t cmd;
	uint8_t reg;
	int reg_value;
	uint8_t dataSize;
	

public:
	RWCtrl(UART_HandleTypeDef *uart)
	{

		_uart = uart;

		// HAL_GPIO_WritePin(GPIOC, RW_ENA1_Pin,GPIO_PIN_SET);
	}

	void ReceiveData(uint8_t *Data, int byte)
	{
		uint8_t *buff = Data;
		for (int i = 0; i < byte; i++)
		{
			*buff = Main2Wheel.read();
			buff++;
		}
	}

	void WriteReg(uint8_t Reg, int16_t Data)
	{

		uint8_t Pack[6];
		Pack[0] = RW_address;
		Pack[1] = cmd_writeReg;
		Pack[2] = Reg;
		Set_value<int16_t>(Data, &Pack[3]);
		Pack[5] = 0x0A;
		HAL_UART_Transmit(_uart, Pack, 6, 100);
	}

	void Receive6byteRespon()
	{
		for (int i; i < 6; i++)
		{
			Main2Wheel.read();
		}
	}

	void ReadReg(uint8_t Reg, int16_t *Data)
	{
		uint8_t T_Pack[4];
		uint8_t R_Pack[6];
		memset(R_Pack, 0, 6);
		// Main2Wheel.flush();
		T_Pack[0] = RW_address;
		T_Pack[1] = cmd_readReg;
		T_Pack[2] = Reg;
		T_Pack[3] = 0x0A;
		while (!((R_Pack[0] == 0x01) & (R_Pack[1] == 0x11) & (R_Pack[2] == Reg) & (R_Pack[5] == 0x0A)))
		{
			HAL_UART_Transmit(_uart, T_Pack, 4, 100);
			// if( __HAL_UART_GET_FLAG(_uart, UART_FLAG_RXNE) ){
			HAL_Delay(1);
			// HAL_UART_Receive(_uart,R_Pack, 6, 100);
			ReceiveData(&R_Pack[0], 6);
		}

		//}
		// HAL_UART_Receive(_uart,R_Pack, 6, 100);
		*Data = Get_value<int16_t>(&R_Pack[3], 2);
	}

	void Spin()
	{

		uint8_t Pack[4] = {RW_address, cmd_startMotor, 0, 0x0a};
		HAL_UART_Transmit(_uart, Pack, 4, 100);
	}

	void Stop()
	{

		uint8_t Pack[4] = {RW_address, cmd_stopMotor, 0, 0x0a};
		HAL_UART_Transmit(_uart, Pack, 4, 100);
	}
	void EnableWheel()
	{
		HAL_GPIO_WritePin(GPIOC, RW_ENA1_Pin, GPIO_PIN_SET);
	}

	void DisableWheel()
	{
		HAL_GPIO_WritePin(GPIOC, RW_ENA1_Pin, GPIO_PIN_RESET);
	}

	void flush()
	{
		Main2Wheel.flush();
	}
	void get_error() { return _error;}
};
#else

#define RW_POLLING_MODE
#include "stm32newfnc\newuart.h"
#include "ADCS_REG\KMUTNB_ADCS_REG.h"

enum rw_reg
{
	RW_ID = 0x1,
	RW_STATUS = 0x21,
	RW_BUS_V = 0x22,
	RW_TEMP_C = 0x23,
	RW_POWER_W = 0x24,
	RW_SPEED_RPM = 0x25,
	RW_FAULT = 0x26,
	RW_SPEED_DEV = 0x27,
	RW_NOW_RPM = 0x28,

};
enum rw_cmd
{
	RW_WRITE = cmd_writeReg,
	RW_READ = cmd_readReg,
	RW_START = cmd_startMotor,
	RW_STOP = cmd_stopMotor,
	RW_RESPONSE = res_readReg,
};

NEWUART Main2Wheel(&huart2, 1);
enum RW_state
{
	standby,
	requestData,
	gotData,
	sendingData,
	run,
};
#define rx_size 20
class RWCtrl
{
private:
	NEWUART *_uart;
	GPIO_TypeDef *_en_port;
	uint16_t _en_pin;
	uint8_t _is_enabled = 0;
	RW_state _state = standby;
	uint8_t _addr = 0;
	uint8_t _error =1;
	uint8_t rx_buf[rx_size];
	uint8_t tx_buf[rx_size];

	uint8_t RW_ID_D = 1;
	uint8_t RW_STATUS_D = 0;
	uint16_t RW_BUS_V_D = 0;
	uint16_t RW_TEMP_C_D = 0;
	uint16_t RW_POWER_W_D = 0;
	int16_t RW_SPEED_RPM_D = 0;
	uint32_t RW_FAULT_D = 0;
	uint16_t RW_SPEED_DEV_D = 0;
	int16_t RW_NOW_RPM_D = 0;
	KMUTNB_ADCS_REG rw_reg[50];

public:
	RWCtrl(NEWUART *uart, uint8_t addr, GPIO_TypeDef *en_port, uint16_t en_pin)
	{
		_uart = uart;
		_en_port = en_port;
		_en_pin = en_pin;
		_addr = addr;
		_error = 1;
		DisableWheel();
	}

	void setup_reg()
	{
		rw_reg[RW_ID].init((uint8_t *)&RW_ID_D, sizeof(RW_ID_D), _8UL);
		rw_reg[RW_STATUS].init((uint8_t *)&RW_STATUS_D, sizeof(RW_STATUS_D), _8UL);
		rw_reg[RW_BUS_V].init((uint8_t *)&RW_BUS_V_D, sizeof(RW_BUS_V_D), _16UL);
		rw_reg[RW_TEMP_C].init((uint8_t *)&RW_TEMP_C_D, sizeof(RW_TEMP_C_D), _16UL);
		rw_reg[RW_POWER_W].init((uint8_t *)&RW_POWER_W_D, sizeof(RW_POWER_W_D), _16UL);
		rw_reg[RW_SPEED_RPM].init((uint8_t *)&RW_SPEED_RPM_D, sizeof(RW_SPEED_RPM_D), _16L);
		rw_reg[RW_FAULT].init((uint8_t *)&RW_FAULT_D, sizeof(RW_FAULT_D), _32UL);
		rw_reg[RW_SPEED_DEV].init((uint8_t *)&RW_SPEED_DEV_D, sizeof(RW_SPEED_DEV_D), _16UL);
		rw_reg[RW_NOW_RPM].init((uint8_t *)&RW_NOW_RPM_D, sizeof(RW_NOW_RPM_D), _16L);
	}

	uint8_t check_header(uint8_t *buf)
	{
		return (buf[0] == RW_ID_D || buf[rx_buf[2]].get_init() == 1);
	}
	uint8_t check_cmd(uint8_t *buf)
	{
		return (buf[1] == rw_cmd::RW_RESPONSE);
	}
	uint8_t check_newline(uint8_t *buf)
	{
		return (rw_reg[buf[2]].get_len() + 3 == 0x0A);
	}

	void get_error() { return _error;}

	void EnableWheel()
	{
		if (_is_enabled != 0)
			return;
		HAL_GPIO_WritePin(_en_port, _en_pin, GPIO_PIN_SET);
		_is_enabled = 1;
	}

	void DisableWheel()
	{
		if (_is_enabled == 0)
			return;
		HAL_GPIO_WritePin(_en_port, _en_pin, GPIO_PIN_RESET);
		_is_enabled = 0;
	}
	void transmission(uint8_t reg_id, RW_state state = RW_state::run)
	{
		static uint8_t is_polling = 0;
#ifdef RW_POLLING_MODE
		is_polling = 1;
#endif
		if (_is_enabled != 0)
			return;
		if (rw_reg[reg_id].get_init() != 1)
			return;

		switch (_state)
		{
		case standby:
			break;
		case requestData:
			static uint8_t ss = HAL_OK;
			ss = ReadReg(reg_id, rw_reg, is_polling);
			if (ss == HAL_OK && is_polling == 1)
			{
				_state = gotData;
			}
			break;
		case gotData:
			// check header
			if (check_header(rx_buf) != 0)
			{
				_error=1;
				_state = requestData;
				break;
			}
			// check command
			if (check_cmd(rx_buf) != 0)
			{	
				_error=1;
				_state = requestData;
				break;
			}
			// check endline
			if (check_newline(rx_buf) != 0)
			{
				_error=1;
				_state = requestData;
				break;
			}
			decode(rx_buf);

			break;
		case sendingData:
			break;
		default:
			break;
		}
	}
	void decode(unsigned char *buf)
	{
		uint8_t is_response = 0;
		switch (buf[1])
		{
		case RW_RESPONSE:
			// decode
			is_response = 1;
			_error = 0;
			break;
		case RW_WRITE:
		case RW_READ:
		case RW_START:
		case RW_STOP:
		default:
			break;
		}
		if (is_response != 1)
		{
			_state = requestData;
			return;
		}

		// remaining code is actually decoding section
		KMUTNB_ADCS_REG converter;
		converter.init(&buf[3], rw_reg[buf[2]].get_len(), rw_reg[buf[2]].get_type());
		switch (rw_reg[buf[2]].get_type())
		{
		case _8UL:
			converter.get_value<uint8_t>();
			break;
		case _16L:
			converter.get_value<int16_t>();
			break;
		case _16UL:
			converter.get_value<uint16_t>();
			break;
		case _32L:
			converter.get_value<int32_t>();
			break;
		case _32UL:
			converter.get_value<uint32_t>();
			break;
		case _32F:
			converter.get_value<float>();
			break;
		default:
			break;
		}
	}

	void sendPacket(uint8_t cmd_code, uint8_t reg_id,
					uint8_t *pbuf, uint8_t len)
	{
		tx_buf[0] = _addr;
		tx_buf[1] = cmd_code;
		tx_buf[2] = reg_id;
		memcpy(&tx_buf[3], pbuf, len);
		tx_buf[3 + len] = 0x0A;
		_uart->writeBytes(tx_buf, 4 + len, 0);
	}

	void WriteReg(uint8_t Reg, KMUTNB_ADCS_REG *reg_data)
	{
		sendPacket(rw_cmd::RW_WRITE, Reg, reg_data->get_header(), reg_data->len());
	}
	uint8_t ReadReg(uint8_t Reg, KMUTNB_ADCS_REG *reg_data, uint8_t is_polling = 1)
	{
		sendPacket(rw_cmd::RW_READ, Reg, 0, 0);
		memset(rx_buf, 0, rx_size);
		if (is_polling == 1)
		{
			return _uart->readBytes(rx_buf, reg_data->len() + 3 + 1, 100);
		}
		else
		{
			return HAL_UARTEx_ReceiveToIdle_DMA(_uart->getHandleTypeDef(), rx_buf, rx_size);
		}
	}
	void Spin()
	{
		uint8_t run_pack[4] = {_addr, rw_cmd::RW_START, 0, 0x0a};
		_uart->writeBytes(run_pack, 4, 0);
	}

	void Stop()
	{
		uint8_t stop_pack[4] = {_addr, rw_cmd::RW_STOP, 0, 0x0a};
		_uart->writeBytes(stop_pack, 4, 0);
	}
};

#endif

#endif /* INC_STM32NEWFNC_RWCTRL_H_ */
