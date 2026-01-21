/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAG1_INT_Pin GPIO_PIN_2
#define MAG1_INT_GPIO_Port GPIOE
#define FRAM1_CS_Pin GPIO_PIN_4
#define FRAM1_CS_GPIO_Port GPIOC
#define CAN2_STBY_Pin GPIO_PIN_14
#define CAN2_STBY_GPIO_Port GPIOB
#define MAG2_INT_Pin GPIO_PIN_10
#define MAG2_INT_GPIO_Port GPIOC
#define IMU1_INT1_Pin GPIO_PIN_3
#define IMU1_INT1_GPIO_Port GPIOD
#define CAN1_STBY_Pin GPIO_PIN_4
#define CAN1_STBY_GPIO_Port GPIOD
#define IMU1_INT2_Pin GPIO_PIN_5
#define IMU1_INT2_GPIO_Port GPIOD
#define IMU2_INT1_Pin GPIO_PIN_6
#define IMU2_INT1_GPIO_Port GPIOD
#define IMU2_INT2_Pin GPIO_PIN_7
#define IMU2_INT2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
