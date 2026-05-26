/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "semaphore"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
//#include "RTOS_LIB/TASK_TSC1.h"
#include "adcs_command.h"
//#include "LIB/MAG_REG.h"

#define RX_BUFFER_SIZE 64

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */



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
/* USER CODE BEGIN Variables */

 // Queue to hold received messages


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t fssRxBuffer[RX_BUFFER_SIZE];
uint8_t processBuffer[RX_BUFFER_SIZE];
SemaphoreHandle_t uartRxSemaphore;
SemaphoreHandle_t xCANRxSemaphore;
CANExtId_Typedef rwa_id;
QueueHandle_t canRxQueue;
CAN_RxHeaderTypeDef canDeviceHandle;


void StartDefaultTask(void *argument);
uint8_t rwa_id_decode(uint32_t extid );
uint8_t fss_sun_msg_check(uint8_t *Msg,uint8_t id);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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

  /* creation of defaultTask */

  init_device();
  oem719_reg_setup();
  reg_REG_TSC1_setup();
  GNSSInit();
  RWinit();
  CubemagInit();
  MTQinit();
  IMUInit();
  TASK_TSC1_init_task();
  FSS_Setup();
 // STT_Setup();

  mutex_csp_init();


  HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO1_MSG_PENDING);
  HAL_UART_Receive_DMA(&huart2, uartRxBuffer, sizeof(uartRxBuffer));
  HAL_UARTEx_ReceiveToIdle_DMA(&huart6, fssRxBuffer, RX_BUFFER_SIZE);
 // HAL_UART_Receive_DMA(&huart6, fssRxBuffer, sizeof(fssRxBuffer));
  canRxQueue = xQueueCreate(100,sizeof(canRxHandle_t));
  uartRxSemaphore = xSemaphoreCreateBinary();
  xCANRxSemaphore = xSemaphoreCreateBinary();

  
  //defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	if (hcan->Instance == CAN1)
	{
//		if(GNSS.RxMessage() == 1)
//		{
//			return;
//		}
//
//		else if(whl1.RxMessage() == 1 )
//		{
//			return;
//		}
//
//		else if(whl2.RxMessage() == 1)
//		{
//			return;
//		}
//
//		else if(whl3.RxMessage() == 1)
//		{
//			return;
//		}
//
//		else if(whl4.RxMessage() == 1)
//		{
//			return;
//		}



//		whl1.RxMessage();
//		whl2.RxMessage();
//			whl3.RxMessage();
//		whl4.RxMessage();


	   uint8_t rxData[8];
	   canRxHandle_t canMsg;

	    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &canDeviceHandle, rxData) == HAL_OK) {

//	    	if(GNSS.deviceCheck(&canDeviceHandle) == 1)
//	    	{
//	    		GNSS.RxMessage(rxData, canDeviceHandle.DLC);
//	    	}
//	    	else{
				canMsg.device_id = rwa_id_decode(canDeviceHandle.ExtId);
				canMsg.id = canDeviceHandle.ExtId;
				canMsg.dlc = canDeviceHandle.DLC;

				memcpy(canMsg.data, rxData, canDeviceHandle.DLC);


				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(canRxQueue, &canMsg, &xHigherPriorityTaskWoken);


				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

//	    	}
	    }
	}

	if (hcan->Instance == CAN2)
	{

//

	}


}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;


        xSemaphoreGiveFromISR(uartRxSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//        HAL_UART_Receive_DMA(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);  // Restart DM

        // Restart UART reception
//   	KVH1750.IMUGyro();



    }

//    if (huart->Instance == USART6)
//    {
//    	HAL_UART_Receive_DMA(&huart6, fssRxBuffer, sizeof(fssRxBuffer));
//    }
}

uint8_t fss_sun_msg_check(uint8_t *Msg,uint8_t id)
{


	if ((Msg[0] == 0xC0 ) && (Msg[1] == 0x11) && (Msg[2]  == id))
	{

		return 1;
	}
	return 0;

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART6)
    {
 //       memcpy(processBuffer, fssRxBuffer, Size);



        HAL_UARTEx_ReceiveToIdle_DMA(&huart6, fssRxBuffer, RX_BUFFER_SIZE);
        if(fssRxBuffer[2] == 0x39)
        {
        	fss_ram_data.ID = 0x39;
        }
       // memcpy(&fss_ram_data.vector,fssRxBuffer+2,2);
    }
}


uint8_t rwa_id_decode(uint32_t extid )
{
	rwa_id.source_id = (extid >> 24) & 0x1F;


	return rwa_id.source_id;
}



/* USER CODE END Application */

