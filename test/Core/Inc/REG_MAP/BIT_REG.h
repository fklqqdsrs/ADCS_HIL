#ifndef BIT_REG_H_
#define BIT_REG_H_

#include "KMUTNB_ADCS_REG.h"
#include "main.h"
//#include "NEWIO/UART/newuart.h"
//#include "NEWAPI/MAIN_UART_G4.h"

//extern NEWUART Serial1;

enum bits_len_union_t{

	_mbits_ = 0b00,
	_1bits_ = 0b01,
	_2bits_ = 0b11,
	_4bits_ = 0b1111,
	_7bits_ = 0b1111111,
	_8bits_ = 0b11111111,
	_16bits_,
	_32bits_,
	_64bits_,
	_80bits_,
	_128bits_,
	_160bits_,
	_0bits_,
};

class BIT_REG
{
private:
    KMUTNB_ADCS_REG *_p_reg_main;
    uint8_t _byte_pos;
    uint8_t _bit_len;
    uint8_t _data_type;
    char *_p_note;
    uint8_t _note_len;
    uint8_t _is_init;
    uint8_t _isupdated;
    uint8_t _byte_addr;
    uint8_t _bitPos_inByte;
public:
    uint8_t get_byte_pos()
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

    uint8_t get_data_type()
	{
		return _data_type;
	}

    void init_bit(KMUTNB_ADCS_REG *p_reg_main,uint8_t byte_pos,  uint8_t bitPos_inByte, uint8_t bit_len, uint8_t type, char *p_note, uint8_t note_len)
    {
        _p_reg_main = p_reg_main;
        _byte_pos = byte_pos;
        _bitPos_inByte = bitPos_inByte;
        _bit_len = bit_len;
        _data_type = type;
        _p_note = p_note;
        _note_len = note_len;
        _is_init = 1;
        set_update_flag(0);
    }


    void get_bits(uint8_t *output)
    {
        uint8_t *p_header;
        p_header = _p_reg_main->get_header()+_byte_pos;
        *output = (*p_header >> _bitPos_inByte) & _bit_len;
    }

    void get_byte(uint8_t *output)
    {
	    uint8_t *p_header;
	    uint8_t _len;
	    p_header = _p_reg_main->get_header()+_byte_pos;

	    switch (_data_type)
	    {
	    case _16UL:
	    case _16L:
	 	   _len = 2;
	 	   break;
	    case _32UL:
	    case _32L:
	    case _32F:
		   _len = 4;
		   break;
	    case _64UL:
	    case _64L:
		   _len = 8;
		   break;
	    }
	   	   memcpy(output,p_header,_len);
    }

    void get_data(uint8_t *output)
    {
    	if(_data_type == _8UL){
    		get_bits(output);
    	}else{
    		get_byte(output);
    	}
    }

    void set_bits(uint8_t bits)
    {
        uint8_t *p_header;

        p_header = _p_reg_main->get_header()+_byte_pos;
        *p_header &= ~(_bit_len << _bitPos_inByte);
        *p_header |= bits << _bitPos_inByte;
         
    }

    template<class T>
    void set_byte(T bytes)
    {
    	T *p_header;
    	uint8_t *p_u8header;

		p_u8header = ((_p_reg_main->get_header())+(_byte_pos));
		p_header = (T*)p_u8header;
		*p_header = bytes;
    }

    template<class T>
    void set_data(T data)
	{
		if(_data_type == _8UL){
			set_bits(data);
		}else{
			set_byte<T>(data);
		}
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
