/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : hil_feedback.c
  * @brief          : Hardware-in-the-loop UART->UDP feedback bridge
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "hil_feedback.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lwip.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "usart.h"

#include <string.h>

/* USER CODE BEGIN 0 */
static int hil_udp_socket = -1;
static struct sockaddr_in hil_dest_addr;

static TaskHandle_t hil_task_handle = NULL;
static volatile uint8_t hil_ready_mask = 0U;
static volatile uint8_t hil_active_buf = 0U;
static volatile uint32_t hil_overrun_count = 0U;

ALIGN_32BYTES(static uint8_t hil_uart_rx_buf[2][HIL_UART_RX_SIZE]);

static void HIL_StartUartRx(uint8_t buf_index)
{
#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
  if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)hil_uart_rx_buf[buf_index], (int32_t)HIL_UART_RX_SIZE);
  }
#endif

  (void)HAL_UART_Receive_DMA(&huart2, hil_uart_rx_buf[buf_index], HIL_UART_RX_SIZE);
}

static void HIL_InitUdpSocket(void)
{
  struct sockaddr_in local_addr;

  memset(&hil_dest_addr, 0, sizeof(hil_dest_addr));
  hil_dest_addr.sin_family = AF_INET;
  hil_dest_addr.sin_port = PP_HTONS(HIL_UDP_DEST_PORT);
  hil_dest_addr.sin_addr.s_addr = inet_addr(HIL_UDP_DEST_IP);

  hil_udp_socket = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = PP_HTONS(HIL_UDP_SRC_PORT);
  local_addr.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

  if (hil_udp_socket >= 0) {
    (void)lwip_bind(hil_udp_socket, (struct sockaddr *)&local_addr, (socklen_t)sizeof(local_addr));
  }
}
/* USER CODE END 0 */

void HIL_Task(void const *argument)
{
  (void)argument;

  MX_LWIP_Init();

  hil_task_handle = xTaskGetCurrentTaskHandle();
  hil_ready_mask = 0U;
  hil_active_buf = 0U;
  HIL_InitUdpSocket();
  HIL_StartUartRx(hil_active_buf);

  for (;;)
  {
    (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    for (;;)
    {
      uint8_t buf_index = 0xFF;

      taskENTER_CRITICAL();
      if ((hil_ready_mask & 0x01U) != 0U) {
        hil_ready_mask &= (uint8_t)~0x01U;
        buf_index = 0U;
      } else if ((hil_ready_mask & 0x02U) != 0U) {
        hil_ready_mask &= (uint8_t)~0x02U;
        buf_index = 1U;
      }
      taskEXIT_CRITICAL();

      if (buf_index == 0xFF) {
        break;
      }

      if (hil_udp_socket >= 0) {
        (void)lwip_sendto(hil_udp_socket,
                          hil_uart_rx_buf[buf_index],
                          HIL_UART_RX_SIZE,
                          0,
                          (struct sockaddr *)&hil_dest_addr,
                          (socklen_t)sizeof(hil_dest_addr));
      }
    }
  }
}

void HIL_UartRxCpltFromISR(void)
{
  BaseType_t higher_priority_woken = pdFALSE;
  uint8_t completed = hil_active_buf;

#if defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
  if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
    SCB_InvalidateDCache_by_Addr((uint32_t *)hil_uart_rx_buf[completed], (int32_t)HIL_UART_RX_SIZE);
  }
#endif

  hil_ready_mask |= (uint8_t)(1U << completed);

  {
    uint8_t next = (uint8_t)(completed ^ 1U);

    if ((hil_ready_mask & (uint8_t)(1U << next)) != 0U) {
      hil_ready_mask &= (uint8_t)~(1U << next);
      hil_overrun_count++;
    }

    hil_active_buf = next;
    HIL_StartUartRx(hil_active_buf);
  }

  if (hil_task_handle != NULL) {
    vTaskNotifyGiveFromISR(hil_task_handle, &higher_priority_woken);
  }
  portYIELD_FROM_ISR(higher_priority_woken);
}

void HIL_UartErrorFromISR(void)
{
  HIL_StartUartRx(hil_active_buf);
}
