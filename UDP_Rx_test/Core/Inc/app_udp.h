/*
 * app_udp.h
 *
 *  Created on: Jan 9, 2026
 *      Author: Chinnapop
 */

#ifndef INC_APP_UDP_H_
#define INC_APP_UDP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void UDP_Server_Init(uint16_t listen_port);
bool UDP_Server_Pop(uint8_t *dst, uint16_t dst_max, uint16_t *out_len);



#endif /* INC_APP_UDP_H_ */
