/*
 * register_protocol.h
 *
 * Shared protocol definitions for the Register service (client/server/CLI).
 */

#ifndef REGISTER_PROTOCOL_H_
#define REGISTER_PROTOCOL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Configuration
#ifndef MAX_REGISTERS
#define MAX_REGISTERS 100
#endif

// Actions (4 bits)
#define NONE_REGISTER       0x00
#define ADD_REGISTER        0x01
#define READ_REGISTER       0x02
#define WRITE_REGISTER      0x03
#define REMOVE_REGISTER     0x04
#define GET_REGISTER_COUNT  0x05
#define GET_STATUS_REGISTER 0x06

// Flags (4 bits)
#define FLAGS_NONE      0x00
#define FLAGS_SUCCESS   0x01
#define FLAGS_BUSY      0x02
#define FLAGS_TIMEOUT   0x03
#define FLAGS_NOT_FOUND 0x04

#ifdef __cplusplus
}
#endif

#endif /* REGISTER_PROTOCOL_H_ */

