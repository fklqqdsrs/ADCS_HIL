/*
 * Func_Time_sync.c
 *
 *  Created on: May 30, 2024
 *      Author: TSC-PC
 */

// #include "src/csp_conn.h"
// #include "src/csp_port.h"
#include <src/func/func_timesync/Func_Time_sync.h>

//#include  CONFIG_STM32_HAL_TIME

static uint32_t startTime_ms;
static uint32_t offsetTime_ms;

typedef struct {
	uint32_t interval_s;
	uint8_t node;
} auto_time_sync_param_t;

uint32_t get_time_sync_ms() {
	uint32_t currentTime = csp_get_ms();
	uint32_t elapsedTime = currentTime + offsetTime_ms;
	return elapsedTime;
}

uint32_t get_time_sync_s() {
	uint32_t currentTime_ms = csp_get_ms();
	uint32_t elapsedTime_ms = currentTime_ms + offsetTime_ms;

	uint32_t elapsedTime_s = elapsedTime_ms / 1000;
	if (elapsedTime_ms % 1000 >= 500) {
		elapsedTime_s += 1;
	}

	return elapsedTime_s;
}

void set_time_sync_ms(uint32_t time) {
	startTime_ms = csp_get_ms();
	offsetTime_ms = time - startTime_ms;
}

void set_time_sync_s(uint32_t time) {
	startTime_ms = csp_get_ms();
	offsetTime_ms = (time * 1000) - startTime_ms;
}

uint32_t get_last_sync_time() {
	return startTime_ms;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == ISR_TRIGGLE) {
		time_trigger();
	}
}

void time_trigger() {
	uint32_t currentTime = get_time_sync_ms();
	uint32_t adjustedTime = ((currentTime + 500) / 1000) * 1000;
	set_time_sync_ms(adjustedTime);

	// printf("Time Triggered: Current=%u Adjusted=%u\n", currentTime, adjustedTime);
}

CSP_DEFINE_TASK(auto_time_sync) {
	auto_time_sync_param_t *params = (auto_time_sync_param_t*) param;
	uint32_t interval_s = params->interval_s;
	uint8_t node = params->node;

	while (1) {
		uint32_t time_to_sync = get_time_sync_ms();
		if (csp_set_time_sync(node, time_to_sync, 1000) != 0) {
			printf("Failed to set time sync in auto_time_sync");
		}
		csp_sleep_ms(interval_s * 1000);
	}
	return CSP_TASK_RETURN;
}

int enable_auto_time_sync(uint32_t interval_s, uint8_t node) {
	auto_time_sync_param_t *params = malloc(sizeof(auto_time_sync_param_t));
	if (params == NULL) {
		return -1;
	}
	params->interval_s = interval_s;
	params->node = node;

	int result = csp_thread_create(auto_time_sync, "AUTO_TIME_SYNC", 512, params, 0, NULL);
	if (result != CSP_ERR_NONE) {
		free(params);
		return -2;
	}
	return 0;
}

int csp_set_time_sync(uint8_t node, uint32_t timesync, uint32_t timeout) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, PORT_TIME_SYNC, timeout, CSP_O_NONE);
	if (conn == NULL) {
		return -1;
	}
	csp_packet_t *packet = csp_buffer_get(100);
	if (packet == NULL) {
		return -2;
	}
	packet->data[0] = NSET_TIME_SYNC;
	memcpy(&packet->data[1], &timesync, sizeof(timesync));
	packet->length = 1 + sizeof(timesync);
	if (!csp_send(conn, packet, timeout)) {
		csp_buffer_free(packet);
		return -3;
	}
	csp_close(conn);
	return 0;
}

int csp_time_sync_broadcast(uint32_t timesync, uint32_t timeout) {
	set_time_sync_ms(timesync);
	return csp_set_time_sync(CSP_BROADCAST_ADDR, timesync, timeout);
}

int csp_get_time_sync(uint8_t node, uint32_t timeout, uint32_t *received_time) {
	uint32_t send_time = csp_get_ms();
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, PORT_TIME_SYNC, timeout, CSP_O_NONE);
	if (conn == NULL) {
		return -1;
	}
	csp_packet_t *packet = csp_buffer_get(100);
	if (packet == NULL) {
		csp_close(conn);
		return -2;
	}
	packet->data[0] = NGET_TIME_SYNC;
	memcpy(&packet->data[1], &send_time, sizeof(send_time));
	packet->length = 1 + sizeof(send_time);

	if (!csp_send(conn, packet, timeout)) {
		csp_buffer_free(packet);
		csp_close(conn);
		return -3;
	}
	while ((packet = csp_read(conn, timeout)) != NULL) {
		if (packet->data[0] == NGET_TIME_SYNC) {
			uint32_t received_sync_time;
			memcpy(&received_sync_time, &packet->data[1], sizeof(received_sync_time));
			uint32_t response_time = csp_get_ms();

			uint32_t round_trip = response_time - send_time;
			*received_time = received_sync_time + round_trip / 2;
			//printf("Received time sync: %u (adjusted with round-trip delay: %u ms)\n", *received_time, round_trip / 2);
			csp_buffer_free(packet);
			csp_close(conn);
			return 0;
		}
		csp_buffer_free(packet);
	}
	csp_close(conn);
	return -4;
}

void service_time_sync(csp_conn_t *conn, csp_packet_t *packet) {
	if (packet->data[0] == NSET_TIME_SYNC) {
		uint32_t received_time;
		memcpy(&received_time, &packet->data[1], sizeof(received_time));
		set_time_sync_ms(received_time);
		//printf("service_time_sync: Set time sync: %lu\n", received_time);
	} else if (packet->data[0] == NGET_TIME_SYNC) {
		uint32_t current_time = get_time_sync_ms();
		csp_packet_t *response_packet = csp_buffer_get(100);
		if (response_packet != NULL) {
			response_packet->data[0] = NGET_TIME_SYNC;
			memcpy(&response_packet->data[1], &current_time, sizeof(current_time));
			response_packet->length = 1 + sizeof(current_time);
			if (!csp_send(conn, response_packet, 1000)) {
				//		printf("service_time_sync: !Sent time sync\r\n");
			}
//			printf("service_time_sync: Sent time sync: %lu\n", current_time);
		}
	} else if (packet->data[0] == BROAD_CAST_TIMESYNC) {
		uint32_t received_time;
		memcpy(&received_time, &packet->data[1], sizeof(received_time));
		set_time_sync_ms(received_time);
		//	printf("service_time_sync: time sync broadcsat: %u\n", (unsigned int)received_time);
	}
	csp_buffer_free(packet);
}
