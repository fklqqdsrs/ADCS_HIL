/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
<<<<<<< HEAD
  * Copyright (c) 2026 STMicroelectronics.
=======
  * Copyright (c) 2024 STMicroelectronics.
>>>>>>> 9c4df88b5fc8b034c395cb50010cc883288ff277
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
<<<<<<< HEAD
#include "stm32l4xx_hal.h"
=======
#include "stm32f7xx_hal.h"
>>>>>>> 9c4df88b5fc8b034c395cb50010cc883288ff277

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
<<<<<<< HEAD
=======
#define MTQ2_VREF_Pin GPIO_PIN_1
#define MTQ2_VREF_GPIO_Port GPIOA
#define MTQ3_VREF_Pin GPIO_PIN_2
#define MTQ3_VREF_GPIO_Port GPIOA
#define MTQ1_DIR_Pin GPIO_PIN_0
#define MTQ1_DIR_GPIO_Port GPIOB
#define MTQ1_EN_Pin GPIO_PIN_1
#define MTQ1_EN_GPIO_Port GPIOB
#define MTQ1_STBY_Pin GPIO_PIN_2
#define MTQ1_STBY_GPIO_Port GPIOB
#define MTQ2_DIR_Pin GPIO_PIN_13
#define MTQ2_DIR_GPIO_Port GPIOF
#define MTQ2_EN_Pin GPIO_PIN_14
#define MTQ2_EN_GPIO_Port GPIOF
#define MTQ2_STBY_Pin GPIO_PIN_15
#define MTQ2_STBY_GPIO_Port GPIOF
#define MTQ2_PWM_Pin GPIO_PIN_11
#define MTQ2_PWM_GPIO_Port GPIOE
#define MTQ3_PWM_Pin GPIO_PIN_13
#define MTQ3_PWM_GPIO_Port GPIOE
#define MTQ1_PWM_Pin GPIO_PIN_14
#define MTQ1_PWM_GPIO_Port GPIOE
#define MTQ1_VREF_Pin GPIO_PIN_11
#define MTQ1_VREF_GPIO_Port GPIOB
#define MTQ3_DIR_Pin GPIO_PIN_2
#define MTQ3_DIR_GPIO_Port GPIOG
#define MTQ3_EN_Pin GPIO_PIN_3
#define MTQ3_EN_GPIO_Port GPIOG
#define MTQ3_STBY_Pin GPIO_PIN_4
#define MTQ3_STBY_GPIO_Port GPIOG
#define REG2_5V_EN_Pin GPIO_PIN_3
#define REG2_5V_EN_GPIO_Port GPIOB
>>>>>>> 9c4df88b5fc8b034c395cb50010cc883288ff277

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
