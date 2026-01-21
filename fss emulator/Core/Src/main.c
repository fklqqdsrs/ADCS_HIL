/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define FEND 0xC0
#include "core_cm7.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum  nsp_command_code
{
	ping = 0,
	init = 1,
	peek = 2,
	poke = 3,
	telemetry = 4,
	crc = 6,
	app_telemetry = 7,
	app_command = 8,

};

enum  command_ack_byte
{
	ping_ack = 0xA0,
	init_ack = 0xA1,
	peek_ack = 0xA2,
	poke_ack = 0xA3,
	telemetry_ack = 0xA4,
	crc_ack = 0xA6,
	app_telemetry_ack = 0xA7,
	app_command_ack = 0xA8,

};

enum  fss_memory_map
{
	peek_flash_start = 0x00000000,
	flash_start = 0x00001000,
	special_test_mode = 0x00002000,
	flash_end	= 0x00007DFF,
	xram_start  = 0x00010000,
	xram_end    = 0x000107FF,
	iram_start  = 0x00020000,
	iram_end    = 0x000200FF,
	sfr_start   = 0x00030080,
	sfr_end     = 0x000300FF,
};

enum telemetry_address
{

	last_reset_reason = 0,
	reset_count = 1,
	framing_error_count = 2,
	runt_packet_count = 3,
	oversize_packet_size = 4,
	bad_crc_count = 5,

};

enum  last_reset_code
{

	power_cycle = 0,
	realtime_clock_reset = 1,
	flash_memory_reset = 2,
    comparator0_reset = 3,
	watchdog_timer_reset = 4,
	missing_clock_reset = 5,
	external_reset_pin_reset = 6,
	init_reset = 7,
	application_triggered_reset = 8,

};

enum  image_buffer
{
	operating_image = 4,
	offline_image   = 5,

};

enum app_telemetry_channel
{

	go_code_tlm = 0x0,
	exposure_tlm = 0x1,
	brightness_tlm = 0x2,
	temperature = 0x3,
	vector_float_tlm = 0x6,
	iteration_tlm = 0x7,
	process_time_tlm = 0x8,
	vector_int_tlm = 0x9,
	diag_tlm = 0xD,
	celsius_temperature = 0xE,
	addr = 0xF,
	reply_addr = 0x10

};

typedef struct{

	uint8_t command_code : 5;
	uint8_t ack_bit : 1 ;
	uint8_t b_bit : 1;
	uint8_t poll_bit : 1;

}nsp_message_control_field;

typedef struct{
	uint8_t aec_enabled : 1;
	uint8_t exposure_enabled :1;
	uint8_t dark_subtract_enabled : 1;
	uint8_t readout_enabled : 1;
	uint8_t cpy_to_offline_enabled : 1;
	uint8_t cpy_from_offline_enabled : 1;
	uint8_t fit_enabled : 1;
	uint8_t reserved : 1;

}go_code;

typedef struct{
	uint8_t fend_start;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t mcf;
	uint32_t init_addr;
	uint16_t crc;
	uint8_t fend_end;

}init_t;

typedef struct __attribute__((packed)) {
	uint8_t fend_start;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t mcf;
	uint8_t tlm_byte;
	float sun_vector_x;
	float sun_vector_y;
	float sun_vector_z;
	uint8_t fit_quality;
	uint8_t geometry_quality;
	uint16_t crc;
	uint8_t fend_end;
} sun_vector_t;

typedef struct{

	uint8_t fend_start;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t mcf;
	uint8_t tlm_byte;
	uint16_t crc;
	uint8_t fend_end;

}rx_sun_vector_t;

typedef struct{

	uint8_t fend_start;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t mcf;
	uint8_t byte;
	uint8_t go_code_byte;
	uint16_t crc;
	uint8_t fend_end;

}rx_application_command_t;

typedef struct{

	uint8_t fend_start;
	uint8_t dest_addr;
	uint8_t src_addr;
	uint8_t mcf;
	uint16_t crc;
	uint8_t fend_end;

}tx_application_command_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
const uint8_t device_id = 0x39;
nsp_message_control_field message_control_t;
uint8_t RxBuffer[64];
uint8_t float_sun_vector[4] = {0x39,0x11,0xA7,0x06};
uint16_t rxLen;
uint8_t isInit = 0;
uint16_t RxCRC;
uint16_t calCRC;

uint64_t time_start;
uint64_t time_diff;
uint32_t over_head;
init_t rx_init_t;
init_t tx_init_t;
rx_sun_vector_t rx_sun_vector;
sun_vector_t tx_sun_vector;
rx_application_command_t rx_app_command;
tx_application_command_t tx_app_command;
uint8_t command_type = 0;
float vector[3];
uint8_t can_data[8];
uint8_t buffer[22];
CAN_RxHeaderTypeDef RxHeader;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

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

void nsp_message_control_field_decode(uint8_t message_control_field)
{
	message_control_t.command_code = message_control_field & 0x1F;
	message_control_t.ack_bit = (message_control_field >> 5) & 0x01;
	message_control_t.b_bit = (message_control_field >> 6) & 0x01;
	message_control_t.poll_bit = (message_control_field >> 7) & 0x01;
}

void DWT_Init(void) {
    // Enable DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Reset the cycle counter
    DWT->CYCCNT = 0;

    // Enable the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t DWT_GetClockCycle(void) {
    return DWT->CYCCNT;
}

void can_filter_config()
{

	CAN_FilterTypeDef canFilterConfig;

	canFilterConfig.FilterActivation = ENABLE;
	canFilterConfig.FilterBank = 0;  // Filter bank number
	canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  // Mask mode
	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 32-bit filter
	canFilterConfig.FilterIdHigh = 0x0000;
	canFilterConfig.FilterIdLow = 0x0000;
	canFilterConfig.FilterMaskIdHigh = 0x0000;
	canFilterConfig.FilterMaskIdLow = 0x0000;
	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // FIFO assignment
	canFilterConfig.SlaveStartFilterBank = 14; // For dual CAN (optional)

	if (HAL_CAN_ConfigFilter(&hcan1, &canFilterConfig) != HAL_OK) {
	    // Handle error
	}


}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	tx_init_t.fend_start = FEND;
	tx_init_t.src_addr = device_id;
	tx_init_t.mcf = 0xA1;
	tx_init_t.fend_end = FEND;

	tx_app_command.fend_start = FEND;
	tx_app_command.src_addr = device_id;
    tx_app_command.mcf = 0xA8;
	tx_app_command.fend_end = FEND;

	tx_sun_vector.fend_start = FEND;
	tx_sun_vector.src_addr = device_id;
	tx_sun_vector.mcf = 0xA7;
	tx_sun_vector.fit_quality = 0xFD;
	tx_sun_vector.geometry_quality = 0x00;
	tx_sun_vector.fend_end = FEND;

  /* USER CODE END 1 */

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
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  DWT_Init();
  HAL_CAN_Start(&hcan1);
  can_filter_config();
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxBuffer, sizeof(RxBuffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */




	  if(isInit == 1)
	  {
		    switch (command_type) {

		        case 2:
		            HAL_UART_Transmit(&huart2,(uint8_t*)&tx_sun_vector,22,1000 );
		            command_type = 0;
		            break;
		        case 3:
		            HAL_UART_Transmit(&huart2,(uint8_t *)&tx_app_command,sizeof(tx_app_command)-1,1000 );
		            command_type = 0;
		            break;
		        default:

		            break;
		    }

	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    if (huart->Instance == USART2)
    {


        rxLen = Size;
        memcpy((uint8_t*)&RxCRC,RxBuffer+(rxLen-3),2);
        calCRC = message_crc16(RxBuffer+1, Size-4);

        if((RxCRC == calCRC) && (RxBuffer[1] == device_id))
        {
			switch (RxBuffer[3])
			{
				case 0xA1:

					  memcpy((uint8_t*)&rx_init_t,RxBuffer,rxLen);
					  if(rx_init_t.init_addr == (uint32_t)flash_start)
					  {
						isInit = 1;
					  }
					  tx_init_t.dest_addr = rx_init_t.src_addr;
					  tx_init_t.init_addr = rx_init_t.init_addr;
					  tx_init_t.crc = message_crc16((uint8_t*)&tx_init_t+1, Size-4);
					  HAL_UART_Transmit(&huart2, (uint8_t*)&tx_init_t, sizeof(init_t)-1, 1000);


					  break;

				case 0xA7:

					  memcpy((uint8_t*)&rx_sun_vector,RxBuffer,rxLen);
					  tx_sun_vector.dest_addr = rx_sun_vector.src_addr;
					  if(rx_sun_vector.tlm_byte == 0x06)
					  {
						  tx_sun_vector.tlm_byte = rx_sun_vector.tlm_byte;
						  tx_sun_vector.sun_vector_x = vector[0];
						  tx_sun_vector.sun_vector_y = vector[1];
						  tx_sun_vector.sun_vector_z = vector[2];
						  tx_sun_vector.crc = message_crc16((uint8_t*)&tx_sun_vector+1, 16);
						  if(isInit == 1)
						  {

						  	  command_type = 2;
						  }
					  }



					  break;

				case 0xA8:

					  memcpy((uint8_t*)&rx_app_command,RxBuffer,rxLen);
					  tx_app_command.dest_addr = rx_app_command.src_addr;
					  tx_app_command.crc = message_crc16((uint8_t*)&tx_app_command+1, 3);
					  if(isInit == 1)
					  {
					  	  command_type = 3;
					  }

					  break;


				default:

					  break;
			}

        }


        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, RxBuffer, sizeof(RxBuffer));
    }

}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, can_data) != HAL_OK)
	  {
		  Error_Handler();

	  }

	  if(RxHeader.ExtId == 0x102)
	  {

		  memcpy((uint8_t*)vector,can_data,8);

	  }
	  else if(RxHeader.ExtId == 0x103)
	  {
		  memcpy((uint8_t*)vector+8,can_data,4);
	  }

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
