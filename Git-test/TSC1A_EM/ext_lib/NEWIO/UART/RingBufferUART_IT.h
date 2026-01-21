#ifndef RINGBUFFERUART_H_
#define RINGBUFFERUART_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include <string.h>

/* change the size of the buffer */
#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 32
#endif

typedef struct {
	unsigned char buffer[UART_BUFFER_SIZE];
	volatile unsigned int head;
	volatile unsigned int tail;
} ring_buffer;

class RingBufferUART_IT {
private:
	ring_buffer rx_buffer = { { 0 }, 0, 0 };
	ring_buffer tx_buffer = { { 0 }, 0, 0 };
	ring_buffer *_rx_buffer;
	ring_buffer *_tx_buffer;
	UART_HandleTypeDef *_uart;

public:
	RingBufferUART_IT(UART_HandleTypeDef *uart) {
		_uart = uart;
		_rx_buffer = &rx_buffer;
		_tx_buffer = &tx_buffer;
	}

	UART_HandleTypeDef* getHandleTypeDef() {
		return _uart;
	}

	void ring_init() {
		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		__HAL_UART_ENABLE_IT(_uart, UART_IT_ERR);
		/* Enable the UART Data Register not empty Interrupt */
		__HAL_UART_ENABLE_IT(_uart, UART_IT_RXNE);
	}
	void store_char(unsigned char c, ring_buffer *buffer) {
		int i = (unsigned int) (buffer->head + 1) % UART_BUFFER_SIZE;

		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		if (i != buffer->tail) {
			buffer->buffer[buffer->head] = c;
			buffer->head = i;
		}
	}
	/* Look for a particular string in the given buffer
	 * @return 1, if the string is found and -1 if not found
	 * @USAGE:: if (Look_for ("some string", buffer)) do something
	 */
	int Look_for(char *str, char *buffertolookinto) {
		int stringlength = strlen(str);
		int bufferlength = strlen(buffertolookinto);
		int so_far = 0;
		int indx = 0;
		repeat: while (str[so_far] != buffertolookinto[indx])
			indx++;
		if (str[so_far] == buffertolookinto[indx]) {
			while (str[so_far] == buffertolookinto[indx]) {
				so_far++;
				indx++;
			}
		}

		else {
			so_far = 0;
			if (indx >= bufferlength)
				return -1;
			goto repeat;
		}

		if (so_far == stringlength)
			return 1;
		else
			return -1;
	}

	void GetDataFromBuffer(char *startString, char *endString,
			char *buffertocopyfrom, char *buffertocopyinto) {
		int startStringLength = strlen(startString);
		int endStringLength = strlen(endString);
		int so_far = 0;
		int indx = 0;
		int startposition = 0;
		int endposition = 0;

		repeat1: while (startString[so_far] != buffertocopyfrom[indx])
			indx++;
		if (startString[so_far] == buffertocopyfrom[indx]) {
			while (startString[so_far] == buffertocopyfrom[indx]) {
				so_far++;
				indx++;
			}
		}

		if (so_far == startStringLength)
			startposition = indx;
		else {
			so_far = 0;
			goto repeat1;
		}

		so_far = 0;

		repeat2: while (endString[so_far] != buffertocopyfrom[indx])
			indx++;
		if (endString[so_far] == buffertocopyfrom[indx]) {
			while (endString[so_far] == buffertocopyfrom[indx]) {
				so_far++;
				indx++;
			}
		}

		if (so_far == endStringLength)
			endposition = indx - endStringLength;
		else {
			so_far = 0;
			goto repeat2;
		}

		so_far = 0;
		indx = 0;

		for (int i = startposition; i < endposition; i++) {
			buffertocopyinto[indx] = buffertocopyfrom[i];
			indx++;
		}
	}

	void flush() {
		memset(_rx_buffer->buffer, '\0', UART_BUFFER_SIZE);
		_rx_buffer->head = 0;
	}

	/* Peek for the data in the Rx Bffer without incrementing the tail count
	 * Returns the character
	 * USAGE: if (Uart_peek () == 'M') do something
	 */
	int peek() {
		if (_rx_buffer->head == _rx_buffer->tail) {
			return -1;
		} else {
			return _rx_buffer->buffer[_rx_buffer->tail];
		}
		return -1;
	}

	/* reads the data in the rx_buffer and increment the tail count in rx_buffer of the given UART */
	int read() {

		// if the head isn't ahead of the tail, we don't have any characters
		if (_rx_buffer->head == _rx_buffer->tail) {
			return -1;
		} else {
			unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
			_rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1)
					% UART_BUFFER_SIZE;
			return c;
		}
	}

	/* writes the data to the tx_buffer and increment the head count in tx_buffer */
	void write(int c) {
		if (c >= 0) {
			int i = (_tx_buffer->head + 1) % UART_BUFFER_SIZE;

			// If the output buffer is full, there's nothing for it other than to
			// wait for the interrupt handler to empty it a bit
			// ???: return 0 here instead?
			while (i == _tx_buffer->tail)
				;

			_tx_buffer->buffer[_tx_buffer->head] = (uint8_t) c;
			_tx_buffer->head = i;

			__HAL_UART_ENABLE_IT(_uart, UART_IT_TXE); // Enable UART transmission interrupt
		}
	}
	/* checks if the data is available to read in the rx_buffer of the uart */
	int IsDataAvailable() {
		return (uint16_t) (UART_BUFFER_SIZE + _rx_buffer->head
				- _rx_buffer->tail) % UART_BUFFER_SIZE;
	}

	/* Copies the entered number of characters (blocking mode) from the Rx buffer into the buffer, after some particular string is detected
	 * Returns 1 on success and -1 otherwise
	 * USAGE: while (!(Get_after ("some string", 6, buffer, uart)));
	 */
	int Get_after(char *string, uint8_t numberofchars, char *buffertosave) {

		while (Wait_for(string) != 1)
			;
		for (int indx = 0; indx < numberofchars; indx++) {
			while (!(IsDataAvailable()))
				;
			buffertosave[indx] = read();
		}
		return 1;
	}

	/* function to send the string to the uart */
	void sendstring(const char *s) {
		while (*s != '\0')
			write(*s++);
	}

	/* Print a number with any base
	 * base can be 10, 8 etc*/
	void printbase(long n, uint8_t base) {
		char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
		char *s = &buf[sizeof(buf) - 1];

		*s = '\0';

		// prevent crash if called with base == 1
		if (base < 2)
			base = 10;

		do {
			unsigned long m = n;
			n /= base;
			char c = m - base * n;
			*--s = c < 10 ? c + '0' : c + 'A' - 10;
		} while (n);

		while (*s)
			write(*s++);
	}

	int get_dataline(char *buffertocopyinto) {

		return Copy_upto("\n", buffertocopyinto);
	}
	/* Copy the data from the Rx buffer into the buffer, Upto and including the entered string
	 * This copying will take place in the blocking mode, so you won't be able to perform any other operations
	 * Returns 1 on success and -1 otherwise
	 * USAGE: while (!(Copy_Upto ("some string", buffer, uart)));
	 */
	int Copy_upto(char *string, char *buffertocopyinto) {
		int so_far = 0;
		int len = strlen(string);
		int indx = 0;

		again: while (!IsDataAvailable())
			;
		while (peek() != string[so_far]) {
			buffertocopyinto[indx] = _rx_buffer->buffer[_rx_buffer->tail];
			_rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1)
					% UART_BUFFER_SIZE;
			indx++;
			while (!IsDataAvailable())
				;
		}
		while (peek() == string[so_far]) {
			so_far++;
			buffertocopyinto[indx++] = read();
			if (so_far == len)
				return 1;
			while (!IsDataAvailable())
				;
		}

		if (so_far != len) {
			so_far = 0;
			goto again;
		}

		if (so_far == len)
			return 1;
		else
			return -1;
	}
	/* Wait until a paricular string is detected in the Rx Buffer
	 * Return 1 on success and -1 otherwise
	 * USAGE: while (!(Wait_for("some string", uart)));
	 */
	int Wait_for(char *string) {
		int so_far = 0;
		int len = strlen(string);

		again_device:
		while (!IsDataAvailable())
			;
		if (peek() != string[so_far]) {
			_rx_buffer->tail = (unsigned int) (_rx_buffer->tail + 1)
					% UART_BUFFER_SIZE;
			goto again_device;
		}
		while (peek() == string[so_far]) {
			so_far++;
			read();
			if (so_far == len)
				flush();
			return 1;
			while (!IsDataAvailable())
				;
		}

		if (so_far != len) {
			so_far = 0;
			goto again_device;
		}

		if (so_far == len) {
			flush();
			return 1;
		}

		else
			return -1;
	}

	/* the ISR for the uart. put it in the IRQ handler */
	void isr() {
		uint32_t isrflags = READ_REG(_uart->Instance->ISR);
		uint32_t cr1its = READ_REG(_uart->Instance->CR1);

		/* if DR is not empty and the Rx Int is enabled */
		if (((isrflags & USART_ISR_RXNE) != RESET)
				&& ((cr1its & USART_CR1_RXNEIE) != RESET)) {
			/******************
			 *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
			 *          error) and IDLE (Idle line detected) flags are cleared by software
			 *          sequence: a read operation to USART_SR register followed by a read
			 *          operation to USART_DR register.
			 * @note   RXNE flag can be also cleared by a read to the USART_DR register.
			 * @note   TC flag can be also cleared by software sequence: a read operation to
			 *          USART_SR register followed by a write operation to USART_DR register.
			 * @note   TXE flag is cleared only by a write to the USART_DR register.

			 *********************/
			_uart->Instance->ISR; /* Read status register */
			unsigned char c = _uart->Instance->RDR; /* Read data register */
			store_char(c, _rx_buffer);             // store data in buffer

			return;
		}

		/*If interrupt is caused due to Transmit Data Register Empty */
		if (((isrflags & USART_ISR_TXE) != RESET)
				&& ((cr1its & USART_CR1_TXEIE) != RESET)) {

			if (tx_buffer.head == tx_buffer.tail) {
				// Buffer empty, so disable interrupts
				__HAL_UART_DISABLE_IT(_uart, UART_IT_TXE);
			}

			else {
				// There is more data in the output buffer. Send the next byte
				unsigned char c = tx_buffer.buffer[tx_buffer.tail];
				tx_buffer.tail = (tx_buffer.tail + 1) % UART_BUFFER_SIZE;

				/******************
				 *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
				 *          error) and IDLE (Idle line detected) flags are cleared by software
				 *          sequence: a read operation to USART_SR register followed by a read
				 *          operation to USART_DR register.
				 * @note   RXNE flag can be also cleared by a read to the USART_DR register.
				 * @note   TC flag can be also cleared by software sequence: a read operation to
				 *          USART_SR register followed by a write operation to USART_DR register.
				 * @note   TXE flag is cleared only by a write to the USART_DR register.

				 *********************/

				_uart->Instance->ISR;
				_uart->Instance->TDR = c;
			}

			return;
		}
	}
};
#ifdef __cplusplus
}
#endif

#endif /* UARTRINGBUFFER_H_ */
