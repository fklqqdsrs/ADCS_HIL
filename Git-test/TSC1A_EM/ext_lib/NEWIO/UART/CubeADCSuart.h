#ifndef _CUBEADCSUART_H_
#define _CUBEADCSUART_H_

#include "main.h"
#include "NEWIO/newDevice.h"
//#include "NEWIO/newPort.h"
//#include "NEWIO/I2C/newPortI2C.h"
//#include "NEWIO/UART/newuart.h"
#include "REG_MAP/CUBE_REG.h"
#include "REG_MAP/KMUTNB_ADCS_REG.h"
//#include "utility/DataConverter.h"
#define BUF_LEN 100

typedef struct {

	//uint8_t addr;
	uint8_t reg_id;
	uint8_t header[BUF_LEN];
	uint8_t len;
} cube_adcs_uart_data_t;

enum TC_error_flag{
	TC_no_error = 0x00,
	TC_invalid_id,
	TC_incorrect_len,
	TC_invalid_para,
	TC_CRC_error
};

class CubeADCSUART {

private:

	KMUTNB_ADCS_REG *_reg;
	uint8_t _tx_buf[BUF_LEN];
	uint8_t _tx_len = 0;
	cube_adcs_uart_data_t _data;
	uint8_t teletype = 1;
	uint8_t error = 0;
	uint8_t success = 0;

public:

	CubeADCSUART(KMUTNB_ADCS_REG *reg) :
			_reg(reg) {
	}

	uint8_t* get_tx_h() {
		return _tx_buf;
	}
	uint8_t get_txlen() {
		return _tx_len;
	}

	uint8_t get_success() {
		return success;
	}

	uint8_t get_teletype(){
		return teletype;
	}

	void set_error(uint8_t error_type){
		error = error_type;
	}

	uint8_t get_error(){
		return error;
	}

	uint8_t check_package_tlm(uint8_t *buf, uint8_t len) {
		if (len < 5)
			return 1; // incorrect data len
		uint8_t iden1 = buf[0];
		uint8_t header = buf[1];
		uint8_t reg_id = buf[2];
		uint8_t iden2 = buf[len - 2];
		uint8_t ending = buf[len - 1];

		if (iden1 != 0x1F  && header != 0x7F  && iden2 != 0x1F && ending != 0xFF) {
			return 2; // incorrect format
		} else if (_reg[reg_id].get_init() != 1) {
			return 3;// invalid ID
		} else if (len - 5 != _reg[reg_id].get_len()){
			return 4; //incorrect lenght
		}
		return 0;
	}

	uint8_t check_package_tc(uint8_t *buf, uint8_t len) {
		if (len < 5)
			return 1; // incorrect data len
		uint8_t iden1 = buf[0];
		uint8_t header = buf[1];
		uint8_t reg_id = buf[2];
		uint8_t TC_Error_flag = buf[3];
		uint8_t iden2 = buf[len - 2];
		uint8_t ending = buf[len - 1];

		if (iden1 != 0x1F  && header != 0x7F  && iden2 != 0x1F && ending != 0xFF) {
			return 2; // incorrect format
		} else if (_reg[reg_id].get_init() != 1) {
			return 3;// invalid ID
		}
		return TC_Error_flag;
	}

	uint8_t decode_package(uint8_t *buf_in, uint8_t len) {

		uint8_t is_format = check_package_tlm(buf_in, len);
		if (is_format != 0)
			return is_format; // fail

		_data.reg_id = buf_in[2];
		_data.len = len - 5;
		memcpy(_data.header, &buf_in[3], _data.len);

		return 0; // success
	}
	uint8_t encode_package_tlm(uint8_t reg_id){
		//uint8_t len = _reg[reg_id].get_len();
		_tx_buf[0] = 0x1F;
		_tx_buf[1] = 0x7F;
		_tx_buf[2] = reg_id;
		_tx_buf[3] = 0x1F;
		_tx_buf[4] = 0xFF;
		_tx_len = 5;
		teletype = 1;
		return 0;
	}

	uint8_t encode_package_tc(uint8_t reg_id){
		uint8_t len = _reg[reg_id].get_len();
		_tx_buf[0] = 0x1F;
		_tx_buf[1] = 0x7F;
		_tx_buf[2] = reg_id;
		memcpy(&_tx_buf[3], _reg[reg_id].get_header(), len);
		_tx_buf[len + 3] = 0x1F;
		_tx_buf[len + 4] = 0xFF;
		_tx_len = 5 + len;
		teletype = 2;
		return 0;
	}

	uint8_t reading_process_tlm(uint8_t *buf, uint8_t len) {
		uint8_t is_fail = decode_package(buf, len);
		if (is_fail != 0) {
			return is_fail; // fail
		}
		_reg[_data.reg_id].set_bytes(_data.header, _data.len);
		return 0;
	}


};

#endif
