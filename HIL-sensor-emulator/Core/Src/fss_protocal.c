/*
 * fss_protocal.c
 *
 *  Created on: Jan 23, 2026
 *      Author: Chinnapop
 */


#include "fss_protocal.h"




uint16_t message_crc16(const uint8_t* data, size_t len) {
    const uint16_t POLY = 0x8408;
    uint16_t crc = 0xFFFF;

    while (len-- > 0) {
        uint8_t ch = *data++;
        for (int i = 0; i < 8; i++) {
            if ((ch ^ crc) & 0x01) {
                crc = (crc >> 1) ^ POLY;
            } else {
                crc >>= 1;
            }
            ch >>= 1;
        }
    }

    return crc;
}

uint8_t nsp_message_control_field_encode(uint8_t command_code,uint8_t ack_bit,uint8_t b_bit,uint8_t poll_bit)
{

	uint8_t msg_ctrl;

	msg_ctrl |= command_code & 0x1F;
	msg_ctrl |= (ack_bit & 0x01) << 5;
	msg_ctrl |= (b_bit & 0x01) << 6;
	msg_ctrl |= (poll_bit & 0x01) << 7;

    return msg_ctrl;

}

nsp_message_control_field nsp_message_control_field_decode(uint8_t message_control_field)
{
	nsp_message_control_field message_control_t;

	message_control_t.command_code = message_control_field & 0x1F;
	message_control_t.ack_bit = (message_control_field >> 5) & 0x01;
	message_control_t.b_bit = (message_control_field >> 6) & 0x01;
	message_control_t.poll_bit = (message_control_field >> 7) & 0x01;

	return message_control_t;
}
