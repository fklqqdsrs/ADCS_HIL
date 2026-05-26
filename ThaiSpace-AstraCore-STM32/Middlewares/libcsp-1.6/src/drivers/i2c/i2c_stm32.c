/**
 * @file csp_i2c_stm32.c
 * @brief STM32 HAL I2C driver glue layer for the CubeSat Space Protocol (CSP).
 *
 * This module implements a CSP I2C interface on STM32 microcontrollers using
 * HAL I2C drivers and CMSIS-RTOS v2 threading. It provides:
 *  - A transmit function (`csp_i2c_stm32_tx_frame`) using HAL I2C Master transmit (IT/DMA).
 *  - An RX thread that dequeues received frames from ISR and forwards to CSP.
 *  - HAL callbacks (`HAL_I2C_*Callback`) that integrate with CSP queues/semaphores.
 *  - Interface registration function `csp_i2c_open_and_add_interface()`.
 *
 * Key design points:
 *  - RX path is interrupt-driven, frames are passed to a queue, then handled
 *    in a CSP RX thread.
 *  - TX path is thread-based: CSP pushes frames, HAL TX done callback releases a semaphore.
 *  - Uses `csp_buffer_*`, `csp_queue_*`, `csp_bin_sem_*` from CSP arch layer.
 *
 * @note This is an example glue implementation. Adapt buffer management,
 *       error handling, and interface registration for your project.
 *
 */
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <csp/drivers/i2c.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_thread.h>
#include <csp/arch/csp_semaphore.h>
#include <csp/arch/csp_queue.h>
#include <csp/csp.h>

/* Only compile this entire file if I2C is enabled */
#if ENABLE_HW_I2C

/* Configuration */
#define CSP_I2C_RX_MSGQ_TIME_OUT_MS 	1000U

#define CSP_I2C_RX_QUEUE_SIZE 			4
#define CSP_I2C_RX_QUEUE_ITEM_SIZE 		csp_buffer_size()
#define CSP_I2C_RX_THREAD_STACK 		1024
#define CSP_I2C_RX_THREAD_PRIO 			osPriorityRealtime

/* Driver state structure (driver_data) */
typedef struct {
    char name[CSP_IFLIST_NAME_MAX + 1];   /**< Human-readable interface name. */

    csp_iface_t iface;                    /**< CSP interface structure (registered with CSP). */
    csp_i2c_interface_data_t ifdata;      /**< CSP I2C-specific interface data (tx function, etc.). */

    I2C_HandleTypeDef *hi2c;              /**< Pointer to HAL I2C handle for this interface. */

    csp_bin_sem_handle_t tx_sem;          /**< TX complete semaphore, signaled in HAL TX callback. */
    csp_queue_handle_t rx_queue;          /**< RX queue for passing frames from ISR to RX thread. */
    csp_thread_handle_t rx_thread;        /**< RX worker thread handle. */

    volatile int stop_flag;               /**< Flag to request RX thread exit. */

    uint8_t rx_first_recv;                /**< RX state flag: 0 = receiving length, 1 = receiving data. */
    csp_i2c_frame_t *rx_buffer;           /**< Pre-allocated RX buffer for incoming frames. */
    size_t rx_buffer_size;                /**< Size of RX buffer in bytes. */
} i2c_context_t;

/* Keep simple linked list for potential multiple interfaces */
static i2c_context_t * ctx_list = NULL;

/**
 * @brief HAL callback: I2C master TX complete.
 * Must be add into HAL_I2C_MasterTxCpltCallback in project.
 *
 * Signals the TX semaphore to unblock the CSP TX thread waiting for
 * transmit completion.
 *
 * @param hi2c Pointer to HAL I2C handle.
 */
void csp_i2c_stm32_master_tx_callback(I2C_HandleTypeDef *hi2c, portBASE_TYPE *pxTaskWoken) {

    if (!ctx_list) return;
    if (ctx_list->hi2c != hi2c) return;

    /* Release semaphore to indicate TX finished */
    csp_bin_sem_post_isr(&ctx_list->tx_sem, pxTaskWoken);
}

/**
 * @brief HAL callback: I2C listen complete.
 * Must be add into HAL_I2C_ListenCpltCallback in project.
 *
 * Re-enables listen mode to continue receiving further transactions.
 *
 * @param hi2c Pointer to HAL I2C handle.
 */
void csp_i2c_stm32_listen_callback (I2C_HandleTypeDef *hi2c)
{
    if (!ctx_list) return;
    if (ctx_list->hi2c != hi2c) return;

	HAL_I2C_EnableListen_IT(hi2c);
}

/**
 * @brief HAL callback: Address match.
 * Must be add into HAL_I2C_AddrCallback in project.
 *
 * Invoked when a master addresses this device. If direction is transmit,
 * schedules reception of the first 2 bytes (frame length).
 *
 * @param hi2c              HAL I2C handle.
 * @param TransferDirection Direction of transfer (transmit or receive).
 * @param AddrMatchCode     Matched slave address.
 */
void csp_i2c_stm32_addr_callback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (!ctx_list) return;
    if (ctx_list->hi2c != hi2c) return;

	if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
	{
		if (ctx_list->rx_first_recv == 0)
		{
			// receive using sequential function.
			HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t*)&ctx_list->rx_buffer->len, 2, I2C_FIRST_AND_NEXT_FRAME);
		}
	}
}

/**
 * @brief HAL callback: Slave RX complete.
 * Must be add into HAL_I2C_SlaveRxCpltCallback in project.
 *
 * Handles sequential slave reception. First receives frame length,
 * then data bytes. Completed frame is enqueued for RX thread.
 *
 * @param hi2c HAL I2C handle.
 */
void csp_i2c_stm32_slave_rx_callback(I2C_HandleTypeDef *hi2c, portBASE_TYPE *pxTaskWoken)
{
    if (!ctx_list) return;
    if (ctx_list->hi2c != hi2c) return;

	if (!ctx_list->rx_first_recv)
	{
		ctx_list->rx_first_recv = 1;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, ctx_list->rx_buffer->data, ctx_list->rx_buffer->len, I2C_LAST_FRAME);
	}
	else
	{
		ctx_list->rx_first_recv = 0;
		/* Enqueue the frame pointer for the rx thread to pick up */
		if (csp_queue_enqueue_isr(ctx_list->rx_queue, ctx_list->rx_buffer, pxTaskWoken) != CSP_QUEUE_OK) {
			/* Queue full */
			ctx_list->iface.tx_error++;
		}

		memset(ctx_list->rx_buffer, 0, ctx_list->rx_buffer_size);
	}
}

/**
 * @brief HAL callback: I2C error handler.
 * Must be add into HAL_I2C_ErrorCallback in project.
 *
 * Handles AF/NACK and other HAL I2C errors. Attempts to enqueue any
 * partially received frame, resets RX state, increments error counters,
 * and re-enables listen mode.
 *
 * @param hi2c HAL I2C handle.
 */
void csp_i2c_stm32_error_callback(I2C_HandleTypeDef *hi2c, portBASE_TYPE *pxTaskWoken)
{
    if (!ctx_list) return;
    if (ctx_list->hi2c != hi2c) return;

	if (HAL_I2C_ERROR_AF == HAL_I2C_GetError(hi2c))  // AF error
	{
		ctx_list->rx_first_recv = 0;
		/* Enqueue the frame pointer for the rx thread to pick up */
		if (csp_queue_enqueue_isr(ctx_list->rx_queue, ctx_list->rx_buffer, pxTaskWoken) != CSP_QUEUE_OK) {
			/* Queue full */
			ctx_list->iface.tx_error++;
		}
		memset(ctx_list->rx_buffer, 0, ctx_list->rx_buffer_size);
	}
	ctx_list->iface.tx_error++;

	HAL_I2C_EnableListen_IT(hi2c);
}

/**
 * @brief RX thread function.
 *
 * Continuously dequeues I2C frames from the RX queue and passes them to CSP
 * for processing via csp_i2c_rx(). Runs until @ref i2c_context_t::stop_flag is set.
 *
 * @param arg Pointer to the driver context (i2c_context_t).
 */
static void csp_i2c_stm32_rx_thread(void *arg) {
    i2c_context_t *ctx = (i2c_context_t *) arg;
    csp_i2c_frame_t *frame = csp_buffer_get(256);

    for (;;) {
        if (ctx->stop_flag) break;

		if (csp_queue_dequeue(ctx->rx_queue, frame, CSP_I2C_RX_MSGQ_TIME_OUT_MS) == CSP_QUEUE_OK) {
			/* Forward to CSP */
			csp_i2c_rx(&ctx->iface, frame, NULL);

			frame = csp_buffer_get(256);
		}
        /* otherwise timeout -> loop again to check stop_flag */
    }

    /* Thread exiting */
    csp_thread_exit();
}

/**
 * @brief CSP TX function for STM32 I2C.
 *
 * Called by CSP when a frame should be transmitted over I2C.
 * Performs a blocking transmit using HAL I2C (DMA or IT), synchronized by a semaphore
 * released in @ref HAL_I2C_MasterTxCpltCallback.
 *
 * @param driver_data Pointer to driver context (i2c_context_t).
 * @param frame       CSP I2C frame to transmit. Freed after successful transmit.
 *
 * @return CSP_ERR_NONE on success, error code otherwise.
 */
static int csp_i2c_stm32_tx_frame(void * driver_data, csp_i2c_frame_t * frame) {
	i2c_context_t *ctx = (i2c_context_t*) driver_data;
	if (!ctx || !ctx->tx_sem)
		return CSP_ERR_INVAL;

	HAL_StatusTypeDef halsh = HAL_I2C_DisableListen_IT(ctx->hi2c);
	if(halsh != HAL_OK)
		csp_log_error("%s[%s]: HAL_I2C_DisableListen_IT() failed to disable code=%d", __FUNCTION__, ctx->name, halsh);

	HAL_StatusTypeDef halst = HAL_I2C_Master_Transmit_DMA(ctx->hi2c, (uint16_t)(frame->dest << 1), (uint8_t*)&frame->len, frame->len + sizeof(frame->len));
//	HAL_StatusTypeDef halst = HAL_I2C_Master_Transmit_IT(ctx->hi2c, (uint16_t)(frame->dest << 1), (uint8_t *)&frame->len, frame->len + sizeof(frame->len));

	if(halst != HAL_OK)
		csp_log_error("%s[%s]: HAL_I2C_Master_Transmit_DMA() failed to send code=%d", __FUNCTION__, ctx->name, halst);

    /* Wait for completion signalled by HAL callback */
    if (csp_bin_sem_wait(&ctx->tx_sem, 1000) != CSP_SEMAPHORE_OK) {
        /* Timeout waiting for TX complete */
    	csp_log_error("%s[%s]: i2c tx sem time over", __FUNCTION__, ctx->name);
        ctx->iface.tx_error++;
    }

    uint32_t tx_wait=0;
    /* Re-enable listen mode so we can accept slave requests again */
    while (HAL_I2C_GetState(ctx->hi2c) != HAL_I2C_STATE_READY) {
        osDelay(1);
        if(tx_wait++ > 100) break;
    }

//    csp_log_error("%s[%s]: Tx ready wait=%ld", __FUNCTION__, ctx->name, tx_wait);

    HAL_StatusTypeDef halen = HAL_I2C_EnableListen_IT(ctx->hi2c);
	if(halen != HAL_OK)
		csp_log_error("%s[%s]: HAL_I2C_EnableListen_IT() failed to enable code=%d", __FUNCTION__, ctx->name, halen);

	csp_buffer_free(frame);

    return CSP_ERR_NONE;
}

/**
 * @brief Open an STM32 HAL I2C device and register a CSP interface.
 *
 * Allocates driver context, sets up queues/semaphores, creates the RX thread,
 * and enables listen mode for I2C slave reception.
 *
 * @param device        Device identifier string (maps to HAL I2C handle).
 * @param ifname        Interface name (used in CSP interface list).
 * @param rx_buf_size   Size of the receive buffer in bytes.
 * @param return_iface  Optional pointer to receive created CSP interface pointer.
 *
 * @return CSP_ERR_NONE on success, error code otherwise.
 */
int csp_i2c_open_and_add_interface(const char * device, const char * ifname, size_t rx_buf_size, csp_iface_t ** return_iface) {
    I2C_HandleTypeDef *hi2c=NULL;
    if(strcmp(device, TSC_CSP_I2C1_NAME) == 0) {
    	hi2c = &TSC_CSP_I2C1;
    }

    if (!hi2c) {
    	return CSP_ERR_INVAL;
    }

	if (ifname == NULL) {
		ifname = CSP_IF_I2C_DEFAULT_NAME;
	}

	csp_log_info("INIT %s: device: %s, rx buffer size: %i", ifname, device, rx_buf_size);

	i2c_context_t * ctx = csp_calloc(1, sizeof(*ctx));
	if (ctx == NULL) {
		return CSP_ERR_NOMEM;
	}

	/* set interface data */
	strncpy(ctx->name, ifname, sizeof(ctx->name) - 1);
    ctx->hi2c = hi2c;
    ctx->iface.name = ctx->name;
    ctx->iface.interface_data = &ctx->ifdata;
    ctx->iface.driver_data = ctx;
    ctx->ifdata.tx_func = csp_i2c_stm32_tx_frame;
    ctx->rx_buffer_size = (rx_buf_size > CSP_I2C_RX_QUEUE_ITEM_SIZE) ? rx_buf_size : CSP_I2C_RX_QUEUE_ITEM_SIZE;
    ctx->rx_buffer = csp_malloc(ctx->rx_buffer_size);
	if (!ctx->rx_buffer) {
		csp_log_error("%s[%s]: csp_malloc() failed to allocate buffer memory", __FUNCTION__, ctx->name);
		csp_free(ctx);
		return CSP_ERR_NOMEM;
	}

	/* set slave address */
    hi2c->Init.OwnAddress1 = csp_get_address() << 1;
    HAL_I2C_Init(hi2c);

    /* link into ctx_list */
    if (ctx_list == NULL) ctx_list = ctx;

    /* Add interface to CSP */
    int ret = csp_i2c_add_interface(&ctx->iface);
    if (ret != CSP_ERR_NONE) {
        if (ctx->rx_buffer) csp_free(ctx->rx_buffer);
        csp_free(ctx);
        ctx_list = NULL;
        return ret;
    }

    /* Create TX semaphore */
	csp_bin_sem_create(&ctx->tx_sem);

	ctx->rx_queue = csp_queue_create(CSP_I2C_RX_QUEUE_SIZE, CSP_I2C_RX_QUEUE_ITEM_SIZE);
    if (!ctx->rx_queue) {
    	csp_log_error("%s[%s]: csp_queue_create() failed to create i2c rx_queue", __FUNCTION__, ctx->name);
        csp_bin_sem_remove(&ctx->tx_sem);
        csp_queue_remove(ctx->rx_queue);
        if (ctx->rx_buffer) csp_free(ctx->rx_buffer);
        csp_free(ctx);
        return CSP_ERR_NOMEM;
    }

    /* Start RX thread */
	if (csp_thread_create(csp_i2c_stm32_rx_thread, "csp_i2c_rx", CSP_I2C_RX_THREAD_STACK, ctx, CSP_I2C_RX_THREAD_PRIO, &ctx->rx_thread) != CSP_ERR_NONE) {
		csp_log_error("%s[%s]: csp_thread_create() failed to create csp_i2c_rx thread", __FUNCTION__, ctx->name);
        csp_bin_sem_remove(&ctx->tx_sem);
		csp_queue_remove(ctx->rx_queue);
        if (ctx->rx_buffer) csp_free(ctx->rx_buffer);
        csp_free(ctx);
		return CSP_ERR_NOMEM;
	}

    /* Start receiving frames as I2C slave */
    if(HAL_I2C_EnableListen_IT(ctx->hi2c) != HAL_OK) {
    	csp_log_error("%s[%s]: HAL_I2C_EnableListen_IT() failed to listen i2c bus", __FUNCTION__, ctx->name);
    }

	if (return_iface) {
		*return_iface = &ctx->iface;
	}


	return CSP_ERR_NONE;
}

#endif
