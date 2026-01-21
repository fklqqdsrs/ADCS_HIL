/*
 * Func_Time_sync.h
 *
 *  Created on: May 30, 2024
 *      Author: TSC-PC
 */

#ifndef SRC_FUNC_TIME_SYNC_H_
#define SRC_FUNC_TIME_SYNC_H_

#include "main.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <include/csp/arch/csp_time.h>
#include <include/csp/csp.h>
#include <include/csp/csp_buffer.h>
#include <include/csp/arch/csp_thread.h>

#if defined STM32F722xx
  #define CONFIG_STM32_HAL_TIME "stm32f7xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#elif defined STM32L496xx
  #define CONFIG_STM32_HAL_TIME "stm32l4xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#elif defined STM32H723xx
 #define CONFIG_STM32_HAL_TIME "stm32h7xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#elif defined STM32G431xx
  #define CONFIG_STM32_HAL_TIME "stm32g4xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#elif defined STM32F103xB
  #define CONFIG_STM32_HAL_TIME "stm32f1xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#elif defined STM32F767xx
  #define CONFIG_STM32_HAL_TIME "stm32f7xx_hal.h"
 #define ISR_TRIGGLE GPIO_PIN_15
#else

#endif

#define PORT_TIME_SYNC 8

#define NSET_TIME_SYNC 0x27
#define NGET_TIME_SYNC 0x28
#define BROAD_CAST_TIMESYNC 0x29

#define AUTO_TIME_SYNC 0x30
#define TRIGGLE_ADJUST_SYNC 0x31
#ifdef __cplusplus
extern "C" {
#endif
/**
 * The function `get_time_sync_ms` calculates the elapsed time in milliseconds by adding the current
 * time to an offset time.
 * 
 * @return The function `get_time_sync_ms` returns the elapsed time in milliseconds, which is
 * calculated by adding the current time in milliseconds (`currentTime`) to the offset time in
 * milliseconds (`offsetTime_ms`).
 */
uint32_t get_time_sync_ms();

/**
 * The function `get_time_sync_s` calculates the elapsed time in seconds with a precision of 1 second.
 * 
 * @return The function `get_time_sync_s` returns the elapsed time in seconds, synchronized to the
 * nearest second.
 */
uint32_t get_time_sync_s();

/**
 * The function `set_time_sync_ms` calculates the offset time in milliseconds based on the current
 * time.
 * 
 * @param time The `time` parameter represents the desired time in milliseconds that you want to
 * synchronize with.
 */
void set_time_sync_ms(uint32_t time) ;

/**
 * The function `set_time_sync_s` sets the synchronization time based on the input time in seconds.
 * 
 * @param time The `time` parameter in the `set_time_sync_s` function represents the time in seconds
 * that you want to synchronize with.
 */
void set_time_sync_s(uint32_t time) ;

/**
 * The function get_last_sync_time() returns the value of startTime_ms, which represents the last
 * synchronization time.
 * 
 * @return The function `get_last_sync_time` returns the value of the variable `startTime_ms`, which is
 * of type `uint32_t`.
 */
uint32_t get_last_sync_time();


/**
 * The HAL_GPIO_EXTI_Callback function triggers a time_trigger function when a specific GPIO pin is
 * activated.
 * 
 * @param GPIO_Pin GPIO_Pin is a parameter representing the specific GPIO pin that triggered the
 * interrupt in the HAL_GPIO_EXTI_Callback function.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/**
 * The function `time_trigger` adjusts a given time value to the nearest second based on the last digit
 * of the milliseconds part.
 */
void time_trigger();

/**
 * The function `auto_time_sync` periodically synchronizes time with a specified node using the CSP
 * protocol.
 * 
 * @return CSP_TASK_RETURN
 */
CSP_DEFINE_TASK(auto_time_sync);

/**
 * The function `enable_auto_time_sync` allocates memory for thread parameters, sets the interval and
 * node values, creates a thread for auto time synchronization, and returns 0 if successful.
 * 
 * @param interval_ms The `interval_ms` parameter specifies the time interval in milliseconds for the
 * auto time synchronization process.
 * @param node The `node` parameter in the `enable_auto_time_sync` function is of type `uint8_t` and
 * represents the identifier of the node for which the auto time synchronization is being enabled.
 * 
 * @return The function `enable_auto_time_sync` returns an integer value. It returns 0 if the auto time
 * sync thread creation is successful, and it returns -1 if there is a failure in memory allocation or
 * thread creation.
 */
int enable_auto_time_sync(uint32_t interval_s, uint8_t node);

/**
 * The function `csp_set_time_sync` establishes a CSP connection to a specified node and sends a time
 * synchronization packet containing a timestamp.
 * 
 * @param node The `node` parameter in the `csp_set_time_sync` function is of type `uint8_t` and
 * represents the node to which the time synchronization will be set.
 * @param timesync The `timesync` parameter in the `csp_set_time_sync` function is a 32-bit unsigned
 * integer that represents the time synchronization value to be set for a specific node. This value is
 * used to synchronize time between nodes in a network.
 * @param timeout The `timeout` parameter in the `csp_set_time_sync` function represents the maximum
 * time allowed for the operation to complete before it times out. It is specified in milliseconds. If
 * the operation takes longer than the specified timeout value, the function will return an error.
 * 
 * @return The function `csp_set_time_sync` returns an integer value. If the function executes
 * successfully, it returns 0. If there are any failures during the execution of the function, it
 * returns -1.
 */
int csp_set_time_sync(uint8_t node ,uint32_t timesync,uint32_t timeout);

/**
 * The function `csp_get_time_sync` establishes a connection with a node, sends a request for time
 * synchronization, and receives the synchronized time from the node.
 * 
 * @param node The `node` parameter in the `csp_get_time_sync` function is of type `uint8_t` and
 * represents the node identifier to which the time synchronization request will be sent.
 * @param timeout The `timeout` parameter in the `csp_get_time_sync` function is the duration in
 * milliseconds for which the function will wait to receive a response before timing out.
 * @param received_time The `received_time` parameter is a pointer to a `uint32_t` variable where the
 * received time synchronization data will be stored after the function `csp_get_time_sync` is executed
 * successfully.
 * 
 * @return The function `csp_get_time_sync` returns an integer value. If the function is successful in
 * receiving and processing the time synchronization data from the specified node, it returns 0. If
 * there are any errors during the process, such as failure to establish a connection, send or receive
 * data, or encountering an invalid header, it returns -1.
 */
int csp_get_time_sync(uint8_t node, uint32_t timeout, uint32_t *received_time);

/**
 * The function `csp_time_sync_broadcast` sets the time synchronization in milliseconds and broadcasts
 * it to all nodes in the network with a specified timeout.
 * 
 * @param timesync The `timesync` parameter is a 32-bit unsigned integer that represents the time
 * synchronization value to be set.
 * @param timeout The `timeout` parameter in the `csp_time_sync_broadcast` function is a `uint32_t`
 * type variable that represents the time duration in milliseconds for which the function will wait for
 * a response after broadcasting the time synchronization message. If no response is received within
 * this timeout period, the function will
 * 
 * @return The function `csp_set_time_sync` is being called with parameters `CSP_BROADCAST_ADDR`,
 * `timesync`, and `timeout`, and the return value of this function is being returned by the
 * `csp_time_sync_broadcast` function.
 */
int csp_time_sync_broadcast(uint32_t timesync,uint32_t timeout);

/**
 * The function `service_time_sync` handles different types of time synchronization packets, setting or
 * retrieving time sync values accordingly.
 * 
 * @param conn The `conn` parameter in the `service_time_sync` function is of type `csp_conn_t*`, which
 * is a pointer to a CSP connection structure. This structure likely contains information about the
 * connection over which the packet is received or needs to be sent. It may include details such as the
 * @param packet The `packet` parameter is of type `csp_packet_t` and contains data that is being
 * received for time synchronization. The function `service_time_sync` processes this packet to either
 * set the received time for synchronization, get the current time for synchronization, or handle a
 * broadcast time synchronization message.
 */
void service_time_sync(csp_conn_t * conn, csp_packet_t * packet);
#ifdef __cplusplus
}
#endif
#endif /* SRC_FUNC_TIME_SYNC_H_ */
