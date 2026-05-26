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
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
CAN_RxHeaderTypeDef RxHeader;
uint8_t can_data[8];
uint8_t device_id = 0x34;
uint8_t can_mag_data[8];
float  mag[3];

uint8_t cube_msg[15] = {0,0,0,0,0,0,0,1,0,0,0,0,0,1,0};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

typedef enum
{
	TC_req = 0x01,
	TC_ack,
	TC_nack,
	TLM_req,
	TLM_res,
	TLM_nack,
	TC_EXT,
	TLM_EXT
} msgtypes;


typedef struct
{
	uint8_t msgtype;
	uint8_t src_addr;
	uint8_t dest_addr;
	uint8_t reg_id;

}ident_t;

ident_t ext_id;

/* USER CODE BEGIN PV */




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void can_filter_config();
void reverse_bytes(uint8_t *src, uint8_t *dst, uint32_t len);
void decode_id(uint32_t can_id, ident_t *id);
uint32_t encode_id(uint8_t msg_type, uint8_t reg_id, uint8_t src_addr,
				   uint8_t dest_addr);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t start_time;
uint32_t stop_time;
uint8_t  send_flag = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */




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
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
 // HAL_TIM_Base_Start_IT(&htim7);
  HAL_CAN_Start(&hcan1);
  can_filter_config();
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  uint32_t lastTick = HAL_GetTick();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  start_time = HAL_GetTick();

      if ((start_time - lastTick) >= 20)
      {
          lastTick = start_time;


       if(send_flag == 1)
       {


		  send_flag = 0;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, can_data) != HAL_OK)
	  {
		  Error_Handler();

	  }

	  if(RxHeader.ExtId == 1)
	  {

//		  memcpy(cube_msg,can_data,7);
//		  cube_msg[8] = can_data[7];
		  memcpy((uint8_t *)&mag,can_data,8);

	  }
	  else if(RxHeader.ExtId == 2)
	  {
		  //memcpy(cube_msg+9,can_data,4);
		  memcpy((uint8_t *)&mag+8,can_data,4);
	  }

}





void can_filter_config()
{

//	CAN_FilterTypeDef canFilterConfig;
//
//	canFilterConfig.FilterActivation = ENABLE;
//	canFilterConfig.FilterBank = 0;  // Filter bank number
//	canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  // Mask mode
//	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 32-bit filter
//	canFilterConfig.FilterIdHigh = 0x0000;
//	canFilterConfig.FilterIdLow = 0x0000;
//	canFilterConfig.FilterMaskIdHigh = 0x0000;
//	canFilterConfig.FilterMaskIdLow = 0x0000;
//	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // FIFO assignment
//	canFilterConfig.SlaveStartFilterBank = 14; // For dual CAN (optional)
//
//	if (HAL_CAN_ConfigFilter(&hcan1, &canFilterConfig) != HAL_OK) {
//	    // Handle error
//	}
	CAN_FilterTypeDef filterConfig;
	uint32_t base_id, mask;

	// === Filter สำหรับ Extended ID 0–7 → FIFO0 ===
	base_id = 0x00000000;        // ID base 0
	mask    = 0x1FFFFFF8;        // Mask: จับเฉพาะ 3 บิตล่างของ 29-bit ID

	filterConfig.FilterBank = 0;
	filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterConfig.FilterActivation = ENABLE;

	filterConfig.FilterIdHigh     = ((base_id << 3) | (1 << 2)) >> 16;
	filterConfig.FilterIdLow      = ((base_id << 3) | (1 << 2)) & 0xFFFF;
	filterConfig.FilterMaskIdHigh = ((mask    << 3) | (1 << 2)) >> 16;
	filterConfig.FilterMaskIdLow  = ((mask    << 3) | (1 << 2)) & 0xFFFF;

	HAL_CAN_ConfigFilter(&hcan1, &filterConfig);

	// === Filter สำหรับ Extended ID 8–31 → FIFO1 ===
	base_id = 0x00000008;        // ID base 8 (จะ match 8–15 ด้วย mask เดิม)
	mask    = 0x1FFFFFF8;        // Same mask

	filterConfig.FilterBank = 1;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	filterConfig.FilterIdHigh     = ((base_id << 3) | (1 << 2)) >> 16;
	filterConfig.FilterIdLow      = ((base_id << 3) | (1 << 2)) & 0xFFFF;
	filterConfig.FilterMaskIdHigh = ((mask    << 3) | (1 << 2)) >> 16;
	filterConfig.FilterMaskIdLow  = ((mask    << 3) | (1 << 2)) & 0xFFFF;

	HAL_CAN_ConfigFilter(&hcan1, &filterConfig);



}

void reverse_bytes(uint8_t *src, uint8_t *dst, uint32_t len) {
    while (len >= 4) { // Process 4 bytes at a time
        dst[0] = src[len - 1];
        dst[1] = src[len - 2];
        dst[2] = src[len - 3];
        dst[3] = src[len - 4];
        dst += 4;
        len -= 4;
    }
    while (len--) { // Process remaining bytes
        *dst++ = *src--;
    }
}

uint32_t encode_id(uint8_t msg_type, uint8_t reg_id, uint8_t src_addr,
				   uint8_t dest_addr)
{
	uint32_t can_id = 0;
	can_id |= (msg_type & 0x1F) << 24; // First 5 bits
	can_id |= (reg_id & 0xFF) << 16;   // Next 8 bits
	can_id |= (src_addr & 0xFF) << 8;  // Next 8 bits
	can_id |= dest_addr & 0xFF;		   // Last 8 bits
	return can_id;
}

void decode_id(uint32_t can_id, ident_t *id)
{

	id->msgtype = (msgtypes)(can_id >> 24) & 0x1F; // First 5 bits
	id->reg_id = (can_id >> 16) & 0xFF;			  // Next 8 bits
	id->src_addr = (can_id >> 8) & 0xFF;			  // Next 8 bits
	id->dest_addr = can_id & 0xFF;				  // Last 8 bits
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
  while(1)
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
