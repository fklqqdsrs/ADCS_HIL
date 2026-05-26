/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Streaming file transfer API (Streaming V2 Protocol)
 * 
 * This header defines the public streaming API for the streaming_v2 implementation.
 * The streaming_v1 (legacy) implementation has been removed.
 */

#ifndef _FILE_TRANSFER_STREAMING_H_
#define _FILE_TRANSFER_STREAMING_H_

#if defined __cplusplus
extern "C" {
#endif

#include <csp/csp.h>
#include <stdint.h>
#include <stdbool.h>
#include "file_core.h"

/*===========================================================================
 * CONSTANTS
 *===========================================================================*/

#define FTS_MAX_SESSIONS 4              /**< Maximum concurrent streaming sessions */

/*===========================================================================
 * TYPES
 *===========================================================================*/

/**
 * @brief Streaming transfer direction
 */
typedef enum {
    FTS_DIRECTION_UPLOAD,               /**< Upload from local to remote */
    FTS_DIRECTION_DOWNLOAD              /**< Download from remote to local */
} fts_direction_t;

/**
 * @brief Streaming session state
 */
typedef enum {
    FTS_STATE_IDLE,                     /**< Session not started */
    FTS_STATE_CONNECTING,               /**< Establishing connection */
    FTS_STATE_TRANSFERRING,             /**< Active transfer */
    FTS_STATE_PAUSED,                   /**< Transfer paused */
    FTS_STATE_COMPLETED                 /**< Transfer completed successfully */
} fts_state_t;

/**
 * @brief Opaque streaming session handle
 */
typedef struct fts_session fts_session_t;

/**
 * @brief Streaming transfer statistics
 */
typedef struct {
    uint32_t bytes_transferred;         /**< Bytes transferred so far */
    uint32_t chunks_sent;               /**< Number of chunks sent */
} fts_stats_t;

/**
 * @brief Streaming configuration
 */
typedef struct {
    uint8_t node;                       /**< Remote node address */
    uint8_t priority;                   /**< CSP priority (0-3) */
    uint16_t chunk_size;                /**< Chunk size in bytes */
    uint32_t timeout_ms;                /**< Timeout in milliseconds */
    uint32_t resume_offset;             /**< Resume from this byte offset (0=start from beginning) */
    bool verify_crc;                    /**< Enable CRC verification */
} fts_config_t;

/**
 * @brief Default streaming configuration
 */
#define FTS_DEFAULT_CONFIG { \
    .node = 0, \
    .priority = 2, \
    .chunk_size = FTS_MAX_CHUNK_SIZE, \
    .timeout_ms = 5000, \
    .resume_offset = 0, \
    .verify_crc = false \
}


/*===========================================================================
 * API FUNCTIONS
 *===========================================================================*/

/**
 * @brief Create streaming session
 * 
 * @param direction Upload or download
 * @param local_path Local file path
 * @param remote_path Remote file path
 * @param config Configuration (NULL for defaults)
 * @param session Output session handle
 * @return FM_OK on success, error code otherwise
 */
fm_err_t fts_create_streaming(
    fts_direction_t direction,
    const char *local_path,
    const char *remote_path,
    const fts_config_t *config,
    fts_session_t **session);

/**
 * @brief Start streaming transfer
 * 
 * @param session Session handle
 * @return FM_OK on success, error code otherwise
 */
fm_err_t fts_start_streaming(fts_session_t *session);

/**
 * @brief Process streaming transfer (one chunk)
 * 
 * Call this repeatedly until transfer completes or fails.
 * 
 * @param session Session handle
 * @return FM_OK on success, error code otherwise
 */
fm_err_t fts_process_streaming(fts_session_t *session);

/**
 * @brief Destroy streaming session
 * 
 * @param session Pointer to session handle (set to NULL on return)
 * @return FM_OK on success, error code otherwise
 */
fm_err_t fts_destroy_streaming(fts_session_t **session);

/**
 * @brief Get current transfer state
 * 
 * @param session Session handle
 * @return Current state
 */
fts_state_t fts_get_state(fts_session_t *session);

/**
 * @brief Get last error code
 * 
 * @param session Session handle
 * @return Error code
 */
fm_err_t fts_get_error(fts_session_t *session);

/**
 * @brief Check if transfer is active
 * 
 * @param session Session handle
 * @return true if active, false otherwise
 */
bool fts_is_active(fts_session_t *session);

/**
 * @brief Check if transfer is complete
 * 
 * @param session Session handle
 * @return true if complete, false otherwise
 */
bool fts_is_complete(fts_session_t *session);

#if defined __cplusplus
}
#endif

#endif /* _FILE_TRANSFER_STREAMING_H_ */
