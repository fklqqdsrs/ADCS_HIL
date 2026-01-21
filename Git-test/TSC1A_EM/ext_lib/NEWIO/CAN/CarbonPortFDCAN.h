#ifndef _CARBONPORTFDCAN_H_
#define _CARBONPORTFDCAN_H_
#include "main.h"
#include "../newPort.h"
#include "../newHeader.h"

enum class CAN_state
{
    stop,
    waiting,
    getData,
    sendData
};

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

class CarbonPortFDCAN : public newPort
{
private:
    FDCAN_HandleTypeDef *_can;
    CAN_state _state = CAN_state::stop;
    uint32_t _txMailbox;
    FDCAN_FilterTypeDef canfilterconfig;
    FDCAN_TxHeaderTypeDef *_tx_head;
    FDCAN_RxHeaderTypeDef *_rx_head;
    adcs_rwa_data_t _tx_buffer, _rx_buffer;
    uint8_t _self_id,_success;
    uint32_t _RxFifo;
    uint32_t *_idbuff;

public:
    CarbonPortFDCAN(FDCAN_HandleTypeDef *can, uint8_t self_id, uint32_t RxFifo = FDCAN_RX_FIFO0,uint8_t mode_t = AS_MASTER) : newPort(mode_t)
    {
        _can = can;
        _self_id = self_id;
        _RxFifo = RxFifo;
        _success = 0;
        _tx_head->IdType = FDCAN_EXTENDED_ID;
		_tx_head->TxFrameType = FDCAN_DATA_FRAME;
		_tx_head->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		_tx_head->BitRateSwitch = FDCAN_BRS_OFF;
		_tx_head->FDFormat = FDCAN_CLASSIC_CAN;
		_tx_head->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		_tx_head->MessageMarker = 0;
		_idbuff = &_tx_head->Identifier;

    }
    uint8_t init_rx_irq()
    {
		uint32_t FIFO_setting = (_RxFifo == FDCAN_RX_FIFO0) ? FDCAN_IT_RX_FIFO0_NEW_MESSAGE : FDCAN_IT_RX_FIFO1_NEW_MESSAGE;
		_state = CAN_state::waiting;
		return HAL_FDCAN_ActivateNotification(_can, FIFO_setting, 0);
    }
    void set_rxtx_head(FDCAN_RxHeaderTypeDef *rx_head, FDCAN_TxHeaderTypeDef *tx_head)
	{
		_rx_head = rx_head;
		_tx_head = tx_head;
	}

    FDCAN_RxHeaderTypeDef *get_rx_head()
	{
		return _rx_head;
	}

	uint8_t get_state()
	{
		return (uint8_t)_state;
	}

	uint8_t get_self_id()
	{
		return _self_id;
	}

	void set_self_id(uint8_t self_id)
	{
		_self_id = self_id;
	}

	uint8_t set_filter(uint32_t RxFifo = FDCAN_RX_FIFO0)
	{

		canfilterconfig.IdType = FDCAN_EXTENDED_ID;
		canfilterconfig.FilterIndex = 0;
		canfilterconfig.FilterType = FDCAN_FILTER_MASK;
		canfilterconfig.FilterConfig = (_RxFifo == FDCAN_RX_FIFO0) ? FDCAN_FILTER_TO_RXFIFO0 : FDCAN_FILTER_TO_RXFIFO1;
		canfilterconfig.FilterID1 = 0x0;
		canfilterconfig.FilterID2 = 0x0;
		return HAL_FDCAN_ConfigFilter(_can, &canfilterconfig);
	}

	FDCAN_HandleTypeDef *getHandleTypeDef() { return _can; }

	uint8_t writeBytes(FDCAN_TxHeaderTypeDef *tx_head, uint8_t data[], uint32_t *d_Mailbox)
	{
		_state = CAN_state::stop;
		while(HAL_FDCAN_GetTxFifoFreeLevel(_can) <3) {}

		_state = CAN_state::sendData;
		return HAL_FDCAN_AddMessageToTxFifoQ(_can, tx_head, data);
	}

	uint8_t readBytes(FDCAN_RxHeaderTypeDef *rx_head, uint8_t *buffer)
	{
		_state = CAN_state::getData;
		return HAL_FDCAN_GetRxMessage(_can, _RxFifo, rx_head, buffer);
	}

	uint8_t CAN_IRQ_HANDLE(FDCAN_HandleTypeDef *hcan, uint8_t *buffer, uint32_t RxFifo)
	{
		uint8_t res = 0;
		if (hcan != _can || RxFifo != _RxFifo)
		{
			_state = CAN_state::stop;
			return 5;
		}
		res = readBytes(_rx_head, buffer);
		return res;
	}

	uint8_t available()
	{
		return HAL_FDCAN_GetRxFifoFillLevel(_can, _RxFifo);
	}

	template <typename T>
	uint32_t *get_extID(T *header)
	{
		return &header->Identifier;
	}

	adcs_rwa_data_t *get_rx_buffer()
	{
		return &_rx_buffer;
	}

	void decode_id(uint32_t can_id)
	{
		_rx_buffer.msgtype = (msgtypes)(can_id >> 24) & 0x1F; // First 5 bits
		_rx_buffer.reg_id = (can_id >> 16) & 0xFF;			  // Next 8 bits
		_rx_buffer.src_addr = (can_id >> 8) & 0xFF;			  // Next 8 bits
		_rx_buffer.dest_addr = can_id & 0xFF;				  // Last 8 bits
	}

	void decode_dlc(FDCAN_RxHeaderTypeDef *rx_head)
	{
		_rx_buffer.len = rx_head->DataLength >> 16;
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
		_tx_head->DataLength = 0x10000 * ((len > 8) ? 8 : len);
	}

	uint8_t get_dlc()
	{
		return _rx_buffer.len;
	}

	// send ack or nack response
	uint8_t send_response(uint8_t dest_addr, uint8_t reg_id, uint8_t ack_type)
	{
		*_idbuff = encode_id(ack_type, reg_id, _self_id, dest_addr);
		set_dlc(0);
		return this->writeBytes(_tx_head, _tx_buffer.header, &_txMailbox);
	}

	// Process a received packet
	uint8_t processPckt(FDCAN_RxHeaderTypeDef header, uint8_t data[])
	{
		decode_id(*get_extID<FDCAN_RxHeaderTypeDef>(&header)); /*decode 29 bit id*/
		decode_dlc(&header);
		if (_rx_buffer.dest_addr != _self_id)
		{
			_success = 0;
			return 6;
		}

		return 0;
	}

	uint8_t request_data(uint8_t dest_addr, uint8_t reg_id)
	{

		*_idbuff = encode_id(TLM_req, reg_id, _self_id, dest_addr);
		set_dlc(0);
		_success = 1;

		return this->writeBytes(_tx_head, _tx_buffer.header, &_txMailbox);
	}

	uint8_t send_buffer(adcs_rwa_data_t *pbuf)
	{
		*_idbuff = encode_id(pbuf->msgtype, pbuf->reg_id, pbuf->src_addr, pbuf->dest_addr);
		encode_package(pbuf->header, pbuf->len);
		set_dlc(pbuf->len);
		return this->writeBytes(_tx_head, _tx_buffer.header, &_txMailbox);
	}

	uint8_t send_data_bytes(uint8_t dest_addr, uint8_t reg_id, uint8_t *data, uint8_t len)
	{
		*_idbuff = encode_id(TC_req, reg_id, _self_id, dest_addr);
		encode_package(data, len);
		set_dlc(len);
		return this->writeBytes(_tx_head, _tx_buffer.header, &_txMailbox);
	}

	void send_EXT_data_bytes(uint8_t dest_addr, uint8_t reg_id, uint8_t *data, uint8_t len)
	{
			*_idbuff = encode_id(TC_EXT, reg_id, _self_id, dest_addr);
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
				this->writeBytes(_tx_head, _tx_buffer.header, &_txMailbox);
				HAL_Delay(1);
			}

	}

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

};
#endif
