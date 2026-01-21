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
#include "i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "General.h"
#include "WHL500.h"
#include "KVH1750.h"
#include "kmCAN.h"
#include "CUBEMAG.h"
#include "OEM719.h"
#include "newuart.h"
#include "I2C/newPortI2C.h"
#include "INA219/newINA.h"
#include "STSPIN/newMTQ.h"
#include "MTQpin.h"
#include "MTQ_header.h"
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
unsigned long crc;
//uint32_t time;
uint32_t speed = 0;
//adcs_rwa_data_t *RXbuff;
uint32_t rpm;
uint32_t rpm2;
uint32_t rpm3;
uint32_t rpm4;
uint8_t buffer[40];
float Mag[3];
//uint32_t time;
uint32_t old_time;
uint8_t check;
uint8_t hello[] = {"Start MCU!!"};
uint8_t loop[]  = {"in loop"};
uint8_t interrupt[] = {"interrupt"};
uint8_t Bestpos[] = {0xAA, 0x44, 0x12, 0x1C, 0x01, 0x00, 0x00, 0xC0,
					 0x20, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0xA0, 0x1F, 0x00, 0x00,
					 0x2A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0xD3, 0xF5, 0x10, 0x9A };

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

NEWUART Serial5(&huart5,100);
newPortI2C I2C2BUS(&hi2c2, AS_MASTER);
newINA x_sensor(&I2C2BUS, (0x40<< 1));
newINA y_sensor(&I2C2BUS, (0x41<< 1));
newINA z_sensor(&I2C2BUS, (0x44<< 1));

newPortCAN CANWHL1(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL2(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL3(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANWHL4(&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANMTM (&hcan1, 0x01, CAN_RX_FIFO0);
newPortCAN CANGNSS(&hcan1, 0x11, CAN_RX_FIFO0);

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

NEWMTQ MTQx(&mtq_driver_x,&x_sensor,&mtq_profile_x);
NEWMTQ MTQy(&mtq_driver_y,&y_sensor,&mtq_profile_y);
NEWMTQ MTQz(&mtq_driver_z,&z_sensor,&mtq_profile_z);

void IMUData();
uint8_t CubemagCheck();
void MagCpy();
void timeloop(void (*function)(),uint16_t Ms);
void CAN_Transmit(uint8_t data[], uint16_t size);

void show_currentMTQ(NEWMTQ *p,int16_t *d) {
	//char buffer[100];
	static float Icur = 0;
	//Vbus = p->read_loadV();
	Icur = p->read_mA();
	*d = Icur;
	//P = p->read_Power();
	//Vsh = p->read_Vsh();
	HAL_Delay(50);

}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	IMUData();
//}
int16_t MTQ_currentx = 0;
int16_t MTQ_currenty = 0;
int16_t MTQ_currentz = 0;

int16_t cur_pwmx = 0;
int16_t cur_pwmy = 0;
int16_t cur_pwmz = 0;
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
  MX_UART5_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_CAN3_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

 // kvh1750_reg_setup();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);


  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(REG2_5V_EN_GPIO_Port, REG2_5V_EN_Pin, GPIO_PIN_SET);
 // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);



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
//
  whl1.SetDeviceIDfilter(7);
  whl2.SetDeviceIDfilter(4);
  whl3.SetDeviceIDfilter(20);
  whl4.SetDeviceIDfilter(18);

  whl1.RegSetup();
  whl2.RegSetup();
  whl3.RegSetup();
  whl4.RegSetup();
  //whl500_reg_setup();
//  cubemag_reg_setup();
//  oem719_reg_setup();
  whl1.SetMessageParam(PRIOR_HIGH,2, PARAMETER_SERVICE, 35, 0x8B);
  whl2.SetMessageParam(PRIOR_HIGH,4, PARAMETER_SERVICE, 35, 0x8B);
  whl3.SetMessageParam(PRIOR_HIGH,5, PARAMETER_SERVICE, 35, 0x8B);
  whl4.SetMessageParam(PRIOR_HIGH,6, PARAMETER_SERVICE, 35, 0x8B);
////for(int i= 0; i < 73; i++)
//{
  whl1.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl1.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 1000000);
////
  whl2.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl2.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 1500000);
////
  whl3.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl3.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 2000000);

  whl4.SendCommand(SET, pid_en,      PARAM_TYPE_UINT8,  1);
  whl4.SendCommand(SET, drv_enable,  PARAM_TYPE_UINT8,  1);
  whl4.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, 1000000);

  char buffer[70];
  Serial5.begin(115200);
//  HAL_UARTEx_ReceiveToIdle_DMA(Serial5.getHandleTypeDef(), rx_buf_u5,
//      			rx_size);
  Serial5.println("Hello");

  x_sensor.begin();
  y_sensor.begin();
  z_sensor.begin();

  mtq_driver_x.begin();
  mtq_driver_y.begin();
  mtq_driver_z.begin();
 // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);

//  CANGNSS.set_rxtx_head(&RxHeader, &TxHeader);
//  HAL_Delay(1000);
//  for(int i = 0; i < 5; i++)
//  {
//  GNSS.binary_log(CC0M2, bestXYZ, ONTIME, 0.05);
// GNSS.binary_log(CC0M2, bestPos, ONTIME, 0.05);
	//  GNSS.binary_log(CC0M2, bestPos, ONTIME, 0.05);
	//  GNSS.TxMessage(Bestpos, sizeof(Bestpos), &TxHeader);
  	//  CAN_Transmit(Bestpos, sizeof(Bestpos));
//  }

//  for(int i = 0; i < 5; i++)
//  {
//	  GNSS.binary_log(CC0M2, bestXYZ, ONCE, 0);
	//  GNSS.TxMessage(Bestpos, sizeof(Bestpos), &TxHeader);
  	//  CAN_Transmit(Bestpos, sizeof(Bestpos));
//  }
//
//
//  CANGNSS.set_filter(CAN_RX_FIFO0);
//  crc = GNSS.CalculateBlockCRC32((sizeof(Bestpos)-4), Bestpos, 1);

//  GNSS.binary_log(CC0M2, 0x2A, ONTIME, 1.0);
//  HAL_TIM_Base_Start_IT(&htim6);
//  HAL_UART_Transmit(&huart5,hello,sizeof(hello), 500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  MTM.request_data(0x34, PriMagMeasurement);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  HAL_UART_Transmit(&huart5,loop,sizeof(loop), 500);
//	  HAL_Delay(10000);
	  //__SEV();

//	  GNSS.binary_log(CC0M2, bestXYZ, ONCE, 0);
//	  GNSS.binary_log(CC0M2, bestPos, ONCE, 0);
//	  LoopDelay(500);
//	  GNSS.getBestPos();
//	  whl1.SendCommand(GET, mrpm);
//	  HAL_Delay(20);
//	  whl2.SendCommand(GET, mrpm);
//	  HAL_Delay(20);
//	  whl3.SendCommand(GET, mrpm);
//	  HAL_Delay(20);
//	  whl4.SendCommand(GET, mrpm);
//	  HAL_Delay(20);
//
//	  MTM.request_data(0x34, PriMagMeasurement);
//	  HAL_Delay(20);
//	  check = CM.DeviceCheck();
	  //IMUData();
//	  whl1.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm*1000));
//	  whl2.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm2*1000));
//	  whl3.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm3*1000));
//	  whl4.SendCommand(SET, mrpm_target, PARAM_TYPE_UINT32, (rpm4*1000));
	//  HAL_Delay(50);

//	  HAL_UART_Transmit(huart, pData, Size, Timeout)
//	  HAL_UART_Transmit(&huart2, TEST, sizeof(TEST), 1000);

	 // IMUData();
	  show_currentMTQ(&MTQx,&MTQ_currentx);
	  show_currentMTQ(&MTQy,&MTQ_currenty);
	  show_currentMTQ(&MTQz,&MTQ_currentz);
	  sprintf(buffer,"MTQ Current X Y Z : %d, %d, %d\r\n", MTQ_currentx,MTQ_currenty,MTQ_currentz);
	  Serial5.print(buffer, 0);
	  MTQx.run_raw_pwm(cur_pwmx,65534);
	  MTQy.run_raw_pwm(cur_pwmy,65534);
	  MTQz.run_raw_pwm(cur_pwmz,65534);
	  HAL_Delay(200);
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

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
	TxHeader.ExtId = 0x4061C11;

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
//			GNSS.RxMessage();
//			GNSS.getBestXYZ();
//			CM.RxMessage();
//			whl1.RxMessage();
//			whl2.RxMessage();
//			whl3.RxMessage();
//			whl4.RxMessage();

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

		//	memcpy(Data_Buffer2+RxIndex2,RxMsg,8);
			RxIndex2 += 8;
		}

		if (hcan->Instance == CAN3)
		{
			if(HAL_CAN_GetRxMessage(&hcan3, CAN_RX_FIFO0, &RxHeader, RxMsg) != HAL_OK)
			{
				Error_Handler();
			}

			//memcpy(Data_Buffer3+RxIndex3,RxMsg,8);
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {

	 // HAL_UART_Transmit(&huart5,interrupt,sizeof(interrupt), 500);
	  __SEV();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void IMUData()
{

	  HAL_UART_Receive(&huart2, buffer, 36,100);
	  BigEndianCpy((uint8_t *)Gyro,12,buffer+4);
//	  BigEndianCpy((uint8_t *)Accel,12,buffer+16);
//	  BigEndianCpy((uint8_t *)&Temp,2,buffer+30);
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
