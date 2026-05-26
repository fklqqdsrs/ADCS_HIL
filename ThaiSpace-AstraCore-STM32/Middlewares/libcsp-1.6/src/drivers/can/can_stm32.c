/*
 * STM32F7 CMSIS-RTOS2 CAN driver for CSP
 * Converted from Zephyr driver semantics: uses HAL CAN (bxCAN) + CMSIS-RTOS2
 * - RX: HAL CAN Rx FIFO0 callback posts to an RTOS message queue
 * - RX thread: dequeues, applies CSP-era filters (software), forwards to csp_can_rx()
 * - TX: HAL_CAN_AddTxMessage with retry timeout
 *
 * Notes:
 * - Caller must initialize the CAN peripheral (bitrate, modes) before calling open.
 * - This implementation uses a software RX filter (filter_addr/filter_mask) that matches the CFP
 *   destination field. For best performance, configure hardware CAN filters in HAL if possible.
 * - This is intended as a drop-in style implementation similar to the Zephyr driver you provided.
 */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "stm32l4xx_hal.h"

#include <csp/drivers/can_socketcan.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_semaphore.h>
#include <csp/arch/csp_queue.h>
#include <csp/csp.h>

#if ENABLE_HW_CAN

#define CAN_MAX_DLEN					8

#define CSP_CAN_RX_MSGQ_TIME_OUT_MS 	1000U
#define CSP_CAN_TX_TIME_OUT_MS      	100U
#define CSP_CAN_RX_THREAD_STACK    		256U

/* CAN frame message for queueing */
typedef struct {
    uint32_t id;   /* Extended ID (no flags) */
    uint8_t dlc;
    uint8_t data[8];
    uint8_t ide; /* CAN_ID_STD / CAN_ID_EXT */
    uint8_t rtr;
} can_frame_msg_t;

/* CAN interface data */
typedef struct {
    char name[CSP_IFLIST_NAME_MAX + 1];
    csp_iface_t iface;
    csp_can_interface_data_t ifdata;

    CAN_HandleTypeDef *hcan;           	/* HAL CAN handle */

    csp_bin_sem_handle_t tx_sem;		/* semaphore for outgoing frames */
    csp_queue_handle_t rx_queue;       	/* queue for incoming frames */
    csp_thread_handle_t rx_thread;     	/* RX processing thread */

    volatile int stop_flag;

    /* Software RX filter parameters */
    uint32_t filter_addr;
    uint32_t filter_mask;
    int filter_set;
} can_context_t;

/* Keep simple linked list for potential multiple interfaces */
static can_context_t * ctx_list = NULL;

/* Helper: find ctx by hcan */
static can_context_t* can_ctx_find_by_hcan(CAN_HandleTypeDef *hcan) {
	can_context_t *it = ctx_list;

	while (it) {
		if (it->hcan == hcan)
			return it;

		it = (can_context_t*) it->iface.driver_data; /* driver_data points back to ctx in our design */
		break; /* defensive: in this simple implementation we expect driver_data==ctx */
	}

	return NULL;
}

/* Helper: find ctx by iface pointer */
static can_context_t * can_ctx_from_iface(csp_iface_t *iface) {
    if (!iface) return NULL;
    return (can_context_t *) iface->driver_data;
}

/* RX thread: dequeue frames, apply filters, and forward to CSP */
static void csp_can_stm32_rx_thread(void *arg) {
    can_context_t *ctx = (can_context_t *) arg;
    can_frame_msg_t msg;

    for (;;) {
        if (ctx->stop_flag) break;

        if (csp_queue_dequeue(ctx->rx_queue, &msg, CSP_CAN_RX_MSGQ_TIME_OUT_MS) == CSP_QUEUE_OK) {
            /* Validate DLC */
            if (msg.dlc > CAN_MAX_DLEN) continue;

            /* Only accept extended frames (CSP uses EFF) */
            if (msg.ide != CAN_ID_EXT) continue;

            /* Drop RTR */
            if (msg.rtr == CAN_RTR_REMOTE) continue;

#ifdef CSP_CAN_SW_FILTER
            /* Apply software filter if set */
            if (ctx->filter_set) {
                /* CFP destination logic: for CFP v1, shift/mask as needed. */
            	uint32_t dst_field = CFP_DST(msg.id);
                if ((dst_field & ctx->filter_mask) != (ctx->filter_addr & ctx->filter_mask)) {
                    continue; /* not matching filter */
                }
            }
#endif

            /* Forward to CSP */
            csp_can_rx(&ctx->iface, msg.id, msg.data, msg.dlc, NULL);
        }
        /* otherwise timeout -> loop again to check stop_flag */
    }

    /* Thread exiting */
    csp_thread_exit();
}

/* HAL callback: called from ISR context when a message pending in FIFO0
 * Must be linked into weak HAL_CAN_RxFifo0MsgPendingCallback in project.
 */
void csp_can_stm32_tx_callback(CAN_HandleTypeDef *hcan, portBASE_TYPE *pxTaskWoken)
{
  // Actions to perform after a message in Mailbox 0 is transmitted
  // For example, set a flag or send the next message
	can_context_t *ctx = can_ctx_find_by_hcan(hcan);

	if (!ctx)
		return;

	csp_bin_sem_post_isr(&ctx->tx_sem, pxTaskWoken);
}

/* HAL callback: called from ISR context when a message pending in FIFO0
 * Must be linked into weak HAL_CAN_RxFifo0MsgPendingCallback in project.
 */
void csp_can_stm32_rx_callback(CAN_HandleTypeDef *hcan, portBASE_TYPE *pxTaskWoken) {
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[8];
	can_context_t *ctx = can_ctx_find_by_hcan(hcan);

	if (!ctx)
		return;

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
		return;

	can_frame_msg_t msg;
	msg.ide = (rxHeader.IDE == CAN_ID_EXT) ? CAN_ID_EXT : CAN_ID_STD;
	msg.rtr = (rxHeader.RTR == CAN_RTR_REMOTE) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
	msg.dlc = rxHeader.DLC;
	if (msg.ide == CAN_ID_EXT)
		msg.id = rxHeader.ExtId;
	else
		msg.id = rxHeader.StdId;
	memcpy(msg.data, rxData, msg.dlc);

	/* post to queue; from ISR use 0 timeout */
	csp_queue_enqueue_isr(ctx->rx_queue, &msg, pxTaskWoken);
}

/* TX function for CSP */
static int csp_can_stm32_tx_frame(void *driver_data, uint32_t id, const uint8_t *data, uint8_t dlc) {
	if (dlc > 8)
		return CSP_ERR_INVAL;

	can_context_t *ctx = (can_context_t*) driver_data;
	if (!ctx || !ctx->hcan)
		return CSP_ERR_DRIVER;

	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	txHeader.ExtId = id & 0x1FFFFFFF;
	txHeader.IDE = CAN_ID_EXT;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = dlc;
	txHeader.TransmitGlobalTime = DISABLE;

	uint8_t txData[8];
	memcpy(txData, data, dlc);

	HAL_StatusTypeDef res = HAL_OK;
	uint32_t elapsed = 0;

	csp_bin_sem_wait(&ctx->tx_sem, 1);

	for (;;) {
		res = HAL_CAN_AddTxMessage(ctx->hcan, &txHeader, txData, &txMailbox);
		if (res == HAL_OK)
			break;
		if (elapsed >= 1000)
			break;
		osDelay(5);
		elapsed += 5;
	}

	osDelay(1);

	return (res == HAL_OK) ? CSP_ERR_NONE : CSP_ERR_TX;
}

/* Configure hardware CAN filter for extended ID matching of CFP destination bits
 * This example programs a 32-bit filter in mask mode. The mapping between CSP/CFP
 * destination and CAN extended ID must match your system. The HAL expects the
 * 32-bit ID field to be represented as (ExtId << 3) in FilterIdHigh/Low.
 */
int csp_can_stm32_config_hw_filter(csp_iface_t *iface, uint32_t filter_addr, uint32_t filter_mask) {
    if (!iface || !iface->driver_data) return CSP_ERR_INVAL;
    can_context_t *ctx = can_ctx_from_iface(iface);
    if (!ctx || !ctx->hcan) return CSP_ERR_INVAL;

    CAN_FilterTypeDef sFilterConfig = {0};
    /* Use filter bank 0 (adjust if using multiple CAN instances and banks) */
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;

    /* Shift to HAL 32-bit filter position: (ExtId << 3) */
    uint32_t id_shifted = (filter_addr & 0x1FFFFFFF) << 3;
    uint32_t mask_shifted = (filter_mask & 0x1FFFFFFF) << 3;

    sFilterConfig.FilterIdHigh = (uint16_t) ((id_shifted >> 16) & 0xFFFF);
    sFilterConfig.FilterIdLow  = (uint16_t) (id_shifted & 0xFFFF);
    sFilterConfig.FilterMaskIdHigh = (uint16_t) ((mask_shifted >> 16) & 0xFFFF);
    sFilterConfig.FilterMaskIdLow  = (uint16_t) (mask_shifted & 0xFFFF);

    /* If using multiple CAN interfaces/banks, ensure FilterBank is unique per interface */
    if (HAL_CAN_ConfigFilter(ctx->hcan, &sFilterConfig) != HAL_OK) {
        return CSP_ERR_DRIVER;
    }

    return CSP_ERR_NONE;
}

/* Open and add interface
 * Parameters:
 *  - ifname: optional name (NULL -> default)
 *  - hcan: pointer to initialized HAL CAN handle (must be configured by caller)
 *  - address: local CSP address
 *  - bitrate: ignored (HAL peripheral should already be configured)
 *  - filter_addr/filter_mask: software filter values; set to 0/0 to disable
 */
int csp_can_socketcan_open_and_add_interface(const char * device, const char * ifname, int bitrate, bool promisc, csp_iface_t ** return_iface) {
    (void) bitrate; /* Not used here */

    CAN_HandleTypeDef *hcan=NULL;
    if(strcmp(device, TSC_CSP_CAN1_NAME) == 0) {
    	hcan = &TSC_CSP_CAN1;
    }

    if (!hcan) {
    	return CSP_ERR_INVAL;
    }

	if (ifname == NULL) {
		ifname = CSP_IF_CAN_DEFAULT_NAME;
	}

	csp_log_info("INIT %s: device: %s, bitrate: %i, promisc: %d", ifname, device, bitrate, promisc);

	can_context_t * ctx = csp_calloc(1, sizeof(*ctx));
	if (ctx == NULL) {
		return CSP_ERR_NOMEM;
	}

    /* set interface data */
    strncpy(ctx->name, ifname, sizeof(ctx->name) - 1);
    ctx->hcan = hcan;
    ctx->stop_flag = 0;
    ctx->iface.name = ctx->name;
    ctx->iface.interface_data = &ctx->ifdata;
    ctx->iface.driver_data = ctx;
    ctx->ifdata.tx_func = csp_can_stm32_tx_frame;

    /* store software filter */
    ctx->filter_addr = CFP_MAKE_DST(csp_get_address());
    ctx->filter_mask = CFP_MAKE_DST((1 << CFP_HOST_SIZE) - 1);
    ctx->filter_set = promisc ? 0 : 1;
    if(ctx->filter_set)
    	csp_can_stm32_config_hw_filter(&ctx->iface, ctx->filter_addr, ctx->filter_mask);
    else
    	csp_can_stm32_config_hw_filter(&ctx->iface, 0, 0);

    /* link into ctx_list */
    if (ctx_list == NULL) ctx_list = ctx;

    /* Add interface to CSP */
    int res = csp_can_add_interface(&ctx->iface);
    if (res != CSP_ERR_NONE) {
        /* cleanup */
    	csp_log_error("%s[%s]: csp_can_add_interface() failed, error: %d", __FUNCTION__, ctx->name, res);
        free(ctx);
        return res;
    }

    /* Create TX semaphore */
	csp_bin_sem_create(&ctx->tx_sem);
	csp_bin_sem_post(&ctx->tx_sem);

    /* Create RX queue (depth 32) */
    ctx->rx_queue = csp_queue_create(64, sizeof(can_frame_msg_t));
    if (!ctx->rx_queue) {
    	csp_log_error("%s[%s]: csp_queue_create() failed, error: %s", __FUNCTION__, ctx->name, strerror(errno));
    	csp_free(ctx);
        return CSP_ERR_NOMEM;
    }

    /* Start HAL CAN peripheral rx notifications */
    HAL_CAN_Start(hcan);
    HAL_CAN_ActivateNotification(hcan, TSC_CSP_CAN1_ACTIVE_FLAG);

    /* Start RX thread */
	if (csp_thread_create(csp_can_stm32_rx_thread, "csp_can_rx", CSP_CAN_RX_THREAD_STACK, ctx, osPriorityRealtime, &ctx->rx_thread) != CSP_ERR_NONE) {
		csp_log_error("%s[%s]: csp_thread_create() failed to create csp_can_rx thread, errno: %s", __FUNCTION__, ctx->name, strerror(errno));
		csp_queue_remove(ctx->rx_queue);
		csp_free(ctx);
		return CSP_ERR_NOMEM;
	}

	if (return_iface) {
		*return_iface = &ctx->iface;
	}

	return CSP_ERR_NONE;
}

csp_iface_t * csp_can_socketcan_init(const char * device, int bitrate, bool promisc)
{
	int res = CSP_ERR_INVAL;
	csp_iface_t * return_iface = NULL;

	res = csp_can_socketcan_open_and_add_interface(device, CSP_IF_CAN_DEFAULT_NAME, bitrate, promisc, &return_iface);

	return (res == CSP_ERR_NONE) ? return_iface : NULL;
}

/* Set software RX filter */
int csp_can_stm32_set_rx_filter(csp_iface_t *iface, uint16_t filter_addr, uint16_t filter_mask) {
	if (!iface || !iface->driver_data)
		return CSP_ERR_INVAL;

	can_context_t *ctx = (can_context_t*) iface->driver_data;
	ctx->filter_addr = filter_addr;
	ctx->filter_mask = filter_mask;
	ctx->filter_set = (filter_mask != 0) ? 1 : 0;

	return CSP_ERR_NONE;
}

/* Stop and remove interface */
int csp_can_stm32_stop(csp_iface_t *iface) {
	if (!iface || !iface->driver_data)
		return CSP_ERR_INVAL;

	can_context_t *ctx = (can_context_t*) iface->driver_data;

	/* Deactivate notifications */
	if (ctx->hcan)
		HAL_CAN_DeactivateNotification(ctx->hcan, TSC_CSP_CAN1_ACTIVE_FLAG);

	/* Signal thread to stop and terminate */
	ctx->stop_flag = 1;
	if (ctx->rx_thread)
		osThreadTerminate(ctx->rx_thread);

	if (ctx->rx_queue)
		csp_queue_remove(ctx->rx_queue);

	/* Remove from CSP (no dedicated remove API in this file) */
	/* csp_can_remove_interface(iface); */

	/* Remove from ctx_list */
	if (ctx_list == ctx)
		ctx_list = NULL;

	csp_free(ctx);
	return CSP_ERR_NONE;
}

#endif
