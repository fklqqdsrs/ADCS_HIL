/*
 * ksdef.h
 *
 *  Created on: 11 May 2025
 *      Author: Pongpot
 */

#ifndef INC_KSDEF_H_
#define INC_KSDEF_H_

/**
 * @addtogroup Error
 */

/**@{*/

/* Error code definitions */
#define TSC_EOK                          0               /**< There is no error */
#define TSC_ERROR                        1               /**< A generic error happens */
#define TSC_ETIMEOUT                     2               /**< Timed out */
#define TSC_EFULL                        3               /**< The resource is full */
#define TSC_EEMPTY                       4               /**< The resource is empty */
#define TSC_ENOMEM                       5               /**< No memory */
#define TSC_ENOSYS                       6               /**< No system */
#define TSC_EBUSY                        7               /**< Busy */
#define TSC_EIO                          8               /**< IO error */
#define TSC_EINTR                        9               /**< Interrupted system call */
#define TSC_EINVAL                       10              /**< Invalid argument */

/* boolean type definitions */
#define TSC_TRUE                         1               /**< boolean true  */
#define TSC_FALSE                      	 0               /**< boolean fails */

/**@}*/

/**
 * @ingroup BasicDef
 *
 * @def TSC_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. TSC_ALIGN(13, 4)
 * would return 16.
 */
#define TSC_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def TSC_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. TSC_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define TSC_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define TSC_ASSERT(EX)                                                        \
if (!(EX))                                                                    \
{                                                                             \
    tsc_assert_handler(#EX, __FUNCTION__, __LINE__);                          \
}

/**
 * Double List structure
 */
struct tsc_list_node
{
    struct tsc_list_node *next;                          /**< point to next node. */
    struct tsc_list_node *prev;                          /**< point to prev node. */
};
typedef struct tsc_list_node tsc_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct tsc_slist_node
{
    struct tsc_slist_node *next;                         /**< point to next node. */
};
typedef struct tsc_slist_node tsc_slist_t;                /**< Type for single list. */

/* USER CODE BEGIN Private defines */
#define TSC_USING_HEAP
#define TSC_USING_FINSH
#define TSC_USING_CONSOLE
#define TSC_KSERVICE_USING_STDLIB
#define TSC_DEBUG

#define TSC_ALIGN_SIZE 4
#define TSC_ALIGN_SIZE_COPY 4

#define TSC_CONSOLEBUF_SIZE 1024
#define TSC_CONSOLEBUF_SIZE_COPY 1024

#define FINSH_USING_SYMTAB 1
#define FINSH_USING_DESCRIPTION
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 10
#define FINSH_CMD_SIZE 128


#define TSC_USING_TSC_LOG
#define TSC_NAME_MAX 16
#define TSC_LOG_OUTPUT_LVL_D
#define TSC_LOG_OUTPUT_LVL 7
#define TSC_LOG_ASSERT_ENABLE
#define TSC_LOG_LINE_BUF_SIZE 512
#define TSC_LOG_USING_ASYNC_OUTPUT
#define TSC_LOG_ASYNC_OUTPUT_BUF_SIZE 2048
#define TSC_LOG_ASYNC_OUTPUT_BY_THREAD
#define TSC_LOG_ASYNC_OUTPUT_THREAD_STACK 4096
#define TSC_LOG_ASYNC_OUTPUT_THREAD_PRIORITY 9

/* log format */
#define TSC_LOG_USING_COLOR
#define TSC_LOG_USING_ISR_LOG
#define TSC_LOG_USING_FILTER
#define TSC_LOG_OUTPUT_TIME
#define TSC_LOG_TIME_USING_TIMESTAMP
#define TSC_LOG_OUTPUT_LEVEL
#define TSC_LOG_OUTPUT_TAG
#define TSC_LOG_OUTPUT_THREAD_NAME
/* end of log format */
#define TSC_LOG_BACKEND_USING_CONSOLE
#define TSC_LOG_USING_FILTER

// #define TSC_LOG_BACKEND_USING_FILE
#define TSC_LOG_FILE_ROOT_PATH "/D/logs"
#define TSC_LOG_FILE_NAME_BASE "tsc_log.log"
#define TSC_LOG_FILE_MAX_NUM 10
#define TSC_LOG_FILE_MAX_SIZE 256

/* USER CODE END Private defines */

#endif /* INC_KSDEF_H_ */
