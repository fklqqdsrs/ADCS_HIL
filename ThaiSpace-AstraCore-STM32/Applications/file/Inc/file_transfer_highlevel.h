/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: High-level file transfer API for satellite OBC
 * 
 * This module provides reliable, satellite-grade file transfer operations
 * built on top of the low-level CSP file transfer protocol.
 * 
 * Designed for:
 * - High-latency communication links (hundreds of ms)
 * - Unreliable links with packet loss
 * - Constrained memory environments (STM32L496)
 * - Resumable transfers with CRC32 verification
 */

#ifndef _FILE_TRANSFER_HIGHLEVEL_H_
#define _FILE_TRANSFER_HIGHLEVEL_H_

#if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "file_core.h"
#include "crc32.h"

/**
 * @brief File transfer options structure
 * 
 * Configures behavior for upload/download operations.
 * All fields are optional - use FT_DEFAULT_OPTIONS for sensible defaults.
 */
typedef struct {
    uint32_t timeout_ms;     /**< Per-chunk timeout in milliseconds (default: 2000) */
    uint8_t  priority;       /**< CSP priority level (default: 2) */
    uint8_t  retries;        /**< Retry attempts per chunk (default: 3) */
    uint16_t chunk_size;     /**< Transfer chunk size in bytes (default: 200, range: 128-200) */
    uint8_t  verify_crc;     /**< Enable CRC32 verification (1=enabled, 0=disabled) */
    uint32_t resume_offset;  /**< Resume transfer from this byte offset (default: 0) */
} ft_options_t;

/**
 * @brief Default transfer options
 * 
 * Sensible defaults for satellite-to-ground and ground-to-satellite transfers.
 * - 2s timeout per chunk (accommodates high latency)
 * - Priority 2 (normal priority)
 * - 3 retries per chunk (handles packet loss)
 * - 200-byte chunks (balance efficiency and reliability)
 * - CRC verification enabled
 * - Start from beginning
 */
#define FT_DEFAULT_OPTIONS  {2000, 2, 3, 200, 1, 0}

/**
 * @brief Upload file to remote node (Ground -> Satellite)
 * 
 * Reads a local file and transfers it to a remote node over CSP.
 * Implements streaming transfer with chunk-level retry logic and optional CRC32.
 * 
 * Operation flow:
 * 1. Open local file using VFS/FatFs
 * 2. Seek to resume_offset if non-zero
 * 3. Loop: read chunk, compute CRC, send with retry
 * 4. Continue until EOF or error
 * 
 * Memory constraints:
 * - Uses single stack buffer (no dynamic allocation)
 * - Buffer size = chunk_size (configurable, typically 200 bytes)
 * - Suitable for STM32L496 with ~256KB RAM
 * 
 * Reliability features:
 * - Per-chunk retry with configurable count
 * - Timeout per chunk operation
 * - CRC32 calculation for integrity (optional)
 * - Resume capability from any offset
 * - Error propagation through fm_err_t codes
 * 
 * @param node          Target CSP node address
 * @param local_path    Local file path (e.g., "/D/data.txt")
 * @param remote_path   Remote file path (e.g., "/D/telemetry/data.txt")
 * @param opt           Transfer options (NULL uses FT_DEFAULT_OPTIONS)
 * 
 * @return FM_OK on success, error code on failure
 * 
 * @note This function is BLOCKING. For non-blocking operation,
 *       consider implementing a state machine in a FreeRTOS task.
 * 
 * @note Transfer progress is not reported. Add callback if needed
 *       for UI feedback or telemetry.
 * 
 * @example
 * ft_options_t opt = FT_DEFAULT_OPTIONS;
 * opt.resume_offset = 1024;  // Resume from 1KB
 * fm_err_t err = ft_upload(5, "/D/commands.txt", "/C/newcmd.txt", &opt);
 */
fm_err_t ft_upload(
    int node,
    const char *local_path,
    const char *remote_path,
    const ft_options_t *opt);

/**
 * @brief Download file from remote node (Satellite -> Ground)
 * 
 * Requests file data from a remote node and writes it locally.
 * Implements streaming transfer with chunk-level retry logic and optional CRC32.
 * 
 * Operation flow:
 * 1. Open/create local file using VFS/FatFs
 * 2. Seek to resume_offset if non-zero
 * 3. Loop: request chunk, compute CRC, write to file
 * 4. Continue until ft_read_chunk returns 0 bytes (EOF)
 * 
 * Memory constraints:
 * - Uses single stack buffer (no dynamic allocation)
 * - Buffer size = chunk_size (configurable, typically 200 bytes)
 * - Suitable for STM32L496 with ~256KB RAM
 * 
 * Reliability features:
 * - Resumable from any offset
 * - CRC32 calculation for integrity (optional)
 * - Automatic retry handled by underlying CSP layer
 * - Error propagation through fm_err_t codes
 * 
 * @param node          Source CSP node address
 * @param remote_path   Remote file path to download
 * @param local_path    Local destination path
 * @param opt           Transfer options (NULL uses FT_DEFAULT_OPTIONS)
 * 
 * @return FM_OK on success, error code on failure
 * 
 * @note This function is BLOCKING. For non-blocking operation,
 *       consider implementing a state machine in a FreeRTOS task.
 * 
 * @note Transfer progress is not reported. Add callback if needed
 *       for UI feedback or telemetry.
 * 
 * @example
 * ft_options_t opt = FT_DEFAULT_OPTIONS;
 * opt.verify_crc = 0;  // Disable CRC for faster download
 * fm_err_t err = ft_download(5, "/C/logs/syslog.txt", "/D/download/syslog.txt", &opt);
 */
fm_err_t ft_download(
    int node,
    const char *remote_path,
    const char *local_path,
    const ft_options_t *opt);

#if defined __cplusplus
}
#endif

#endif /* _FILE_TRANSFER_HIGHLEVEL_H_ */
