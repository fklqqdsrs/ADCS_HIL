#ifndef HIL_FEEDBACK_H
#define HIL_FEEDBACK_H

#include <stdint.h>

#define HIL_UART_RX_SIZE 64U
#define HIL_UDP_DEST_IP "192.168.1.12"
//#define HIL_UDP_DEST_IP "127.0.0.1"
#define HIL_UDP_DEST_PORT 5005U
#define HIL_UDP_SRC_PORT 5006U

void HIL_Task(void const *argument);
void HIL_UartRxCpltFromISR(void);
void HIL_UartErrorFromISR(void);

#endif /* HIL_FEEDBACK_H */
