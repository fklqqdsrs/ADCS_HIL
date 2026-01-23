/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
//HAL_StatusTypeDef CAN_Send_Multi_Bytes(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *tx_head, uint8_t *pData, uint16_t len);
void CAN_Filter(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef can_tx_multiframe(CAN_HandleTypeDef *hcan,
                                   const CAN_TxHeaderTypeDef *txheader_in,
                                   const uint8_t *data,
                                   uint16_t length,
                                   uint32_t timeout_ms);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

