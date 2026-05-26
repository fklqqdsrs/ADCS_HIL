/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: File transfer protocol definitions (Standard + Streaming)
 * 
 * This header consolidates both protocol definitions:
 * - Standard Protocol (Port 11): Simple request-response
 * - Streaming Protocol (Port 12): Persistent connection with flow control
 */

#ifndef FILE_TRANSFER_PROTOCOLS_H
#define FILE_TRANSFER_PROTOCOLS_H

#if defined __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <csp/csp.h>
#include <csp_port.h>
#include <csp/csp_types.h>
#include <csp/arch/csp_thread.h>
#include "thread_manager_core.h"
#include "file_core.h"
#include "ft_config.h"

/*===========================================================================
 * STANDARD PROTOCOL (Port 11)
 *===========================================================================*/

/* Constants */
#define FT_CSP_PORT 11
#define FT_CSP_TIMEOUT_MS 1000U
#define CSP_FT_SERVER_CON_MAX 8
#define FT_CSP_MTU 196U
#define FT_MAX_PATH_LEN 128U
#define FT_MAX_DATA_LEN (FT_CSP_MTU - sizeof(ft_packet_header_t) - sizeof(uint16_t) - FT_MAX_PATH_LEN)

/* Command Types */
typedef enum {
	FT_CMD_READ = 1, FT_CMD_WRITE, FT_CMD_READ_CHUNK, FT_CMD_LIST, FT_CMD_DELETE, FT_CMD_STAT, FT_CMD_MKDIR, FT_CMD_RENAME, FT_CMD_COPY, FT_CMD_GET_SPACE
} ft_cmd_t;

/* Flags */
#define FT_FLAG_NONE 0x00
#define FT_FLAG_MORE 0x01
#define FT_FLAG_EOF 0x02
#define FT_FLAG_ERROR 0x80

/* Packet Header */
typedef struct __attribute__((packed)) {
	uint16_t cmd;
	uint8_t flags;
	int16_t status;
	uint16_t seq;
	uint16_t payload_len;
} ft_packet_header_t;

/*===========================================================================
 * STREAMING PROTOCOL (Port 12)
 *===========================================================================*/

/* Constants */
#define FTS_CSP_PORT 12                    /**< Streaming protocol port */
#define FTS_PROTOCOL_VERSION 1             /**< Protocol version */
#define FTS_MAX_CHUNK_SIZE 200             /**< Maximum chunk size (fits in 256-byte CSP buffer) */
#define FTS_WINDOW_SIZE 4                  /**< Flow control window size */
#define FTS_SESSION_TIMEOUT_MS 10000       /**< Session timeout (10 seconds) */
#define FTS_SESSION_CLEANUP_INTERVAL_MS 10000  /**< Cleanup check interval (10 seconds) */

/* Security Constants */
#define FTS_MAX_REQUESTS_PER_MINUTE 60     /**< Rate limit: max requests per minute per node */
#define FTS_RATE_LIMIT_WINDOW_MS 60000     /**< Rate limit window (1 minute) */

/* CSP Security Options */
#define FTS_SECURITY_NONE   0x00           /**< No security */
#define FTS_SECURITY_HMAC   0x01           /**< HMAC authentication */
#define FTS_SECURITY_XTEA   0x02           /**< XTEA encryption */
#define FTS_SECURITY_ALL    (FTS_SECURITY_HMAC | FTS_SECURITY_XTEA)  /**< Both HMAC and XTEA */

/* Command Types */
typedef enum {
	/* Session management */
	FTS_CMD_OPEN_UPLOAD = 0x10, /**< Open file for upload (client->server) */
	FTS_CMD_OPEN_DOWNLOAD = 0x11, /**< Open file for download (client->server) */
	FTS_CMD_CLOSE = 0x12, /**< Close current transfer */

	/* Data transfer */
	FTS_CMD_DATA = 0x20, /**< Data chunk */
	FTS_CMD_ACK = 0x21, /**< Acknowledge chunk */
	FTS_CMD_NACK = 0x22, /**< Negative acknowledge (retry) */

	/* Status */
	FTS_CMD_ERROR = 0x41 /**< Error notification */
} fts_cmd_t;

/* Packet Structures */
typedef struct __attribute__((packed)) {
	uint8_t version; /**< Protocol version (FTS_PROTOCOL_VERSION) */
	uint8_t cmd; /**< Command type (fts_cmd_t) */
	uint16_t flags; /**< Command-specific flags */
	uint32_t session_id; /**< Session identifier */
	uint32_t sequence; /**< Sequence number for ordering */
	uint16_t payload_len; /**< Payload length in bytes */
	uint16_t checksum; /**< Header checksum (CRC16) */
} fts_packet_header_t;

typedef struct __attribute__((packed)) {
	uint32_t file_size; /**< Total file size (0 if unknown) */
	uint32_t resume_offset; /**< Resume from this offset */
	uint16_t chunk_size; /**< Requested chunk size */
	uint8_t flags; /**< Transfer flags */
	uint8_t reserved;
	char path[0]; /**< Variable-length path (null-terminated) */
} fts_open_payload_t;

typedef struct __attribute__((packed)) {
	uint32_t offset; /**< File offset for this chunk */
	uint16_t data_len; /**< Data length in this packet */
	uint16_t crc16; /**< Data CRC16 */
	uint8_t data[0]; /**< Variable-length data */
} fts_data_payload_t;

typedef struct __attribute__((packed)) {
	uint32_t acked_sequence; /**< Sequence number being acknowledged */
	uint32_t next_expected; /**< Next expected sequence number */
	uint16_t window_size; /**< Current receive window size */
	uint8_t error_code; /**< Error code (0 = success) */
	uint8_t reserved;
} fts_ack_payload_t;

typedef struct __attribute__((packed)) {
	uint8_t error_code; /**< Error code (fm_err_t) */
	uint8_t reserved[3];
	char message[0]; /**< Variable-length error message */
} fts_error_payload_t;

/**
 * @brief Resume metadata structure
 * 
 * Stored in .resume file to validate resume operations
 */
typedef struct {
	uint32_t magic; /**< Magic number for validation (0x52534D46 = "RSMF") */
	uint32_t version; /**< Metadata version */
	uint32_t file_size; /**< Expected total file size */
	uint32_t bytes_transferred; /**< Bytes transferred so far */
	uint32_t partial_crc32; /**< CRC32 of data transferred so far */
	uint32_t timestamp; /**< Last update timestamp */
	char remote_path[FTS_MAX_PATH_LEN]; /**< Remote file path */
	char local_path[FTS_MAX_PATH_LEN]; /**< Local file path */
} fts_resume_metadata_t;

#define FTS_RESUME_MAGIC 0x52534D46  /**< "RSMF" - Resume Metadata File */
#define FTS_RESUME_VERSION 1

/* Flags */
#define FTS_OPEN_FLAG_VERIFY_CRC    0x0001  /**< Enable CRC verification */
#define FTS_OPEN_FLAG_RESUME        0x0002  /**< Resume from offset */
#define FTS_DATA_FLAG_LAST          0x0001  /**< Last chunk in transfer */

/* Helper Functions */
uint16_t fts_crc16(const uint8_t *data, size_t len);
bool fts_validate_header(const fts_packet_header_t *header);
void fts_create_header(fts_packet_header_t *header, uint8_t cmd, uint32_t session_id, uint32_t sequence, uint16_t payload_len);

/* Resume Metadata Functions */
fm_err_t fts_save_resume_metadata(const fts_resume_metadata_t *metadata);
fm_err_t fts_load_resume_metadata(const char *local_path, fts_resume_metadata_t *metadata);
fm_err_t fts_delete_resume_metadata(const char *local_path);
bool fts_validate_resume(const fts_resume_metadata_t *metadata, const char *remote_path, uint32_t expected_size);

/* Security Functions */
bool fts_validate_path(const char *path);
bool fts_is_path_allowed(const char *path);
bool fts_check_rate_limit(uint32_t node_address);
bool fts_is_connection_secure(csp_conn_t *conn);
uint8_t fts_get_required_security_level(void);
void fts_set_required_security_level(uint8_t level);
#if defined __cplusplus
}
#endif

#endif /* FILE_TRANSFER_PROTOCOLS_H */


/*===========================================================================
 * ERROR CONVERSION HELPERS
 *===========================================================================*/

/**
 * @brief Convert int16_t status to fm_err_t
 */
static inline fm_err_t ft_int16_to_fm_err(int16_t status) {
	if (status < 0) {
		return FM_ERR_IO;
	}
	if (status > 127) {
		return FM_ERR_IO;
	}
	return (fm_err_t) status;
}

/**
 * @brief Convert int32_t result to fm_err_t
 */
static inline fm_err_t ft_int32_to_fm_err(int32_t result) {
	if (result < 0) {
		return FM_ERR_IO;
	}
	if (result > 255) {
		return FM_ERR_IO;
	}
	return (fm_err_t) result;
}

/**
 * @brief Convert fm_err_t to int16_t
 */
static inline int16_t ft_fm_err_to_int16(fm_err_t err) {
	return (int16_t) err;
}

/**
 * @brief Convert fm_err_t to int32_t
 */
static inline int32_t ft_fm_err_to_int32(fm_err_t err) {
	return (int32_t) err;
}

/**
 * @brief Convert fm_err_t to negative int32_t
 */
static inline int32_t ft_fm_err_to_neg_int32(fm_err_t err) {
	return -(int32_t) err;
}
