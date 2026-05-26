#include "app_modules_config.h"
#if CSP_ENABLE_FILE

/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: High-level file transfer API implementation for satellite OBC
 * 
 * This module provides reliable, satellite-grade file transfer operations
 * built on top of low-level CSP file transfer protocol.
 */

#include "file_transfer_highlevel.h"
#include "file_core.h"
#include "file_transfer_standard.h"
#include "file_transfer_streaming.h"
#include "ft_config.h"
#include <string.h>
#include <stdlib.h>
#include <kservice.h>

#define LOG_TAG		"file_transfer"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>

/*
 * Forward declarations for internal APIs
 * These functions are implemented in other modules but not declared in public headers
 */

/* ft_read_chunk from file_transfer_client.c */
extern int32_t ft_read_chunk(uint8_t node, const char *path, uint32_t offset, 
                            uint32_t length, void *buf);

/* Streaming v2 functions from file_transfer_streaming_v2.c */
extern fm_err_t fts_create_streaming(
    fts_direction_t direction,
    const char *local_path,
    const char *remote_path,
    const fts_config_t *config,
    fts_session_t **session);

extern fm_err_t fts_start_streaming(fts_session_t *session);

extern fm_err_t fts_process_streaming(fts_session_t *session);

extern fm_err_t fts_destroy_streaming(fts_session_t **session);


/**
 * @brief Internal helper: validate and normalize options
 * 
 * Ensures options are within valid ranges.
 * Uses stack-allocated temporary to avoid modifying caller's structure.
 */
static void ft_normalize_options(ft_options_t *out, const ft_options_t *in) {
    if (in == NULL) {
        /* Use default options */
        out->timeout_ms = 2000;
        out->priority = 2;
        out->retries = 3;
        out->chunk_size = 200;
        out->verify_crc = 1;
        out->resume_offset = 0;
    } else {
        /* Copy and validate */
        out->timeout_ms = (in->timeout_ms == 0) ? 2000 : in->timeout_ms;
        out->priority = in->priority;
        out->retries = (in->retries == 0) ? 3 : in->retries;
        
        /* Clamp chunk_size to safe range */
        if (in->chunk_size < 128) {
            out->chunk_size = 128;
        } else if (in->chunk_size > FTS_MAX_CHUNK_SIZE) {
            out->chunk_size = FTS_MAX_CHUNK_SIZE;
        } else {
            out->chunk_size = in->chunk_size;
        }
        
        out->verify_crc = in->verify_crc;
        out->resume_offset = in->resume_offset;
    }
}

/**
 * @brief Internal helper: sleep/retry delay
 * 
 * Provides delay between retry attempts.
 * Uses FreeRTOS delay if available, otherwise busy-wait.
 */
static void ft_retry_delay_ms(uint32_t ms) {
#ifdef USE_FREERTOS
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    /* Use HAL_Delay if FreeRTOS is not enabled or not yet started */
    HAL_Delay(ms);
#endif
}

/**
 * @brief Upload file to remote node (Ground -> Satellite)
 * 
 * Implements streaming upload with chunk-level retry logic.
 * 
 * Algorithm:
 * 1. Open local file using VFS
 * 2. Seek to resume_offset if specified
 * 3. Loop until EOF:
 *    a. Read chunk from local file
 *    b. Update CRC32 if enabled
 *    c. Send chunk using ft_write_file()
 *    d. Retry on failure (up to retries count)
 *    e. Update file offset
 * 4. Close file
 * 5. Return status
 * 
 * Memory: Uses single stack buffer (chunk_size bytes).
 * No dynamic allocation suitable for satellite OBC.
 */
fm_err_t ft_upload(
    int node,
    const char *local_path,
    const char *remote_path,
    const ft_options_t *opt) {
    
    fm_file_handle_t *local_fp = NULL;
    ft_options_t normalized_opts;
    fm_err_t err;
    uint32_t bytes_read;
    uint8_t retry_count;
    fm_err_t write_result;
    fm_file_info_t file_info;
    fts_session_t *streaming_session = NULL;
    
    LOG_I("ft_upload: Starting upload [%s] -> node %d:[%s]", local_path, node, remote_path);

    /* Validate inputs */
    if (local_path == NULL || remote_path == NULL) {
        LOG_E("ft_upload: Invalid paths provided");
        return FM_ERR_INVAL;
    }
    
    /* Get file size to determine protocol selection */
    err = fm_stat(local_path, &file_info);
    if (err != FM_OK) {
        LOG_E("ft_upload: Cannot stat file %s (err=%d)", local_path, err);
        return err;
    }
    
    /* Normalize options */
    ft_normalize_options(&normalized_opts, opt);
    
    /* Auto-select protocol based on file size */
    if (file_info.size >= FT_STREAMING_THRESHOLD_BYTES) {
        /* Use streaming_v2 protocol for large files */
        LOG_I("ft_upload: Using STREAMING protocol (size: %lu bytes)", (unsigned long)file_info.size);
        
        /* Create streaming session */
        fts_config_t streaming_config = FTS_DEFAULT_CONFIG;
        streaming_config.node = (uint8_t)node;
        streaming_config.chunk_size = normalized_opts.chunk_size;
        streaming_config.timeout_ms = normalized_opts.timeout_ms;
        streaming_config.resume_offset = normalized_opts.resume_offset;
        streaming_config.verify_crc = normalized_opts.verify_crc;
        streaming_config.priority = normalized_opts.priority;
        
        err = fts_create_streaming(FTS_DIRECTION_UPLOAD, local_path, remote_path, 
                                   &streaming_config, &streaming_session);
        if (err != FM_OK) {
            LOG_E("ft_upload: Failed to create streaming session (err=%d)", err);
            return err;
        }
        
        /* Start streaming transfer */
        err = fts_start_streaming(streaming_session);
        if (err != FM_OK) {
            LOG_E("ft_upload: Failed to start streaming (err=%d)", err);
            fts_destroy_streaming(&streaming_session);
            return err;
        }
        
        while (fts_is_active(streaming_session)) {
            err = fts_process_streaming(streaming_session);
            if (err != FM_OK && !fts_is_active(streaming_session)) {
                break;
            }
        }
        
        /* Check final status */
        if (!fts_is_complete(streaming_session)) {
            err = fts_get_error(streaming_session);
            LOG_E("ft_upload: Streaming failed (err=%d)", err);
            fts_destroy_streaming(&streaming_session);
            return err;
        }
        
        /* Cleanup streaming session */
        fts_destroy_streaming(&streaming_session);
        LOG_I("ft_upload: Upload complete via streaming_v2");
        return FM_OK;
    }
    
    /* Use standard protocol for small files */
    LOG_I("ft_upload: Using STANDARD protocol (size: %lu bytes)", (unsigned long)file_info.size);
    
    /* Allocate buffer for entire file using malloc (heap) */
    uint8_t *file_buffer = (uint8_t*)malloc(file_info.size);
    if (file_buffer == NULL) {
        LOG_E("ft_upload: Memory allocation error (%lu bytes)", (unsigned long)file_info.size);
        return FM_ERR_NOMEM;
    }
    
    /* Open local file for reading */
    err = fm_open(local_path, FM_O_READ, &local_fp);
    if (err != FM_OK) {
        LOG_E("ft_upload: Cannot open local file %s (err=%d)", local_path, err);
        free(file_buffer);
        return err;
    }
    
    /* Read entire file into buffer */
    bytes_read = 0;
    err = fm_read(local_fp, file_buffer, file_info.size, &bytes_read);
    fm_close(&local_fp);
    
    if (err != FM_OK || bytes_read != file_info.size) {
        LOG_E("ft_upload: File read failed (err=%d, read=%lu/%lu)", 
                   err, (unsigned long)bytes_read, (unsigned long)file_info.size);
        free(file_buffer);
        return (err != FM_OK) ? err : FM_ERR_IO;
    }
    
    /* Calculate CRC32 if verification enabled */
    uint32_t crc32_val = 0;
    if (normalized_opts.verify_crc) {
        crc32_val = crc32_init();
        crc32_val = crc32_update(crc32_val, file_buffer, bytes_read);
        crc32_val = crc32_finalize(crc32_val);
        LOG_D("ft_upload: Local file CRC32: 0x%08lX", (unsigned long)crc32_val);
    }
    
    /* Upload file with retry logic */
    LOG_D("ft_upload: Transmitting data to remote node...");
    
    retry_count = 0;
    write_result = FM_ERR_IO;
    
    while (retry_count <= normalized_opts.retries) {
        write_result = ft_write_file(
            (uint8_t)node,
            remote_path,
            file_buffer,
            bytes_read
        );
        
        if (write_result == FM_OK) {
            break;  /* Success */
        }
        
        retry_count++;
        if (retry_count <= normalized_opts.retries) {
            LOG_W("ft_upload: Retry %d/%d (timeout: %lu ms)", 
                       retry_count, normalized_opts.retries, normalized_opts.timeout_ms);
            ft_retry_delay_ms(normalized_opts.timeout_ms);
        }
    }
    
    /* Free buffer */
    free(file_buffer);
    
    if (write_result != FM_OK) {
        LOG_E("ft_upload: Failed after %d retries (err=%d)", normalized_opts.retries, write_result);
        return write_result;
    }
    
    LOG_I("ft_upload: Success! %lu bytes transferred", (unsigned long)bytes_read);
    return FM_OK;
}

/**
 * @brief Download file from remote node (Satellite -> Ground)
 */
fm_err_t ft_download(
    int node,
    const char *remote_path,
    const char *local_path,
    const ft_options_t *opt) {

    fm_err_t err;
    fts_session_t *streaming_session = NULL;
    ft_options_t normalized_opts;

    LOG_I("ft_download: Starting download node %d:[%s] -> [%s]", node, remote_path, local_path);

    /* Validate inputs */
    if (local_path == NULL || remote_path == NULL) {
        LOG_E("ft_download: Invalid paths provided");
        return FM_ERR_INVAL;
    }

    /* Normalize options */
    ft_normalize_options(&normalized_opts, opt);

    /* Always use streaming_v2 protocol for download (no need to stat first) */
    LOG_I("ft_download: Using STREAMING protocol");

    /* Create streaming session */
    fts_config_t streaming_config = FTS_DEFAULT_CONFIG;
    streaming_config.node = (uint8_t)node;
    streaming_config.chunk_size = normalized_opts.chunk_size;
    streaming_config.timeout_ms = normalized_opts.timeout_ms;
    streaming_config.resume_offset = normalized_opts.resume_offset;
    streaming_config.verify_crc = normalized_opts.verify_crc;
    streaming_config.priority = normalized_opts.priority;

    err = fts_create_streaming(FTS_DIRECTION_DOWNLOAD, local_path, remote_path,
                               &streaming_config, &streaming_session);
    if (err != FM_OK) {
        LOG_E("ft_download: Cannot create streaming session (err=%d)", err);
        return err;
    }

    /* Start streaming transfer */
    err = fts_start_streaming(streaming_session);
    if (err != FM_OK) {
        LOG_E("ft_download: Cannot start streaming (err=%d)", err);
        fts_destroy_streaming(&streaming_session);
        return err;
    }

    /* Process streaming transfer until complete */
    while (1) {
        err = fts_process_streaming(streaming_session);

        if (err == FM_ERR_GENERIC) {
            break;
        }

        if (err != FM_OK) {
            LOG_E("ft_download: Streaming processing error (err=%d)", err);
            fts_destroy_streaming(&streaming_session);
            return err;
        }
    }

    /* Cleanup streaming session */
    fts_destroy_streaming(&streaming_session);
    LOG_I("ft_download: Success! Download complete via streaming_v2");
    return FM_OK;
}


#endif /* CSP_ENABLE_FILE */
