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
#include "can.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "WHL500.h"
#include "KVH1750.h"
#include "kmCAN.h"
#include "CUBEMAG.h"
#include "OEM719.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t speed = 0;
adcs_rwa_data_t *RXbuff;
uint32_t rpm;
uint32_t rpm2;
uint32_t rpm3;
uint32_t rpm4;
uint8_t buffer[40];
float Mag[3];
//uint32_t time;
uint32_t old_time;
uint8_t check;

uint8_t Data_Buffer1[100];
uint8_t Data_Buffer2[100];
uint8_t Data_Buffer3[100];
float Roll = 0.0;
float roll = 0.0;
float Pitch = 0.0;
float pitch = 0.0;
float Yaw = 0.0;
float yaw = 0.0;
float Gyro[3];
float Accel[3];
int16_t Temp;
uint8_t Status;


CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef TxHeader;

newPortCAN CANWHL1(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL2(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL3(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL4(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANMTM(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANGNSS(&hcan1, 0x11, CAN_RX_FIFO0);
//
OEM719 GNSS(&CANGNSS,0x11,0x06);
kmCAN MTM(&CANMTM, 0x01);
CUBEMAG CM(&MTM);

newPortCAN test(&hcan3, 0x01, CAN_RX_FIFO0);
newPortCAN send(&hcan1,0x03,CAN_RX_FIFO0);

WHLREG whl1reg;
WHLREG whl2reg;
WHLREG whl3reg;
WHLREG whl4reg;

WHL500 whl1(&CANWHL1,&whl1reg,0x01);
WHL500 whl2(&CANWHL2,&whl2reg,0x01);
WHL500 whl3(&CANWHL3,&whl3reg,0x01);
WHL500 whl4(&CANWHL4,&whl4reg,0x01);
//WHL500 Test(&test,0x01);

void IMUData();
uint8_t CubemagCheck();
void MagCpy();
void timeloop(void (*function)(),uint16_t Ms);
void CAN_Transmit(uint8_t data[], uint16_t size);


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	IMUData();
//}

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
  MX_USART2_UART_Init();
  MX_UART5_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_CAN3_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  kvh1750_reg_setup();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);


  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
 // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);



  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);


  HAL_CAN_ActivateNotification(&hcan1,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF);
  HAL_CAN_ActivateNotification(&hcan2,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF);
  HAL_CAN_ActivateNotification(&hcan3,CAN_IT_TX_MAILBOX_EMPTY|CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_BUSOFF);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_Start(&hcan2);
  HAL_CAN_Start(&hcan3);



  CANWHL1.set_filter(CAN_RX_FIFO0);
  CANWHL1.set_rxtx_head(whl1.getCANRxHeader(), whl1.getCANTxHeader());

  CANWHL2.set_filter(CAN_RX_FIFO0);
  CANWHL2.set_rxtx_head(whl2.getCANRxHeader(), whl2.getCANTxHeader());

  CANWHL3.set_filter(CAN_RX_FIFO0);
  CANWHL3.set_rxtx_head(whl3.getCANRxHeader(), whl3.getCANTxHeader());

  CANWHL4.set_filter(CAN_RX_FIFO0);
  CANWHL4.set_rxtx_head(whl4.getCANRxHeader(), whl4.getCANTxHeader());

  CANGNSS.set_filter(CAN_RX_FIFO0);

  whl1.SetDeviceIDfilter(7);
  whl2.SetDeviceIDfilter(4);
  whl3.SetDeviceIDfilter(20);
  whl4.SetDeviceIDfilter(18);

  whl1.RegSetup();
  whl2.RegSetup();
  whl3.RegSetup();
  whl4.RegSetup();
  //whl500_reg_setup();
  cubemag_reg_setup();
  oem719_reg_setup();
  whl1.SetMessageParam(PRIOR_HIGH,2, PARAMETER_SERVICE, 35, 0x8B);
  whl2.SetMessageParam(PRIOR_HIGH,4, PARAMETER_SERVICE, 35, 0x8B);
  whl3.SetMessageParam(PRIOR_HIGH,5, PARAMETER_SERVICE, 35, 0x8B);
  whl4.SetMessageParam(PRIOR_HIGH,6, PARAMETER_SERVICE, 35, 0x8B);
////for(int i= 0; i < 73; i++)
//{
  whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl1.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 000000);
//
  whl2.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl2.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 00000);
//
  whl3.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl3.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 000000);

  whl4.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl4.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl4.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 00000);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_Delay(9000);
//  MTM.request_data(0x34, PriMagMeasurement);

//  MTM.request_data(0x34, 0xC6);
//  HAL_Delay(1000);
//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
 // Test.Ping(5);
//  HAL_UART_Receive_IT(&huart2, buffer, 36);
//  TxHeader.ExtId = GNSS.encode_id(0x01, 0x06, 0x11, 0x5D);
//  TxHeader.RTR = CAN_RTR_DATA;
//  TxHeader.IDE = CAN_ID_EXT;
//
//
//  CANGNSS.set_filter(CAN_RX_FIFO0);
//  CANGNSS.set_rxtx_head(&RxHeader, &TxHeader);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  MTM.request_data(0x34, PriMagMeasurement);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  whl1.SendCommand(GET, mrpm);
	  HAL_Delay(20);
	  whl2.SendCommand(GET, mrpm);
	  HAL_Delay(20);
	  whl3.SendCommand(GET, mrpm);
	  HAL_Delay(20);
	  whl4.SendCommand(GET, mrpm);
	  HAL_Delay(20);

	  MTM.request_data(0x34, PriMagMeasurement);
	  HAL_Delay(20);
//	  check = CM.DeviceCheck();
//	  IMUData();
//	  whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm*1000));
//	  whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm2*1000));
//	  whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm3*1000));
//	  whl4.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm4*1000));
	//  HAL_Delay(50);

//	  HAL_UART_Transmit(huart, pData, Size, Timeout)
//	  HAL_UART_Transmit(&huart2, TEST, sizeof(TEST), 1000);

	  IMUData();



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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void CAN_Transmit(uint8_t data[], uint16_t size)
{

	uint32_t TxMailBox;

	uint16_t numFrames = (size + 8 - 1) / 8;
	TxHeader.ExtId = 0x0142F034;

	for (uint16_t frameIndex = 0; frameIndex < numFrames; frameIndex++)
	{


	    if (frameIndex == (numFrames - 1))
	    {
	      TxHeader.DLC = size % 8;
	      if (TxHeader.DLC == 0)
	      {
	        TxHeader.DLC = 8;
	      }
	    }



	    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (data + (frameIndex * 8)), &TxMailBox) != HAL_OK)
	    {
	      Error_Handler();
	    }

	    HAL_Delay(1);
	  //  for (int i = 0; i<700; i++){}
	   // while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {

	   // }
	  }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//	whl1.RxMessage();

		uint8_t RxMsg[8];
//		static uint8_t RxIndex1 = 0;
		static uint8_t RxIndex2 = 0;
		static uint8_t RxIndex3 = 0;

		if (hcan->Instance == CAN1)
		{

//			CM.RxMessage();
//			whl1.RxMessage();
			GNSS.RxMessage();
			CM.RxMessage();
			whl1.RxMessage();
			whl2.RxMessage();
			whl3.RxMessage();
			whl4.RxMessage();

//			if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxMsg) != HAL_OK)
//			{
//				Error_Handler();
//			}
//
//				memcpy(Data_Buffer1+RxIndex1,RxMsg,8);
//				RxIndex1 += 8;

		}

		if (hcan->Instance == CAN2)
		{
			if(HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, RxMsg) != HAL_OK)
			{
				Error_Handler();
			}

			memcpy(Data_Buffer2+RxIndex2,RxMsg,8);
			RxIndex2 += 8;
		}

		if (hcan->Instance == CAN3)
		{
			if(HAL_CAN_GetRxMessage(&hcan3, CAN_RX_FIFO0, &RxHeader, RxMsg) != HAL_OK)
			{
				Error_Handler();
			}

			memcpy(Data_Buffer3+RxIndex3,RxMsg,8);
			RxIndex3 += 8;
		}
		/*
		 * incoming new message
		 * clear buffer
		 */


		// copy data to buffer
//			memcpy(Data_Buffer+RxIndex,RxMsg,8);
//			RxIndex += 8;



}



void IMUData()
{

	  HAL_UART_Receive(&huart2, buffer, 36,100);
	  BigEndianCpy((uint8_t *)Gyro,12,buffer+4);
	  BigEndianCpy((uint8_t *)Accel,12,buffer+16);
	  BigEndianCpy((uint8_t *)&Temp,2,buffer+30);
	  Status = buffer[28];


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
