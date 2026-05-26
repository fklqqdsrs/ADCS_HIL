//! @file hw_i2c.c
//! @brief HAL I2C callbacks that bridge to CSP I2C glue.


#define __THIS_FILE_NAME__   "hw_i2c.c"

#include <kservice.h>
#include "hw_i2c.h"
#include "main.h"

/* Only compile this entire file if I2C is enabled */
#if ENABLE_HW_I2C
// CSP STM32 I2C glue callbacks (provided in csp_i2c_stm32.c)
extern void csp_i2c_stm32_master_tx_callback(I2C_HandleTypeDef *hi2c, BaseType_t *pxTaskWoken);
extern void csp_i2c_stm32_listen_callback   (I2C_HandleTypeDef *hi2c);
extern void csp_i2c_stm32_addr_callback     (I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
extern void csp_i2c_stm32_slave_rx_callback (I2C_HandleTypeDef *hi2c, BaseType_t *pxTaskWoken);
extern void csp_i2c_stm32_error_callback    (I2C_HandleTypeDef *hi2c, BaseType_t *pxTaskWoken);

/* -------------------------------------------------------------------------- */
/* HAL -> FreeRTOS ISR bridge                                                  */
/* -------------------------------------------------------------------------- */

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    csp_i2c_stm32_master_tx_callback(hi2c, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
    csp_i2c_stm32_listen_callback(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode) {
    csp_i2c_stm32_addr_callback(hi2c, TransferDirection, AddrMatchCode);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    csp_i2c_stm32_slave_rx_callback(hi2c, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    csp_i2c_stm32_error_callback(hi2c, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/* End of file */
#endif /* ENABLE_HW_I2C */
