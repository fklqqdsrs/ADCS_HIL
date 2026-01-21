#ifndef NEWBIT_REG_H_
#define NEWBIT_REG_H_

#include "KMUTNB_ADCS_REG.h"
enum bit_len_union_t{

	_mbit = 0b00,
	_1bit = 0b01,
	_2bit = 0b11,
	_4bit = 0b1111,
	_8bit = 0b11111111,
	_16bit,
	_32bit,
	_64bit,
	_0bit,
};

class NEW_BIT_REG
{
private:
    KMUTNB_ADCS_REG *_p_reg_main;
    uint8_t* _byte_pos;
    uint8_t _bit_len;
    uint8_t _data_type;
    char *_p_note;
    uint8_t _nByte;
    uint8_t _note_len;
    uint8_t _is_init;
    uint8_t _isupdated;
    uint8_t _byte_addr;
    uint8_t _bitPos_inByte;
public:
    uint8_t* get_byte_pos()
    {
        return _byte_pos;
    }

    uint8_t get_bit_len()
    {
        return _bit_len;
    }

    char *get_p_note()
    {
        return _p_note;
    }

    uint8_t get_note_len()
    {
        return _note_len;
    }
    uint8_t get_nByte()
    {
        return _nByte;
    }

    void init_bit(KMUTNB_ADCS_REG *p_reg_main,uint8_t byte_pos,  uint8_t bitPos_inByte, uint8_t bit_len, uint8_t nByte, char *p_note, uint8_t note_len)
    {
        _p_reg_main = p_reg_main;
        _byte_pos = _p_reg_main->get_header()+byte_pos;
        _bitPos_inByte = bitPos_inByte;
        _bit_len = bit_len;
        _nByte = nByte;
        _p_note = p_note;
        _note_len = note_len;
        _is_init = 1;
        set_update_flag(0);
    }


    void get_bits(uint8_t *output)
    {
        *output = (*_byte_pos >> _bitPos_inByte) & _bit_len;
    }

    void get_byte(uint8_t *output)
    {
	   	memcpy(output,_byte_pos,_nByte);
    }

    void get_data(uint8_t *output)
    {
    	if(_nByte == 1){
    		get_bits(output);
    	}else{
    		get_byte(output);
    	}
    }

    void set_bits(uint8_t bits)
    {
        *_byte_pos &= ~(_bit_len << _bitPos_inByte);
        *_byte_pos |= bits << _bitPos_inByte;
         
    }

    template<class T>
    void set_byte(T bytes[])
    {
		memcpy(_byte_pos,bytes,_nByte);
    }

    template<class T>
    void set_data(T data[])
	{
		if(_nByte == 1){
			set_bits(*data);
		}else{
			set_byte<T>(data);
		}
	}

    void update_data(uint8_t* data)
    	{
    		memcpy(_byte_pos,data,_nByte);
    	}

    KMUTNB_ADCS_REG *get_p_reg() { return _p_reg_main; }

    void set_p_reg(KMUTNB_ADCS_REG *header)
    {
        _p_reg_main = header;
    }

    uint8_t get_update_flag() { return _isupdated; }
    void set_update_flag(uint8_t flag) { _isupdated = flag; }
};

#endif /* INC_KMUTNB_ADCS_REG_H_ */
