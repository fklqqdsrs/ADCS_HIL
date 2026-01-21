

#ifndef SRC_FUNC_CHICKEN_H_
#define SRC_FUNC_CHICKEN_H_

#include <include/csp/csp.h>

#ifdef __cplusplus
extern "C" {
#endif

int send_chicken(uint8_t node, uint8_t prio,  uint16_t dport, uint32_t timeout, uint8_t opts, uint8_t *data, uint16_t  data_length);

int request_chicken(uint8_t node, uint8_t prio,  uint16_t dport, uint32_t timeout, uint8_t opts,
                    uint8_t *send_data, uint16_t  send_data_length,
                    uint8_t *recv_data, uint16_t  *recv_data_length);



#ifdef __cplusplus
}
#endif
#endif 
