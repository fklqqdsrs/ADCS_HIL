#ifndef _newPortCAN_h_
#define _newPortCAN_h_

/*
for can fd buadrate https://phryniszak.github.io/stm32g-fdcan/
data bit is for using comunicate other fdcan device
nominal bit is can 2.0b device
*/

#include "NEWIO/newHeader.h"

enum class CAN_state
{
    stop,
    waiting,
    getData,
    sendData
};

class newPortCAN : public newPort
{
private:
    CAN_HandleTypeDef *_can;
    CAN_state _state = CAN_state::stop;
    CAN_FilterTypeDef canfilterconfig;
    CAN_TxHeaderTypeDef *_tx_head;
    CAN_RxHeaderTypeDef *_rx_head;
    uint32_t _RxFifo;
    uint8_t _self_id;

public:
    newPortCAN(CAN_HandleTypeDef *can, uint8_t self_id, uint32_t RxFifo = CAN_RX_FIFO0) : newPort(AS_MASTER)
    {
        _can = can;
        _RxFifo = RxFifo;
        _self_id = self_id;
    }
    uint8_t init_rx_irq(){
    	uint32_t FIFO_setting = (_RxFifo == CAN_RX_FIFO0)? CAN_IT_RX_FIFO0_MSG_PENDING:CAN_IT_RX_FIFO1_MSG_PENDING;
    	_state = CAN_state::waiting;
    	return HAL_CAN_ActivateNotification(_can,FIFO_setting);
    }
    
    void set_rxtx_head(CAN_RxHeaderTypeDef *rx_head,CAN_TxHeaderTypeDef *tx_head) {
        _rx_head = rx_head;
        _tx_head = tx_head;
    }

    CAN_RxHeaderTypeDef* get_rx_head(){
        return _rx_head;
    }

    uint8_t get_state(){
        return (uint8_t)_state;
    }

    uint8_t get_self_id(){
        return _self_id;
    }
    void set_self_id(uint8_t self_id){
        _self_id = self_id;
    }

    uint8_t set_filter(uint32_t RxFifo = CAN_RX_FIFO1)
    {

        if (RxFifo != CAN_RX_FIFO0)
        {
            canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
            canfilterconfig.FilterBank = 14;           // at least 14
            canfilterconfig.SlaveStartFilterBank = 14; // how many filters to assign to the CAN1 (master can)
        }
        else
        {
            canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
            canfilterconfig.SlaveStartFilterBank = 0; // how many filters to assign to the CAN1 (master can)
            canfilterconfig.FilterBank = 0;           // which filter bank to use from the assigned ones
        }
        canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
        canfilterconfig.FilterIdHigh = 0; // will just receive 0x103 address mssg
        canfilterconfig.FilterIdLow = 0;
        canfilterconfig.FilterMaskIdHigh = 0;
        canfilterconfig.FilterMaskIdLow = 0;
        canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
        canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
        
        return HAL_CAN_ConfigFilter(_can, &canfilterconfig);

    }

    CAN_HandleTypeDef *getHandleTypeDef() { return _can; }

    uint8_t writeBytes(CAN_TxHeaderTypeDef *tx_head, uint8_t data[], uint32_t *txMailbox)
    {
    	_state = CAN_state::sendData;
        return HAL_CAN_AddTxMessage(_can, tx_head, data, txMailbox);
    }

    uint8_t readBytes(CAN_RxHeaderTypeDef *rx_head, uint8_t *buffer)
    {
    	_state = CAN_state::getData;
        return HAL_CAN_GetRxMessage(_can,_RxFifo, rx_head, buffer);
    }

    uint8_t CAN_IRQ_HANDLE(CAN_HandleTypeDef *hcan,uint8_t *buffer ,uint32_t RxFifo)
    {
        uint8_t res = 0;
        if (hcan != _can || RxFifo != _RxFifo)
        {
        	_state = CAN_state::stop;
            return 5;
        }
        res = readBytes(_rx_head,buffer);
        return res;
    }

    uint8_t available(){
        return HAL_CAN_GetRxFifoFillLevel(_can, _RxFifo);
    }
};

#endif
