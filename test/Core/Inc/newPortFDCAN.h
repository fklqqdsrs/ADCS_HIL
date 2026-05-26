#ifndef _newPortFDCAN_h_
#define _newPortFDCAN_h_

/*
for can fd buadrate https://phryniszak.github.io/stm32g-fdcan/
data bit is for using comunicate other fdcan device
nominal bit is can 2.0b device
*/

#include "../newHeader.h"

enum class CAN_state
{
    stop,
    waiting,
    getData,
    sendData
};

class newPortFDCAN : public newPort
{
private:
    FDCAN_HandleTypeDef *_can;
    CAN_state _state = CAN_state::stop;
    FDCAN_FilterTypeDef canfilterconfig;
    FDCAN_TxHeaderTypeDef *_tx_head;
    FDCAN_RxHeaderTypeDef *_rx_head;
    uint8_t _self_id;
    uint32_t _RxFifo;

public:
    newPortFDCAN(FDCAN_HandleTypeDef *can, uint8_t self_id, uint32_t RxFifo = FDCAN_RX_FIFO0) : newPort(AS_MASTER)
    {
        _can = can;
        _self_id = self_id;
        _RxFifo = RxFifo;
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
};

#endif
