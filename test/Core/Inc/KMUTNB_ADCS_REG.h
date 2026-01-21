/*
 * KMUTNB_ADCS_REG.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Carb0n
 */

#ifndef INC_KMUTNB_ADCS_REG_H_
#define INC_KMUTNB_ADCS_REG_H_
#include <cstring>

#define BigEndian 1
#define LittleEndian 0

//typedef struct{
//	uint8_t reg_ID;
//	int D_leng;
//}DATA_Register;
//
//DATA_Register ADCS_BOARD = {0x00, 1};
//DATA_Register TLM_COUNTER = {0x01, 1};
//DATA_Register MAIN_ERR_FLG = {0x02, 1};
//DATA_Register RW_ERR_FLG = {0x03, 1};
//DATA_Register MTQ_ERR_FLG = {0x04, 1};
//DATA_Register OPERATION_MODE = {0x05, 1};
//DATA_Register BUS_CURRENT = {0x06, 2};
enum data_type 
{
	_0UL, // none data
	_8UL,  // uint8_t
	_16UL, // uint16_t
	_16L,  // int16_t
	_24UL, //uint8_tx3
	_32UL, // uint32_t
	_32L,  // int32_t
	_32F,  // float
	_40UL, // uint8_tx5
	_48L, //int16_tx3
	_48UL,  // uint16_tx3
	_56UL, // uint8_tx7
	_64UL,  // uint16_tx4
	_64L, //double
	_80UL, //Multi data reg
	_96UL, //Big Multi data reg
	_96F,
	_104UL, // Big Plus Multi data reg
	_112UL, //Huge Multi data reg
	_128UL, //Huge Plus Multi data reg
	_160F, //Huge 5 float Multi data reg
	_160UL, //Huge Multi File Array data reg
	_168UL, //Huge Big Multi data reg
	_176UL, //Huge Big Plus Multi data reg
	_192F, //Huge 6 float Multi data reg
	_224F,
	_192D,
	_240UL, //Super Multi data reg
	_240L, //Sign Super Multi data reg
	_248UL, //Ultimate Multi data reg
	_264UL, //Mega Ultimate Multi data reg
	_288F,
	_384D,
	_512D, //Extreme Multi data reg
	_896UL, // Extreme Ultimate Multi data reg
	_1016UL, // Extreme Galaxy Multi data reg
	_1384UL, // Extreme Universe Multi data reg
	_256F, // float
	_ERR
};
template<class D>
union Conv2Atype
{
	D DATA;
	uint8_t bytes [sizeof(D)];
};
class KMUTNB_ADCS_REG
{
private:
	uint8_t *_header;
	uint8_t _len;
	uint8_t _type;
	uint8_t _is_init,_isupdated=0;

public:

	uint8_t get_init(){
		return _is_init;
	}

	void init(uint8_t *header, uint8_t len, uint8_t type)
	{
		_header = header;
		_len = len;
		_type = type;
		_is_init=1;
		set_update_flag(0);
	}
	template <class T>
	T get_value()
	{
		static Conv2Atype<T> Converter;
		memcpy(Converter.bytes, _header, sizeof(T));
		set_update_flag(0);
		return Converter.DATA;
	}

	template <class T>
	void set_value_t(T value)
	{
		static Conv2Atype<T> Converter;
		Converter.DATA = value;
		memcpy(_header, Converter.bytes, _len);
		set_update_flag(1);
	}

	void get_bytes(uint8_t *output)
	{
		memcpy(output, _header, _len);
		set_update_flag(0);
	}

	void set_bytes(uint8_t *bytes, uint8_t len,bool endian)
	{
		if (endian == BigEndian)
		{
			for (int i = 0; i < len; i++)
			{
				_header[i] = bytes[len-1-i] ;
			}
		}
		else
		{
				memcpy(_header, bytes, len);
				set_update_flag(1);
		}
	}

	void set_bytes_EXT(uint8_t *bytes,uint8_t len,uint8_t offset)
	{
		memcpy(_header+(7*offset), bytes, len);
		set_update_flag(1);
	}

	uint8_t *get_header() {  return _header;  }

	void set_header(uint8_t *header)
	{
		_header = header;
	}

	uint8_t get_len(){ return _len; }

	uint8_t get_type() { return _type; }

	uint8_t get_update_flag() { return _isupdated; }
	void set_update_flag(uint8_t flag) { _isupdated = flag;}


};



#endif /* INC_KMUTNB_ADCS_REG_H_ */
