/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Standard file transfer protocol (Client + Server API)
 * 
 * This header combines both client and server API declarations
 * for the standard file transfer protocol (port 11).
 */

#ifndef _FILE_TRANSFER_STANDARD_H_
#define _FILE_TRANSFER_STANDARD_H_

#include <csp/csp.h>
#include <csp_port.h>
#include <csp/arch/csp_thread.h>
#include "file_core.h"
#include "file_transfer_protocols.h"
#include "thread_manager_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * CLIENT API
 *===========================================================================*/

/**
 * @brief Get file/directory information
 * @param node Remote node address
 * @param path Remote file path
 * @param info Output file information
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_stat(uint8_t node, const char *path, fm_file_info_t *info);

/**
 * @brief Create directory
 * @param node Remote node address
 * @param path Remote directory path
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_mkdir(uint8_t node, const char *path);

/**
 * @brief Remove file or directory
 * @param node Remote node address
 * @param path Remote file/directory path
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_rm(uint8_t node, const char *path);

/**
 * @brief Move/rename file or directory
 * @param node Remote node address
 * @param src Source path
 * @param dst Destination path
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_mv(uint8_t node, const char *src, const char *dst);

/**
 * @brief Copy file
 * @param node Remote node address
 * @param src Source path
 * @param dst Destination path
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_cp(uint8_t node, const char *src, const char *dst);

/**
 * @brief Get filesystem space information
 * @param node Remote node address
 * @param total_kb Output total space in KB
 * @param free_kb Output free space in KB
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_get_space(uint8_t node, uint32_t *total_kb, uint32_t *free_kb);

/**
 * @brief Open remote file
 * @param node Remote node address
 * @param path Remote file path
 * @param mode File open mode (FA_READ, FA_WRITE, etc.)
 * @return File handle (>0) on success, negative error code otherwise
 */
int32_t ft_open(uint8_t node, const char *path, uint8_t mode);

/**
 * @brief Read from remote file
 * @param node Remote node address
 * @param handle File handle from ft_open()
 * @param buf Buffer to read into
 * @param len Number of bytes to read
 * @return Number of bytes read on success, negative error code otherwise
 */
int32_t ft_read(uint8_t node, int32_t handle, void *buf, uint32_t len);

/**
 * @brief Write to remote file
 * @param node Remote node address
 * @param handle File handle from ft_open()
 * @param buf Buffer to write from
 * @param len Number of bytes to write
 * @return Number of bytes written on success, negative error code otherwise
 */
int32_t ft_write(uint8_t node, int32_t handle, const void *buf, uint32_t len);

/**
 * @brief Write entire file in one operation (convenience function)
 * @param node Remote node address
 * @param path Remote file path
 * @param buf Buffer containing file data
 * @param len Length of data to write
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_write_file(uint8_t node, const char *path, const void *buf, uint32_t len);

/**
 * @brief Read entire file in one operation (convenience function)
 * @param node Remote node address
 * @param path Remote file path
 * @param buf Buffer to read into
 * @param len Maximum bytes to read
 * @return Number of bytes read on success, negative error code otherwise
 */
int32_t ft_read_file(uint8_t node, const char *path, void *buf, uint32_t len);

/**
 * @brief Read file chunk at specific offset
 * @param node Remote node address
 * @param path Remote file path
 * @param offset File offset to read from
 * @param length Number of bytes to read
 * @param buf Buffer to read into
 * @return Number of bytes read on success, negative error code otherwise
 */
int32_t ft_read_chunk(uint8_t node, const char *path, uint32_t offset, uint32_t length, void *buf);

/**
 * @brief Close remote file
 * @param node Remote node address
 * @param handle File handle from ft_open()
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_close(uint8_t node, int32_t handle);

/**
 * @brief Open remote directory
 * @param node Remote node address
 * @param path Remote directory path
 * @return Directory handle (>0) on success, negative error code otherwise
 */
int32_t ft_opendir(uint8_t node, const char *path);

/**
 * @brief Read directory entry
 * @param node Remote node address
 * @param handle Directory handle from ft_opendir()
 * @param info Output file information
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_readdir(uint8_t node, int32_t handle, fm_file_info_t *info);

/**
 * @brief Close remote directory
 * @param node Remote node address
 * @param handle Directory handle from ft_opendir()
 * @return FM_OK on success, error code otherwise
 */
fm_err_t ft_closedir(uint8_t node, int32_t handle);

/**
 * @brief List directory contents (prints to console)
 * @param node Remote node address
 * @param path Remote directory path
 */
void ft_list(uint8_t node, const char *path);

/*===========================================================================
 * SERVER API
 *===========================================================================*/

#define FT_MAX_OPEN_FILES 4
#define FT_MAX_OPEN_DIRS 2
#define FT_SERVER_THREAD_NAME "csp_file_server"
#define FT_SERVER_STACK_SIZE 4096
#define FT_SERVER_PRIORITY (osPriority_t)osPriorityAboveNormal

/**
 * @brief Handle file transfer server request
 * @param conn CSP connection
 * @param packet Request packet
 */
void ft_server_handler(csp_conn_t *conn, csp_packet_t *packet);

#ifdef __cplusplus
}
#endif

#endif /* _FILE_TRANSFER_STANDARD_H_ */
