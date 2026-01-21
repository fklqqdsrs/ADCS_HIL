/*
 * kmCAN.h
 *
 *  Created on: Jun 15, 2023
 *      Author: INSTED
 */

#ifndef INC_KMCAN_H_
#define INC_KMCAN_H_

#include "main.h"
#include <string.h>

#ifdef NEWFDCAN
#include "newPortFDCAN.h"
#define _canport_ newPortFDCAN
#define _TxHeadType_ FDCAN_TxHeaderTypeDef
#define _RxHeadType_ FDCAN_RxHeaderTypeDef
#else
#include "newPortCAN.h"
#define _canport_ newPortCAN
#define _TxHeadType_ CAN_TxHeaderTypeDef
#define _RxHeadType_ CAN_RxHeaderTypeDef
#endif

/*message types enum*/
typedef enum
{
	TC_req = 0x01,
	TC_ack,
	TC_nack,
	TLM_req,
	TLM_res,
	TLM_nack,
	TC_EXT,
	TLM_EXT
} msgtypes;

typedef struct
{
	uint8_t msgtype;
	uint8_t src_addr;
	uint8_t dest_addr;
	uint8_t reg_id;
	uint8_t header[8];
	uint8_t len;
} adcs_rwa_data_t;
class kmCAN
{
private:
	_canport_ *_can;
	_TxHeadType_ _tx_head;
	_RxHeadType_ _rx_head;
	uint32_t _txMailbox;
	adcs_rwa_data_t _tx_buffer, _rx_buffer;
	uint8_t _self_addr, _success;
	uint32_t _RxFifo;
	uint32_t *_idbuff;

public:
	kmCAN(_canport_ *newcan, uint8_t self_id)
	{
		_success = 0;
		_can = newcan;
		_self_addr = self_id;

#ifdef NEWFDCAN
		_tx_head.IdType = FDCAN_EXTENDED_ID;
		_tx_head.TxFrameType = FDCAN_DATA_FRAME;
		_tx_head.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		_tx_head.BitRateSwitch = FDCAN_BRS_OFF;
		_tx_head.FDFormat = FDCAN_CLASSIC_CAN;
		_tx_head.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		_tx_head.MessageMarker = 0;
		_idbuff = &_tx_head.Identifier;

#else
		_tx_head.IDE = CAN_ID_EXT; // 29 bits ID
		_tx_head.RTR = CAN_RTR_DATA;
		_tx_head.TransmitGlobalTime = DISABLE;
		_idbuff = &_tx_head.ExtId;
#endif
	}
	void set_new_id(uint8_t self_id){
		_self_addr = self_id;
	}

	uint8_t get_id(){
		return _self_addr;
	}
	
	uint32_t *get_extID(_RxHeadType_ *header)
	{
#ifdef NEWFDCAN
		return &header->Identifier;
#else
		return &header->ExtId;
#endif
	}

	_RxHeadType_ *get_rx_head()
	{
		return &_rx_head;
	}
	adcs_rwa_data_t *get_rx_buffer()
	{
		return &_rx_buffer;
	}

	_canport_ *get_CAN_port()
	{
		return _can;
	}

	// Decode the CAN message ID
	void decode_id(uint32_t can_id)
	{
		_rx_buffer.msgtype = (msgtypes)(can_id >> 24) & 0x1F; // First 5 bits
		_rx_buffer.reg_id = (can_id >> 16) & 0xFF;			  // Next 8 bits
		_rx_buffer.src_addr = (can_id >> 8) & 0xFF;			  // Next 8 bits
		_rx_buffer.dest_addr = can_id & 0xFF;				  // Last 8 bits
	}

	void decode_dlc(_RxHeadType_ *rx_head)
	{
#ifdef NEWFDCAN
		_rx_buffer.len = rx_head->DataLength >> 16;

#else
		_rx_buffer.len = rx_head->DLC;
#endif
	}

	// Encode the CAN message ID
	uint32_t encode_id(uint8_t msg_type, uint8_t reg_id, uint8_t src_addr,
					   uint8_t dest_addr)
	{
		uint32_t can_id = 0;
		can_id |= (msg_type & 0x1F) << 24; // First 5 bits
		can_id |= (reg_id & 0xFF) << 16;   // Next 8 bits
		can_id |= (src_addr & 0xFF) << 8;  // Next 8 bits
		can_id |= dest_addr & 0xFF;		   // Last 8 bits
		return can_id;
	}

	void set_dlc(uint8_t len)
	{
#ifdef NEWFDCAN
		_tx_head.DataLength = 0x10000 * ((len > 8) ? 8 : len);

#else
		_tx_head.DLC = (len > 8) ? 8 : len;
#endif
	}

	uint8_t get_dlc()
	{
		return _rx_buffer.len;
	}

	// send ack or nack response
	uint8_t send_response(uint8_t dest_addr, uint8_t reg_id, uint8_t ack_type)
	{
		*_idbuff = encode_id(ack_type, reg_id, _self_addr, dest_addr);
		set_dlc(0);
		return _can->writeBytes(&_tx_head, _tx_buffer.header, &_txMailbox);
	}

	// Process a received packet
	uint8_t processPckt(_RxHeadType_ header, uint8_t data[])
	{
		decode_id(*get_extID(&header)); /*decode 29 bit id*/
		decode_dlc(&header);
		if (_rx_buffer.dest_addr != _self_addr)
		{
			_success = 0;
			return 6;
		}
		_success =1;
		return 0;
	}

	uint8_t request_data(uint8_t dest_addr, uint8_t reg_id)
	{

		*_idbuff = encode_id(TLM_req, reg_id, _self_addr, dest_addr);
		set_dlc(0);
		//_success = 1;

		return _can->writeBytes(&_tx_head, _tx_buffer.header, &_txMailbox);
	}

	uint8_t send_buffer(adcs_rwa_data_t *pbuf)
	{
		*_idbuff = encode_id(pbuf->msgtype, pbuf->reg_id, pbuf->src_addr, pbuf->dest_addr);
		encode_package(pbuf->header, pbuf->len);
		set_dlc(pbuf->len);
		return _can->writeBytes(&_tx_head, _tx_buffer.header, &_txMailbox);
	}

	uint8_t send_data_bytes(uint8_t dest_addr, uint8_t reg_id, uint8_t *data, uint8_t len)
	{
		*_idbuff = encode_id(TC_req, reg_id, _self_addr, dest_addr);
		encode_package(data, len);
		set_dlc(len);
		return _can->writeBytes(&_tx_head, _tx_buffer.header, &_txMailbox);
	}

void send_EXT_data_bytes(uint8_t dest_addr, uint8_t reg_id, uint8_t *data, uint8_t len)
	{
			*_idbuff = encode_id(TC_EXT, reg_id, _self_addr, dest_addr);
			uint8_t ext_r = len/7;
			uint8_t mod = len % 7;
			if(mod >= 1){
				ext_r += 1;
			}
			for(int i = 0; i < ext_r; i++ ){
				if(i == ext_r-1){
					encode_EXT_package(data+(i*7), mod, (ext_r-1-i));
					set_dlc(mod+1);
				}else{
					encode_EXT_package(data+(i*7), 7, (ext_r-1-i));
					set_dlc(8);
				}
				_can->writeBytes(&_tx_head, _tx_buffer.header, &_txMailbox);
				HAL_Delay(1);
			}

	}
	//	uint8_t CAN_IRQ_HANDLE(CAN_HandleTypeDef *hcan, uint32_t RxFifo)
	//	{
	//		_success = 0;
	//		if (hcan != _can)
	//		{
	//			return 5;
	//		}
	//		uint8_t res = _can->readBytes(&_rx_head, _rx_buffer.header);
	//		if (res != 0)
	//		{
	//			return res;
	//		}
	//		_success = 1;
	//		return 0;
	//	}

	uint8_t encode_package(uint8_t *p_data, uint8_t len = 0)
	{

		memcpy(_tx_buffer.header, p_data, len);
		return 0;
	}

	uint8_t encode_EXT_package(uint8_t *p_data, uint8_t len,uint8_t p_left )
	{
			if(p_left == 0){
				memcpy(_tx_buffer.header, p_data, len);
				for(int i = len; i < 8; i++){
					_tx_buffer.header[i] = 0;
				}
			}else{
				memcpy(_tx_buffer.header, p_data, 7);
				_tx_buffer.header[7] = p_left;
			}
			return 0;
	}

	uint8_t decode_package(uint8_t *buf_in, uint8_t len)
	{
		return 0;
	}

	uint32_t measure_ping_ms(uint8_t dest_addr, uint8_t reg_id,uint32_t timeout_ms =1000){
		uint32_t prev_ms = 0,dif_ms =0;
		if( request_data(dest_addr,reg_id) != HAL_OK)
		{
			return timeout_ms;
		} 
		prev_ms = HAL_GetTick();
		do 
		{
			dif_ms = prev_ms - HAL_GetTick();
			if(dif_ms > timeout_ms){
				break;
			}
			if (_rx_buffer.msgtype == TLM_res && _success ==1)
			{
				break;
			}
			
		}while (true);
		return dif_ms;
		
	}



};

#endif /* INC_KMCAN_H_ */
