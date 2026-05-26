#ifndef _NEWUART_H_
#define _NEWUART_H_
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "newPort.h"
#include "newHeader.h"


#define MAX_DMA_LEN_BUF 500


class NEWUART : public newPort {
private:
	UART_HandleTypeDef* _uart;
	uint32_t _baudrate;
	uint32_t _Timeout;
	uint8_t _buffer_size = 0;
	uint8_t _is_DMA = 0;
	uint8_t _rx_dma_buf[20];
	uint8_t _tx_dma_buf[MAX_DMA_LEN_BUF];
    uint16_t _tx_dma_len = 0;

public:
	NEWUART(UART_HandleTypeDef* uart, const uint32_t Timeout, uint32_t BuadRate =
			115200,uint8_t mode_t = AS_MASTER) : newPort(mode_t) {
		_uart = uart;
		_uart->Init.BaudRate = BuadRate;
		_Timeout = Timeout;
		_baudrate = BuadRate;
	}
	UART_HandleTypeDef* getHandleTypeDef() {
		return _uart;
	}
    uint8_t* getTxHeaderPtr(){
        return _tx_dma_buf;
    }

	void begin(const uint32_t BuadRate) {
        _uart->Init.BaudRate = BuadRate;
	}
	void writeByte(const uint8_t data) {
		HAL_UART_Transmit(_uart, &data, sizeof(data), _Timeout);
	}
    
	uint8_t readByte(uint8_t *buffer) {
		return HAL_UART_Receive(_uart, buffer, 1, _Timeout);
	}
	uint8_t readBytes(uint8_t *buffer, uint8_t len, uint8_t asDMA = 0) {
		if (asDMA == 1) {
			memset(_rx_dma_buf, 0, 20);
			HAL_UART_DMAStop(_uart);
			return HAL_UART_Receive_DMA(_uart, _rx_dma_buf, len);
		}
		return HAL_UART_Receive(_uart, buffer, len, _Timeout);
	}
	 void newflush() {
	        memset(_tx_dma_buf, 0, _tx_dma_len);
	        _tx_dma_len = 0;
	}
    void flush(uint16_t len) {
        memset(_tx_dma_buf, 0, len);
        _tx_dma_len = 0;
    }
    void appendBytes(uint8_t *buffer, uint16_t len) {
        memcpy(_tx_dma_buf+_tx_dma_len, buffer, len);
        _tx_dma_len += len;

    }
    void appendChars(char *buffer, uint8_t len){
        appendBytes((uint8_t*)buffer,len);
    }

    uint8_t print_clear(){
        _tx_dma_buf[_tx_dma_len] = '\0';
        uint8_t temp = writeBytes(_tx_dma_buf,strlen((char*)_tx_dma_buf),1);
        return temp;
    }

	uint8_t writeBytes(uint8_t* data, uint8_t len, uint8_t asDMA = 0) {
		if (asDMA == 1) {
			memcpy(_tx_dma_buf, data, len);
			return HAL_UART_Transmit_DMA(_uart, _tx_dma_buf, len);
		}

		return HAL_UART_Transmit(_uart, data, len, _Timeout);
	}

	void print(const char text[], uint8_t asDMA = 0) {
		if (asDMA) {
			this->writeBytes((uint8_t*) text, strlen(text), asDMA);
			return;
		} else {
			uint8_t len = strlen(text);
			uint8_t buf[len + 1], i;
			for (i = 0; i < len + 1; i++) {
				buf[i] = text[i];
			}
			buf[i] = '\0';
			this->writeBytes(buf, sizeof(buf));
		}

	}
	void printI(int16_t data) {
		char buf[11] = { 0 };
		sprintf(buf, "%d", data);
		this->print(buf);
	}
	void printuint32_t(uint32_t data) {
		char buf[11] = { 0 };
		sprintf(buf, "%lu", data);
		this->print(buf);
	}
	void printF(float data) {
		char buf[11] = { 0 };
		sprintf(buf, "%.4f", data);
		this->print(buf);
	}
	void println(const char text[]) {
		print(text);
		print("\r\n");
	}
	/*
uint8_t readMEMs(const uint8_t addr, const uint16_t reg, uint8_t *buffer, const uint8_t len)
	{
		CubeUart.encode_package_tlm(reg);
		uint8_t error = this->writeBytes(CubeUart.get_tx_h(), CubeUart.get_txlen(), 1);
		HAL_Delay(300);
		return error;
	}
	uint8_t writeMEMs(const uint8_t addr, const uint16_t reg, uint8_t *buffer, const uint8_t len)
	{
		CubeUart.encode_package_tc(reg);
		uint8_t error = this->writeBytes(CubeUart.get_tx_h(), CubeUart.get_txlen(), 1);
		HAL_Delay(300);
		return error;
	}
	*/
};
#endif
