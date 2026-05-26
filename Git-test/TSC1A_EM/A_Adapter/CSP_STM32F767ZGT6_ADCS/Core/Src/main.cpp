/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <include/csp/csp.h>
#include <include/csp/arch/csp_thread.h>
#include <src/drivers/can_stm/f767zgtx_can.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MY_SERVER_PORT		10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan3;

/* USER CODE BEGIN PV */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask", .stack_size = 128 * 4, .priority = (osPriority_t) osPriorityNormal, };

const char *rtable = NULL;
uint32_t numcheck = 0;
int result;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN3_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */

static CSP_DEFINE_TASK(task_server);
static CSP_DEFINE_TASK(task_client);
int test_csp_server_task(unsigned int task_stack_size, unsigned int task_priority);
int test_csp_client_task(unsigned int task_stack_size, unsigned int task_priority);
int test_csp_server_client_task(unsigned int task_stack_size, unsigned int task_priority);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Server task - handles requests from clients */
CSP_DEFINE_TASK(task_server) {

	csp_log_info("Server task started");

	/* Create socket with no specific socket options, e.g. accepts CRC32, HMAC, XTEA, etc. if enabled during compilation */
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	/* Bind socket to all ports, e.g. all incoming connections will be handled here */
	csp_bind(sock, CSP_ANY);

	/* Create a backlog of 10 connections, i.e. up to 10 new connections can be queued */
	csp_listen(sock, 10);

	/* Wait for connections and then process packets on the connection */
	while (1) {
		numcheck++;
		if (numcheck == 50000000)
			numcheck = 0;
		//csp_sleep_ms(1);
		/* Wait for a new connection, 10000 mS timeout */
		csp_conn_t *conn;
		if ((conn = csp_accept(sock, 10000)) == NULL) {
			/* timeout */
			continue;
		}

		/* Read packets on connection, timout is 100 mS */
		csp_packet_t *packet;
		while ((packet = csp_read(conn, 100)) != NULL) {
			switch (csp_conn_dport(conn)) {
			case MY_SERVER_PORT:
				/* Process packet here */
				csp_log_info("Packet received on MY_SERVER_PORT: %s", (char* ) packet->data)
				;
				csp_buffer_free(packet);
				break;

			default:
				/* Call the default CSP service handler, handle pings, buffer use, etc. */
				csp_service_handler(conn, packet);
				break;
			}
		}
		/* Close current connection */
		csp_close(conn);

	}

	return CSP_TASK_RETURN;

}
/* End of server task */

/* Client task sending requests to server task */
CSP_DEFINE_TASK(task_client) {

	csp_log_info("Client task started");

	while (1) {

		csp_sleep_ms(1000);

		/* Send ping to server, timeout 1000 mS, ping size 100 bytes */
		result = csp_ping(5, 1000, 100, CSP_O_NONE);
		csp_log_info("Ping address: %u, result %d [mS]", 5, result);

		/* Send reboot request to server, the server has no actual implementation of csp_sys_reboot() and fails to reboot */
		//csp_reboot(29);
		//csp_log_info("reboot system request sent to address: %u", 29);
//
// 		/* Send data packet (string) to server */
//
// 		/* 1. Connect to host on 'server_address', port MY_SERVER_PORT with regular UDP-like protocol and 1000 ms timeout */
// 		csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, server_address, MY_SERVER_PORT, 1000, CSP_O_NONE);
// 		if (conn == NULL) {
// 			/* Connect failed */
// 			csp_log_error("Connection failed");
// 			return CSP_TASK_RETURN;
// 		}
//
// 		/* 2. Get packet buffer for message/data */
// 		csp_packet_t * packet = csp_buffer_get(100);
// 		if (packet == NULL) {
// 			/* Could not get buffer element */
// 			csp_log_error("Failed to get CSP buffer");
// 			return CSP_TASK_RETURN;
// 		}
//
// 		/* 3. Copy data to packet */
// 		snprintf((char *) packet->data, csp_buffer_data_size(), "Hello World (%u)", ++count);
//
// 		/* 4. Set packet length */
// 		packet->length = (strlen((char *) packet->data) + 1); /* include the 0 termination */
//
// 		/* 5. Send packet */
// 		if (!csp_send(conn, packet, 1000)) {
// 			/* Send failed */
// 			csp_log_error("Send failed");
// 			csp_buffer_free(packet);
// 		}
//
// 		/* 6. Close connection */
// 		csp_close(conn);
	}

	return CSP_TASK_RETURN;
}
/* End of client task */

int test_csp_server_task(unsigned int task_stack_size, unsigned int task_priority) {
	int ret = csp_thread_create(task_server, "SERVER", task_stack_size, NULL, task_priority, NULL);
	if (ret != 0) {
		csp_log_error("Failed to start server task, error: %d", ret);
		return ret;
	}
	return CSP_ERR_NONE;
}

int test_csp_client_task(unsigned int task_stack_size, unsigned int task_priority) {
	int ret = csp_thread_create(task_client, "CLIENT", task_stack_size, NULL, task_priority, NULL);
	if (ret != 0) {
		csp_log_error("Failed to start client task, error: %d", ret);
		return ret;
	}
	return CSP_ERR_NONE;
}

int test_csp_server_client_task(unsigned int task_stack_size, unsigned int task_priority) {
	int ret1 = csp_thread_create(task_server, "SERVER", task_stack_size, NULL, task_priority, NULL);
	if (ret1 != 0) {
		csp_log_error("Failed to start server task, error: %d", ret1);
		return ret1;
	}
	int ret2 = csp_thread_create(task_client, "CLIENT", task_stack_size, NULL, task_priority, NULL);
	if (ret2 != 0) {
		csp_log_error("Failed to start client task, error: %d", ret2);
		return ret2;
	}
	return CSP_ERR_NONE;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_CAN3_Init();
	/* USER CODE BEGIN 2 */
	//HAL_CAN_Start(&hcan3);
	/* USER CODE END 2 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	uint8_t address = 7;

	/* enable/disable debug levels */
	csp_debug_set_level(CSP_INFO, false);

	csp_log_info("Initialising CSP\r\n");
	/* Init CSP with address and default settings */
	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = address;
	int error = csp_init(&csp_conf);
	if (error != CSP_ERR_NONE) {
		csp_log_error("csp_init() failed, error: %d", error);
		exit(1);
	}

	csp_route_start_task(500, osPriorityRealtime);
	csp_iface_t *default_iface = NULL;

	/* Add interface(s) */
	//CAN
	csp_driver_can_init(INITIAL_VALUE, CSP_CAN_PROMISC, &default_iface);
	rtable = "0/0 CAN";
	if (rtable) {
		error = csp_rtable_load(rtable);
		if (error < 1) {
			csp_log_error("csp_rtable_load(%s) failed, error: %d", rtable, error);
			exit(1);
		}
	} else if (default_iface) {
		csp_rtable_set(CSP_DEFAULT_ROUTE, 0, default_iface, CSP_NO_VIA_ADDRESS);
	} else {
		/* no interfaces configured - run server and client in process, using loopback interface */
	}

	printf("Connection table\r\n");
	csp_conn_print_table();

	printf("Interfaces\r\n");
	csp_route_print_interfaces();

	printf("Route table\r\n");
	csp_route_print_table();
	//test_csp_server_client_task(500, osPriorityAboveNormal);
	test_csp_server_task(500, osPriorityAboveNormal);
	//test_csp_client_task(500, osPriorityAboveNormal);
	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 128;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief CAN3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN3_Init(void) {

	/* USER CODE BEGIN CAN3_Init 0 */

	/* USER CODE END CAN3_Init 0 */

	/* USER CODE BEGIN CAN3_Init 1 */

	/* USER CODE END CAN3_Init 1 */
	hcan3.Instance = CAN3;
	hcan3.Init.Prescaler = 4;
	hcan3.Init.Mode = CAN_MODE_NORMAL;
	hcan3.Init.SyncJumpWidth = CAN_SJW_2TQ;
	hcan3.Init.TimeSeg1 = CAN_BS1_5TQ;
	hcan3.Init.TimeSeg2 = CAN_BS2_2TQ;
	hcan3.Init.TimeTriggeredMode = DISABLE;
	hcan3.Init.AutoBusOff = DISABLE;
	hcan3.Init.AutoWakeUp = DISABLE;
	hcan3.Init.AutoRetransmission = DISABLE;
	hcan3.Init.ReceiveFifoLocked = DISABLE;
	hcan3.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN CAN3_Init 2 */

	/* USER CODE END CAN3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/* MPU Configuration */

void MPU_Config(void) {
	MPU_Region_InitTypeDef MPU_InitStruct = { 0 };

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected
	 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable (MPU_PRIVILEGED_DEFAULT);

}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
