/*
 * Func_SSDV.c
 *
 *  Created on: Aug 9, 2024
 *      Author: thaib
 */

#include <src/func/func_ssdv/func_SSDV.h>

int set_ssdv_Parameter(uint8_t res , uint8_t quality , uint8_t camEnable ,uint32_t interval) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_SSDV, PORT_SSDV, 1000, CSP_O_NONE);
	if (conn == NULL) {
		csp_log_error("Connection failed\r\n");
		 return CSP_ERR_NOMEM;
	}
	csp_packet_t *packet = csp_buffer_get(sizeof(camera_dfl_t));
	if (packet == NULL) {
		csp_log_error("Failed to get CSP buffer\r\n");
		csp_close(conn);
        return CSP_ERR_BUSY;
	}
	*(camera_dfl_t *)packet->data = (camera_dfl_t){ res, quality, camEnable, interval };
	packet->length = sizeof(camera_dfl_t);
	if (!csp_send(conn, packet, 1000)) {
	 csp_log_error("Send failed\r\n");
        csp_buffer_free(packet);
        csp_close(conn);
        return CSP_ERR_TX;
	}
	csp_buffer_free(packet);
	while ((packet = csp_read(conn, 1000)) != NULL) {
		if (packet->data[0] == 0x00) {
			//csp_buffer_free(packet);
			csp_close(conn);
			return CSP_ERR_NONE;
		}
		csp_buffer_free(packet);
	}
	csp_close(conn);
	return CSP_ERR_TIMEDOUT;
}

// int set_ssdv_Parameter(uint8_t res, uint8_t quality, uint8_t camEnable, uint32_t interval) {
//     csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, ADDRESS_SSDV, PORT_SSDV, 1000, CSP_O_NONE);
//     if (conn == NULL) {
//         csp_log_error("Connection failed\r\n");
//         return CSP_ERR_NOMEM;
//     }

//     camera_dfl_t data = { res, quality, camEnable, interval };
//     csp_packet_t *packet = csp_buffer_get(sizeof(camera_dfl_t));
//     if (packet == NULL) {
//         csp_log_error("Failed to get CSP buffer\r\n");
//         csp_close(conn);
//         return CSP_ERR_BUSY;
//     }

//     memcpy(packet->data, &data, sizeof(camera_dfl_t));
//     packet->length = sizeof(camera_dfl_t);

//     if (!csp_send(conn, packet, 1000)) {
//         csp_log_error("Send failed\r\n");
//         csp_buffer_free(packet);
//         csp_close(conn);
//         return CSP_ERR_TX;
//     }

//     csp_buffer_free(packet);

//     while ((packet = csp_read(conn, 1000)) != NULL) {
//         if (packet->data[0] == 0x55) {
//             csp_buffer_free(packet);
//             csp_close(conn);
//             return CSP_ERR_NONE;
//         }
//         csp_buffer_free(packet);
//     }

//     csp_close(conn);
//     return CSP_ERR_TIMEDOUT;
// }

int reset_ssdv_Parameter(){
	return set_ssdv_Parameter(0, 0, 0, 0);
}

int check_ssdv_Connection(){
	int result = csp_ping(ADDRESS_SSDV, 1000, 2, CSP_O_NONE);
	//csp_log_info("Ping address: %u, result %d [mS]\r\n", ADDRESS_SSDV, result);
	if (result < 0)	return CSP_ERR_TX;
	return result ;
}

