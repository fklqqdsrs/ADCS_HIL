/* WARNING! All changes made to this file will be lost! */

#ifndef W_INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF
#define W_INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF

#define GIT_REV "v1.6-8-g53d740a"
#define CSP_FREERTOS 1

#define CSP_HAVE_LIBSOCKETCAN 0
#define CSP_HAVE_LIBZMQ 0
#define HAVE_PYTHON3 0
#define CSP_DEBUG 0
#define CSP_DEBUG_TIMESTAMP 0
#define CSP_USE_RDP 1
#define CSP_USE_RDP_FAST_CLOSE 0
#define CSP_USE_CRC32 1
#define CSP_USE_HMAC 1
#define CSP_USE_XTEA 1
#define CSP_USE_PROMISC 1
#define CSP_USE_QOS 0
#define CSP_USE_DEDUP 1
#define CSP_USE_EXTERNAL_DEBUG 0
#define CSP_LOG_LEVEL_DEBUG 1
#define CSP_LOG_LEVEL_INFO 1
#define CSP_LOG_LEVEL_WARN 1
#define CSP_LOG_LEVEL_ERROR 1
#define CSP_LITTLE_ENDIAN 1
/* #undef CSP_BIG_ENDIAN */
#define LIBCSP_VERSION "1.6"

//routing
#define RTABLE_STATIC 0
#define RTABLE_CIDR 1

//for STM32
#define CSP_HAVE_CAN 1
#define CAN_TASK 0
#define CSP_HAVE_USART 0
#define CSP_HAVE_I2C 0

#define BOARD_stm32g4xx_hal 1
#define BOARD_stm32f7xx_hal 2
#define BOARD_stm32h7xx_hal 3
#define BOARD_stm32flxx_hal 4
#define BOARD_stm32l4xx_hal 5
#define BOARD_stm32l767_hal 6

#if defined(STM32F722xx)
#define CONFIG_STM32_HAL "stm32f7xx_hal.h"
#define CSP_SELECT_BOARD BOARD_stm32f7xx_hal
#define CONFIG_CSP_CAN hcan1
//#define CONFIG_CSP_I2C hi2c1
#define CONFIG_CSP_UART huart6
#define CONFIG_CSP_PRINT huart3

#elif defined(STM32L496xx)
#define CONFIG_STM32_HAL "stm32l4xx_hal.h"
#define CSP_SELECT_BOARD BOARD_stm32l4xx_hal
#define CONFIG_CSP_CAN hcan1
#define CONFIG_CSP_I2C hi2c1
#define CONFIG_CSP_UART huart3
#define CONFIG_CSP_RADIO huart2
#define CONFIG_CSP_PRINT hlpuart1


#elif defined(STM32H723xx)
    #define CONFIG_STM32_HAL "stm32h7xx_hal.h"
    #define CSP_SELECT_BOARD BOARD_stm32h7xx_hal
//#define CONFIG_CSP_CAN hcan1
//#define CONFIG_CSP_I2C hi2c1
//#define CONFIG_CSP_UART huart3   ยังไม่ได้เเปลี่ยน
#define CONFIG_CSP_PRINT huart3


#elif defined(STM32G431xx)
#define CONFIG_STM32_HAL "stm32g4xx_hal.h"
#define CSP_SELECT_BOARD BOARD_stm32g4xx_hal
#define CONFIG_CSP_CAN hfdcan1
//#define CONFIG_CSP_I2C hi2c1
//#define CONFIG_CSP_UART huart3   ยังไม่ได้เเปลี่ยน
#define CONFIG_CSP_PRINT huart3

#elif defined(STM32F103xB)
    #define CONFIG_STM32_HAL "stm32f1xx_hal.h"
    #define CSP_SELECT_BOARD BOARD_stm32flxx_hal
	#define CONFIG_CSP_CAN hcan
//#define CONFIG_CSP_I2C hi2c1
	#define CONFIG_CSP_UART huart2


#elif defined(STM32F767xx)  //ADCS
#define CONFIG_STM32_HAL "stm32f7xx_hal.h"
#define CSP_SELECT_BOARD BOARD_stm32l767_hal
#define CONFIG_CSP_CAN hcan3
//#define CONFIG_CSP_I2C hi2c1
//#define CONFIG_CSP_UART huart2
#define CONFIG_CSP_PRINT huart5
#else
    #error "Unknown processor"
#endif

#include <cmsis_os.h>
#define CMSIS_V1 0x10002
#define CMSIS_V2 0x20001U
#define CMSIS_VERSION_MAIN (osCMSIS >> 16)
#if osCMSIS == 0x10002
#define OS_VERSION CMSIS_V1   ///< API version (main [31:16] .sub [15:0])
#elif osCMSIS == 0x20001U
#define OS_VERSION CMSIS_V2   ///< API version (main[31:16].sub[15:0])
#else
    #define OS_VERSION CSP_THREAD
#endif

#endif /* W_INCLUDE_CSP_CSP_AUTOCONFIG_H_WAF */
