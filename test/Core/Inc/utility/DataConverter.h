/*
 * DataConverter.h
 *
 *  Created on: Apr 28, 2023
 *      Author: Carb0n
 */

#ifndef INC_DATACONVERTER_H_
#define INC_DATACONVERTER_H_

enum bit_len{

	_1bits,
	_2bits,
	_4bits,
	_8bits,
};


//template<class D>
//union Conv2Atype
//{
//	D DATA;
//	uint8_t bytes [sizeof(D)];
//};

template <class T>
T Get_value (uint8_t *offset_byte,int len){
	uint8_t *byteAdd = offset_byte;
	Conv2Atype<T> Converter;
	for( uint8_t i = 0; i < len ; i++){
		 Converter.bytes[i]= *byteAdd;
		 byteAdd += 1;
	}
	return Converter.DATA;
}

template <class V>
void Set_value (V Data, uint8_t *offset_byte){
	uint8_t *byteAdd = offset_byte;
	int len = sizeof(V);
	Conv2Atype<V> Converter;
	Converter.DATA = Data;
	for( int i = 0; i < len ; i++){
		 *byteAdd = Converter.bytes[i];
		 byteAdd += 1;
	}
}

template <class F>
F Decode_bitOperate(uint8_t *pt, bit_len len, uint8_t bit_pos){
	uint8_t rawData = *pt;
	uint8_t Bitlenght;
	if(len = _1bits){
		Bitlenght = 0x01;
	}else if(len = _2bits){
		Bitlenght = 0x03;
	}else if(len = _4bits){
		Bitlenght = 0x0F;
	}else if(len = _8bits){
		Bitlenght = 0xFF;
	}
	F ex_Data = (rawData>>bit_pos) & Bitlenght;
	return ex_Data;
}

//template <class N>
void Encode_bitOperate(uint8_t *pt, bit_len len, uint8_t bit_pos,int data){
	uint8_t Odata = *pt;
	uint8_t Bitlenght;
	if(len == _1bits){
		Bitlenght = 0x01;
	}else if(len == _2bits){
		Bitlenght = 0x03;
	}else if(len == _4bits){
		Bitlenght = 0x0F;
	}else if(len == _8bits){
		Bitlenght = 0xFF;
	}
	Odata = Odata & (~(Bitlenght<<bit_pos));
	*pt = (data << bit_pos) | Odata;
}




#endif /* INC_DATACONVERTER_H_ */
