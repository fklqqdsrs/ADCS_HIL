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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

CAN_TxHeaderTypeDef cubeTxHeader;
CAN_RxHeaderTypeDef cubeRxHeader;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t device_id = 0x34;
uint8_t can_data[8];
uint8_t can_mag_data[8];

uint8_t cube_msg[15] = {0,0,0,0,0,0,0,1,0,0,0,0,0,1,0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x2007c000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x2007c0a0
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x2007c000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2007c0a0))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */
#endif

ETH_TxPacketConfig TxConfig;

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

ETH_HandleTypeDef heth;

/* USER CODE BEGIN PV */


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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_ETH_Init(void);
/* USER CODE BEGIN PFP */

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


void CAN_Filters_Config(void)
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

	  CAN_FilterTypeDef sFilterConfig;

	  sFilterConfig.FilterBank           = 0;
	  sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
	  sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;


	  sFilterConfig.FilterIdHigh         = 0x0000;
	  sFilterConfig.FilterIdLow          = 0x0004;
	  sFilterConfig.FilterMaskIdHigh     = 0xFFFF;
	  sFilterConfig.FilterMaskIdLow      = 0xFFC4;

	  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	  sFilterConfig.FilterActivation     = ENABLE;
	  sFilterConfig.SlaveStartFilterBank = 14;

	  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

  /* ----------------- กำหนดค่า Filter สำหรับ CAN2 (รับเข้า FIFO1) ----------------- */


//  sFilterConfig.FilterBank           = 14;                    // ใช้ Filter Bank 14 (สำหรับ CAN2)
//  sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
//  sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
//  sFilterConfig.FilterIdHigh         = 0x0000;
//  sFilterConfig.FilterIdLow          = 0x0000;
//  sFilterConfig.FilterMaskIdHigh     = 0x0000;
//  sFilterConfig.FilterMaskIdLow      = 0x0000;
//  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;
//  sFilterConfig.FilterActivation     = ENABLE;
//
//  if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
//  {
//
//    Error_Handler();
//  }

	  sFilterConfig.FilterBank        =  14;
	  sFilterConfig.FilterMode       = CAN_FILTERMODE_IDMASK; // << โหมด Mask
	   sFilterConfig.FilterScale      = CAN_FILTERSCALE_32BIT;

	   sFilterConfig.FilterIdHigh     = 0x2628;
	   sFilterConfig.FilterIdLow      = 0x09A4;

	   sFilterConfig.FilterMaskIdHigh = 0xFFFF;
	   sFilterConfig.FilterMaskIdLow  = 0xFFFE;

	   sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;
	   sFilterConfig.FilterActivation     = ENABLE;


	   if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	   {
	     Error_Handler();
	   }

}


void can_tx_multiframe(CAN_TxHeaderTypeDef txheader, uint8_t *data, uint8_t length)
{
	uint32_t mailbox;
	if (length <= 8)
	{
		txheader.DLC = length;
		HAL_CAN_AddTxMessage(&hcan2, &txheader, data, &mailbox);
	}
	else
	{

		uint8_t num_frame = (length + 7) / 8;

		for (int i = 0; i < num_frame; i++)
		{

			if (i == num_frame - 1)
			{

				txheader.DLC = (length % 8 == 0) ? 8 : (length % 8);
				HAL_CAN_AddTxMessage(&hcan2, &txheader, data + (8 * i), &mailbox);
			}
			else
			{

				txheader.DLC = 8;
				HAL_CAN_AddTxMessage(&hcan2, &txheader, data + (8 * i), &mailbox);
			}
		}
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


	cubeTxHeader.RTR =  CAN_RTR_DATA;
	cubeTxHeader.IDE = CAN_ID_EXT;


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
  MX_CAN2_Init();
  MX_ETH_Init();
  /* USER CODE BEGIN 2 */
  HAL_CAN_Start(&hcan1);
  HAL_CAN_Start(&hcan2);

  CAN_Filters_Config();
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 6;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//
	  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, can_data) != HAL_OK)
	  {
		  Error_Handler();

	  }

	  if(RxHeader.ExtId == 0x1)
	  {
		  memcpy(cube_msg,can_data,7);
		  cube_msg[8] = can_data[7];

	  }
	  else if(RxHeader.ExtId == 0x2)
	  {
		  memcpy(cube_msg+9,can_data,4);
	  }

}


void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//

	  if (HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &cubeRxHeader, can_mag_data) != HAL_OK)
	  {
		  Error_Handler();

	  }

	  decode_id(cubeRxHeader.ExtId, &ext_id);

	  if(ext_id.dest_addr == device_id)
	  {
		  if(ext_id.msgtype == TLM_req)
		  {
			  if(ext_id.reg_id == 0xC5)
			  {
				  cubeTxHeader.ExtId = encode_id(TLM_EXT, 0XC5, device_id, ext_id.src_addr);
				  can_tx_multiframe(cubeTxHeader, cube_msg, sizeof(cube_msg));
			  }
		  }

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
