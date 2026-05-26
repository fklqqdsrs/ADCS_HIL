/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Streaming file transfer (Client + Server)
 * 
 * This file combines both client and server streaming implementations
 * to reduce code duplication and improve maintainability.
 */

#include "file_transfer_streaming.h"
#include "file_transfer_protocols.h"
#include "file_core.h"
#include "ft_config.h"
#include "crc32.h"
#include <string.h>
#include <stdlib.h>
#include <kservice.h>

/*===========================================================================
 * FORWARD DECLARATIONS
 *===========================================================================*/

void fts_server_handler(csp_conn_t *conn);

/*===========================================================================
 * SHARED PROTOCOL HELPERS
 *===========================================================================*/

/**
 * @brief Validate path for security
 * 
 * Checks for:
 * - Path traversal attempts (..)
 * - Absolute path requirement (must start with /)
 * - Double slashes
 * - Path length limits
 * - Invalid characters
 */
bool fts_validate_path(const char *path) {
    if (!path) {
        return false;
    }
    
    size_t len = strlen(path);
    
    /* Check path length */
    if (len == 0 || len >= FTS_MAX_PATH_LEN) {
        tsc_kprintf("[FTS_SECURITY] Path validation failed: invalid length (%zu)\n", len);
        return false;
    }
    
    /* Must start with / (absolute path) */
    if (path[0] != '/') {
        tsc_kprintf("[FTS_SECURITY] Path validation failed: not absolute path\n");
        return false;
    }
    
    /* Check for path traversal (..) */
    if (strstr(path, "..") != NULL) {
        tsc_kprintf("[FTS_SECURITY] Path validation failed: path traversal detected\n");
        return false;
    }
    
    /* Check for double slashes */
    if (strstr(path, "//") != NULL) {
        tsc_kprintf("[FTS_SECURITY] Path validation failed: double slashes\n");
        return false;
    }
    
    /* Check for null bytes in path */
    for (size_t i = 0; i < len; i++) {
        if (path[i] == '\0') {
            tsc_kprintf("[FTS_SECURITY] Path validation failed: null byte in path\n");
            return false;
        }
    }
    
    /* Check for invalid characters */
    for (size_t i = 0; i < len; i++) {
        char c = path[i];
        /* Allow: alphanumeric, /, _, -, . */
        if (!((c >= 'a' && c <= 'z') || 
              (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || 
              c == '/' || c == '_' || c == '-' || c == '.')) {
            tsc_kprintf("[FTS_SECURITY] Path validation failed: invalid character 0x%02X\n", (unsigned char)c);
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Check if path is in allowed list
 * 
 * Whitelist approach: only allow specific prefixes
 */
bool fts_is_path_allowed(const char *path) {
    if (!path) {
        return false;
    }
    
    /* Allowed path prefixes */
    const char *allowed_prefixes[] = {
        "/D/",      /* SD Card (FatFS) */
        "/C/",      /* NOR Flash (LittleFS) */
        NULL
    };
    
    /* Check if path starts with any allowed prefix */
    for (int i = 0; allowed_prefixes[i] != NULL; i++) {
        if (strncmp(path, allowed_prefixes[i], strlen(allowed_prefixes[i])) == 0) {
            return true;
        }
    }
    
    /* Allow root-level files (e.g., /config.json) */
    /* Check if path is in format: /filename (no subdirectories) */
    if (path[0] == '/' && strchr(path + 1, '/') == NULL) {
        return true;
    }
    
    tsc_kprintf("[FTS_SECURITY] Path not allowed: %s\n", path);
    return false;
}

/**
 * @brief Rate limiting structure
 */
typedef struct {
    uint32_t node_address;
    uint32_t request_count;
    uint32_t window_start_ms;
    bool in_use;
} fts_rate_limit_entry_t;

#define FTS_RATE_LIMIT_ENTRIES 8
static fts_rate_limit_entry_t g_rate_limits[FTS_RATE_LIMIT_ENTRIES];

/**
 * @brief Check rate limit for node
 */
bool fts_check_rate_limit(uint32_t node_address) {
    uint32_t current_time = HAL_GetTick();
    fts_rate_limit_entry_t *entry = NULL;
    
    /* Find existing entry for this node */
    for (int i = 0; i < FTS_RATE_LIMIT_ENTRIES; i++) {
        if (g_rate_limits[i].in_use && g_rate_limits[i].node_address == node_address) {
            entry = &g_rate_limits[i];
            break;
        }
    }
    
    /* If not found, allocate new entry */
    if (!entry) {
        for (int i = 0; i < FTS_RATE_LIMIT_ENTRIES; i++) {
            if (!g_rate_limits[i].in_use) {
                entry = &g_rate_limits[i];
                entry->in_use = true;
                entry->node_address = node_address;
                entry->request_count = 0;
                entry->window_start_ms = current_time;
                break;
            }
        }
    }
    
    /* If still no entry (table full), allow but log warning */
    if (!entry) {
        tsc_kprintf("[FTS_SECURITY] Rate limit table full, allowing request\n");
        return true;
    }
    
    /* Check if window expired */
    if (current_time - entry->window_start_ms > FTS_RATE_LIMIT_WINDOW_MS) {
        /* Reset window */
        entry->request_count = 0;
        entry->window_start_ms = current_time;
    }
    
    /* Increment request count */
    entry->request_count++;
    
    /* Check if limit exceeded */
    if (entry->request_count > FTS_MAX_REQUESTS_PER_MINUTE) {
        tsc_kprintf("[FTS_SECURITY] Rate limit exceeded for node %lu: %lu requests in window\n",
                   (unsigned long)node_address, (unsigned long)entry->request_count);
        return false;
    }
    
    return true;
}

/*===========================================================================
 * CSP SECURITY (HMAC/XTEA)
 *===========================================================================*/

/* Global security level requirement */
static uint8_t g_required_security_level = FTS_SECURITY_NONE;

/**
 * @brief Set required security level
 * 
 * @param level Security level (FTS_SECURITY_NONE, FTS_SECURITY_HMAC, 
 *              FTS_SECURITY_XTEA, or FTS_SECURITY_ALL)
 * 
 * Example:
 *   fts_set_required_security_level(FTS_SECURITY_ALL);  // Require both HMAC and XTEA
 */
void fts_set_required_security_level(uint8_t level) {
    g_required_security_level = level;
    tsc_kprintf("[FTS_SECURITY] Required security level set to: ");
    if (level == FTS_SECURITY_NONE) {
        tsc_kprintf("NONE\n");
    } else {
        if (level & FTS_SECURITY_HMAC) tsc_kprintf("HMAC ");
        if (level & FTS_SECURITY_XTEA) tsc_kprintf("XTEA ");
        tsc_kprintf("\n");
    }
}

/**
 * @brief Get required security level
 */
uint8_t fts_get_required_security_level(void) {
    return g_required_security_level;
}

/**
 * @brief Check if connection meets security requirements
 * 
 * Validates that the CSP connection has the required security features enabled.
 * 
 * @param conn CSP connection to check
 * @return true if connection is secure enough, false otherwise
 */
bool fts_is_connection_secure(csp_conn_t *conn) {
    if (!conn) {
        return false;
    }
    
    /* If no security required, always pass */
    if (g_required_security_level == FTS_SECURITY_NONE) {
        return true;
    }
    
    /* Get connection options */
    uint32_t conn_opts = csp_conn_flags(conn);
    
    /* Check HMAC requirement */
    if (g_required_security_level & FTS_SECURITY_HMAC) {
        if (!(conn_opts & CSP_FHMAC)) {
            tsc_kprintf("[FTS_SECURITY] Connection rejected: HMAC required but not enabled\n");
            return false;
        }
    }
    
    /* Check XTEA requirement */
    if (g_required_security_level & FTS_SECURITY_XTEA) {
        if (!(conn_opts & CSP_FXTEA)) {
            tsc_kprintf("[FTS_SECURITY] Connection rejected: XTEA required but not enabled\n");
            return false;
        }
    }
    
    /* Log security status */
    tsc_kprintf("[FTS_SECURITY] Connection accepted with: ");
    if (conn_opts & CSP_FHMAC) tsc_kprintf("HMAC ");
    if (conn_opts & CSP_FXTEA) tsc_kprintf("XTEA ");
    tsc_kprintf("\n");
    
    return true;
}

/**
 * @brief Generate resume metadata filename
 */
static void fts_get_resume_filename(const char *local_path, char *resume_path, size_t max_len) {
    snprintf(resume_path, max_len, "%s.resume", local_path);
}

/**
 * @brief Save resume metadata to file
 */
fm_err_t fts_save_resume_metadata(const fts_resume_metadata_t *metadata) {
    if (!metadata) {
        return FM_ERR_INVAL;
    }
    
    char resume_path[FM_MAX_PATH_LEN];
    fts_get_resume_filename(metadata->local_path, resume_path, sizeof(resume_path));
    
    fm_file_handle_t *fp = NULL;
    fm_err_t err = fm_open(resume_path, FM_O_WRITE | FM_O_CREATE, &fp);
    if (err != FM_OK) {
        return err;
    }
    
    uint32_t bw = 0;
    err = fm_write(fp, metadata, sizeof(fts_resume_metadata_t), &bw);
    fm_close(&fp);
    
    if (err != FM_OK || bw != sizeof(fts_resume_metadata_t)) {
        return (err != FM_OK) ? err : FM_ERR_IO;
    }
    
    return FM_OK;
}

/**
 * @brief Load resume metadata from file
 */
fm_err_t fts_load_resume_metadata(const char *local_path, fts_resume_metadata_t *metadata) {
    if (!local_path || !metadata) {
        return FM_ERR_INVAL;
    }
    
    char resume_path[FM_MAX_PATH_LEN];
    fts_get_resume_filename(local_path, resume_path, sizeof(resume_path));
    
    fm_file_handle_t *fp = NULL;
    fm_err_t err = fm_open(resume_path, FM_O_READ, &fp);
    if (err != FM_OK) {
        return err;
    }
    
    uint32_t br = 0;
    err = fm_read(fp, metadata, sizeof(fts_resume_metadata_t), &br);
    fm_close(&fp);
    
    if (err != FM_OK || br != sizeof(fts_resume_metadata_t)) {
        return (err != FM_OK) ? err : FM_ERR_IO;
    }
    
    /* Validate magic number and version */
    if (metadata->magic != FTS_RESUME_MAGIC) {
        return FM_ERR_INVAL;
    }
    
    if (metadata->version != FTS_RESUME_VERSION) {
        return FM_ERR_INVAL;
    }
    
    return FM_OK;
}

/**
 * @brief Delete resume metadata file
 */
fm_err_t fts_delete_resume_metadata(const char *local_path) {
    if (!local_path) {
        return FM_ERR_INVAL;
    }
    
    char resume_path[FM_MAX_PATH_LEN];
    fts_get_resume_filename(local_path, resume_path, sizeof(resume_path));
    
    return fm_rm(resume_path);
}

/**
 * @brief Validate resume metadata
 */
bool fts_validate_resume(const fts_resume_metadata_t *metadata, const char *remote_path, uint32_t expected_size) {
    if (!metadata || !remote_path) {
        return false;
    }
    
    /* Check magic and version */
    if (metadata->magic != FTS_RESUME_MAGIC) {
        tsc_kprintf("[FTS] Resume validation failed: invalid magic\n");
        return false;
    }
    
    if (metadata->version != FTS_RESUME_VERSION) {
        tsc_kprintf("[FTS] Resume validation failed: version mismatch\n");
        return false;
    }
    
    /* Check remote path matches */
    if (strcmp(metadata->remote_path, remote_path) != 0) {
        tsc_kprintf("[FTS] Resume validation failed: remote path mismatch\n");
        return false;
    }
    
    /* Check file size matches (if known) */
    if (expected_size > 0 && metadata->file_size != expected_size) {
        tsc_kprintf("[FTS] Resume validation failed: file size mismatch (expected %lu, got %lu)\n",
                   (unsigned long)expected_size, (unsigned long)metadata->file_size);
        return false;
    }
    
    /* Check bytes transferred is within bounds */
    if (metadata->bytes_transferred > metadata->file_size) {
        tsc_kprintf("[FTS] Resume validation failed: bytes_transferred > file_size\n");
        return false;
    }
    
    return true;
}

/**
 * @brief Verify partial CRC32 of existing file
 */
static bool fts_verify_partial_crc32(const char *local_path, uint32_t bytes_to_check, uint32_t expected_crc32) {
    fm_file_handle_t *fp = NULL;
    fm_err_t err = fm_open(local_path, FM_O_READ, &fp);
    if (err != FM_OK) {
        return false;
    }
    
    uint32_t crc = crc32_init();
    uint8_t buffer[256];
    uint32_t remaining = bytes_to_check;
    
    while (remaining > 0) {
        uint32_t to_read = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        uint32_t br = 0;
        
        err = fm_read(fp, buffer, to_read, &br);
        if (err != FM_OK || br == 0) {
            fm_close(&fp);
            return false;
        }
        
        crc = crc32_update(crc, buffer, br);
        remaining -= br;
    }
    
    fm_close(&fp);
    crc = crc32_finalize(crc);
    
    if (crc != expected_crc32) {
        tsc_kprintf("[FTS] CRC32 mismatch: expected 0x%08lX, got 0x%08lX\n",
                   (unsigned long)expected_crc32, (unsigned long)crc);
        return false;
    }
    
    return true;
}

/**
 * @brief Simple CRC16 implementation
 */
uint16_t fts_crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    
    return crc;
}

/**
 * @brief Create packet header
 */
void fts_create_header(
    fts_packet_header_t *header,
    uint8_t cmd,
    uint32_t session_id,
    uint32_t sequence,
    uint16_t payload_len)
{
    header->version = FTS_PROTOCOL_VERSION;
    header->cmd = cmd;
    header->flags = 0;
    header->session_id = session_id;
    header->sequence = sequence;
    header->payload_len = payload_len;
    
    /* Calculate header checksum (exclude checksum field itself) */
    header->checksum = fts_crc16((uint8_t*)header, 
                                 sizeof(fts_packet_header_t) - sizeof(uint16_t));
}

/**
 * @brief Validate packet header
 */
bool fts_validate_header(const fts_packet_header_t *header) {
    if (header->version != FTS_PROTOCOL_VERSION) {
        return false;
    }
    
    /* Verify checksum */
    uint16_t calc_crc = fts_crc16((uint8_t*)header,
                                  sizeof(fts_packet_header_t) - sizeof(uint16_t));
    
    return (calc_crc == header->checksum);
}

/**
 * @brief Streaming session with persistent connection
 */
 typedef struct {
    /* Connection */
    csp_conn_t *conn;
    uint32_t session_id;

    /* State */
    fts_state_t state;
    fts_direction_t direction;
    fm_err_t last_error;

    /* File handles */
    fm_file_handle_t *local_fp;
    uint32_t file_size;
    uint32_t current_offset;

    /* Flow control */
    uint32_t sequence;
    uint32_t last_acked;
    uint16_t window_size;
    uint16_t in_flight;

    /* Configuration */
    fts_config_t config;
    char local_path[FM_MAX_PATH_LEN];
    char remote_path[FM_MAX_PATH_LEN];

    /* Statistics */
    fts_stats_t stats;
    uint32_t start_time_ms;
    uint32_t partial_crc32;  /* Running CRC32 for resume validation */
    uint32_t retry_count;    /* Current retry count */
    uint32_t max_retries;    /* Maximum retries per chunk */

    /* Buffer */
    uint8_t *chunk_buffer;
    uint16_t chunk_buffer_size;

    bool in_use;
} fts_streaming_session_t;

/*===========================================================================
 * CLIENT IMPLEMENTATION
 *===========================================================================*/

static fts_streaming_session_t g_streaming_sessions[FTS_MAX_SESSIONS];
static uint32_t g_next_session_id = 1;

/**
 * @brief Log client-side transfer progress (common helper)
 */
static void fts_client_log_progress(const fts_streaming_session_t *session,
                                    const char *direction_label,
                                    uint32_t bytes_total_so_far)
{
    uint32_t now_ms = HAL_GetTick();
    uint32_t elapsed_ms = now_ms - session->start_time_ms;

    /* Protect against division by zero */
    uint32_t kb_per_sec = 0;
    if (elapsed_ms > 0) {
        /* bytes_total_so_far * 1000 / elapsed_ms / 1024 */
        uint32_t tmp = (bytes_total_so_far / 1024U);
        if (tmp > 0U) {
            kb_per_sec = (tmp * 1000U) / elapsed_ms;
        }
    }

    uint32_t percent = 0;
    if (session->file_size > 0U) {
        /* Use 64-bit math to avoid overflow for large files */
        uint64_t num = (uint64_t) bytes_total_so_far * 100ULL;
        percent = (uint32_t) (num / (uint64_t) session->file_size);
        if (percent > 100U) {
            percent = 100U;
        }
    }

    if (session->file_size > 0U) {
        tsc_kprintf(
            "\r\033[K[FTS_CLIENT] %s %3lu%% %lu/%lu bytes, %lu KB/s",
            direction_label,
            (unsigned long) percent,
            (unsigned long) bytes_total_so_far,
            (unsigned long) session->file_size,
            (unsigned long) kb_per_sec
        );
    } else {
        tsc_kprintf(
            "\r\033[K[FTS_CLIENT] %s %lu bytes, %lu KB/s (total size unknown)",
            direction_label,
            (unsigned long) bytes_total_so_far,
            (unsigned long) kb_per_sec
        );
    }
}

/**
 * @brief Allocate streaming session
 */
static fts_streaming_session_t* fts_alloc_streaming_session(void) {
    for (int i = 0; i < FTS_MAX_SESSIONS; i++) {
        if (!g_streaming_sessions[i].in_use) {
            memset(&g_streaming_sessions[i], 0, sizeof(fts_streaming_session_t));
            g_streaming_sessions[i].in_use = true;
            g_streaming_sessions[i].session_id = g_next_session_id++;
            return &g_streaming_sessions[i];
        }
    }
    return NULL;
}

/**
 * @brief Free streaming session
 */
static void fts_free_streaming_session(fts_streaming_session_t *session) {
    if (session && session->in_use) {
        if (session->conn) {
            csp_close(session->conn);
            session->conn = NULL;
        }
        if (session->local_fp) {
            fm_close(&session->local_fp);
        }
        if (session->chunk_buffer) {
            free(session->chunk_buffer);
            session->chunk_buffer = NULL;
        }
        session->in_use = false;
    }
}

/**
 * @brief Send packet over streaming connection
 */
static fm_err_t fts_send_packet(
    fts_streaming_session_t *session,
    const fts_packet_header_t *header,
    const void *payload)
{
    if (!session->conn) {
        return FM_ERR_IO;
    }
    
    size_t total_size = sizeof(fts_packet_header_t) + header->payload_len;
    
    if (total_size > 256) {
        tsc_kprintf("[FTS] Error: packet too large (%zu bytes)\n", total_size);
        return FM_ERR_INVAL;
    }
    
    csp_packet_t *packet = csp_buffer_get(total_size);
    if (!packet) {
        return FM_ERR_NOMEM;
    }
    
    memcpy(packet->data, header, sizeof(fts_packet_header_t));
    if (payload && header->payload_len > 0) {
        memcpy(packet->data + sizeof(fts_packet_header_t), payload, header->payload_len);
    }
    packet->length = total_size;
    
    /* Send with retry */
    for (int retries = 3; retries > 0; retries--) {
        if (csp_send(session->conn, packet, session->config.timeout_ms)) {
            return FM_OK;
        }
        if (retries > 1) {
           csp_sleep_ms(100);
        }
    }
    
    csp_buffer_free(packet);
    return FM_ERR_IO;
}

/**
 * @brief Receive packet from streaming connection
 */
static fm_err_t fts_recv_packet(
    fts_streaming_session_t *session,
    fts_packet_header_t *header,
    void *payload,
    size_t max_payload_size)
{
    if (!session->conn) {
        return FM_ERR_IO;
    }
    
    csp_packet_t *packet = csp_read(session->conn, session->config.timeout_ms);
    if (!packet) {
        return FM_ERR_IO;
    }
    
    if (packet->length < sizeof(fts_packet_header_t)) {
        csp_buffer_free(packet);
        return FM_ERR_INVAL;
    }
    
    memcpy(header, packet->data, sizeof(fts_packet_header_t));
    
    if (!fts_validate_header(header)) {
        csp_buffer_free(packet);
        return FM_ERR_INVAL;
    }
    
    if (payload && header->payload_len > 0) {
        size_t copy_len = (header->payload_len < max_payload_size) ? 
                         header->payload_len : max_payload_size;
        memcpy(payload, packet->data + sizeof(fts_packet_header_t), copy_len);
    }
    
    csp_buffer_free(packet);
    return FM_OK;
}

/**
 * @brief Open streaming session (send OPEN command)
 */
static fm_err_t fts_streaming_open(fts_streaming_session_t *session) {
    size_t path_len = strlen(session->remote_path) + 1;
    if (path_len > FTS_MAX_PATH_LEN) {
        return FM_ERR_NAMETOOLONG;
    }
    
    /* Determine CSP connection options based on security requirements */
    uint32_t csp_opts = CSP_O_NONE;
    if (session->config.priority < 2) {
        /* Use HMAC and XTEA for high priority transfers */
        csp_opts = CSP_O_HMAC | CSP_O_XTEA;
        tsc_kprintf("[FTS_CLIENT] Using secure connection (HMAC + XTEA)\n");
    }
    
    session->conn = csp_connect(
        session->config.priority,
        session->config.node,
        FTS_CSP_PORT,
        session->config.timeout_ms,
        csp_opts
    );
    
    if (!session->conn) {
        return FM_ERR_IO;
    }
    
    /* Log connection security status */
    uint32_t conn_flags = csp_conn_flags(session->conn);
    tsc_kprintf("[FTS_CLIENT] Connection established with: ");
    if (conn_flags & CSP_FHMAC) tsc_kprintf("HMAC ");
    if (conn_flags & CSP_FXTEA) tsc_kprintf("XTEA ");
    if (conn_flags == CSP_O_NONE) tsc_kprintf("NONE ");
    tsc_kprintf("\n");
    
    /* Prepare OPEN packet */
    size_t payload_size = sizeof(fts_open_payload_t) + path_len;
    uint8_t *payload_buf = malloc(payload_size);
    if (!payload_buf) {
        csp_close(session->conn);
        session->conn = NULL;
        return FM_ERR_NOMEM;
    }
    
    fts_open_payload_t *open_payload = (fts_open_payload_t*)payload_buf;
    open_payload->file_size = session->file_size;
    open_payload->resume_offset = session->config.resume_offset;
    open_payload->chunk_size = session->config.chunk_size;
    open_payload->flags = 0;
    
    if (session->config.verify_crc) {
        open_payload->flags |= FTS_OPEN_FLAG_VERIFY_CRC;
    }
    if (session->config.resume_offset > 0) {
        open_payload->flags |= FTS_OPEN_FLAG_RESUME;
    }
    
    memcpy(open_payload->path, session->remote_path, path_len);
    
    fts_packet_header_t header;
    uint8_t cmd = (session->direction == FTS_DIRECTION_UPLOAD) ? 
                  FTS_CMD_OPEN_UPLOAD : FTS_CMD_OPEN_DOWNLOAD;
    
    fts_create_header(&header, cmd, session->session_id, session->sequence++, payload_size);
    
    fm_err_t err = fts_send_packet(session, &header, payload_buf);
    free(payload_buf);
    
    if (err != FM_OK) {
        csp_close(session->conn);
        session->conn = NULL;
        return err;
    }
    
    /* Wait for ACK */
    fts_ack_payload_t ack;
    err = fts_recv_packet(session, &header, &ack, sizeof(ack));
    
    if (err != FM_OK || header.cmd != FTS_CMD_ACK) {
        csp_close(session->conn);
        session->conn = NULL;
        return (err != FM_OK) ? err : FM_ERR_IO;
    }
    
    session->window_size = ack.window_size;
    session->last_acked = ack.acked_sequence;
    
    return FM_OK;
}

/**
 * @brief Build and send DATA packet
 */
static fm_err_t fts_build_and_send_data_packet(
    fts_streaming_session_t *session,
    const uint8_t *data,
    uint16_t data_len,
    bool is_last)
{
    size_t payload_size = sizeof(fts_data_payload_t) + data_len;
    uint8_t *payload_buf = malloc(payload_size);
    if (!payload_buf) {
        return FM_ERR_NOMEM;
    }
    
    fts_data_payload_t *data_payload = (fts_data_payload_t*)payload_buf;
    data_payload->offset = session->current_offset;
    data_payload->data_len = data_len;
    data_payload->crc16 = fts_crc16(data, data_len);
    memcpy(data_payload->data, data, data_len);
    
    fts_packet_header_t header;
    fts_create_header(&header, FTS_CMD_DATA, session->session_id,
                     session->sequence++, payload_size);
    
    if (is_last) {
        header.flags |= FTS_DATA_FLAG_LAST;
        header.checksum = fts_crc16((uint8_t*)&header, 
                                   sizeof(fts_packet_header_t) - sizeof(uint16_t));
    }
    
    fm_err_t err = fts_send_packet(session, &header, payload_buf);
    free(payload_buf);
    
    if (err == FM_OK) {
        session->in_flight++;
        session->current_offset += data_len;
    }
    
    return err;
}

/**
 * @brief Wait and process ACK packets
 */
static fm_err_t fts_wait_for_acks(fts_streaming_session_t *session)
{
    while (session->in_flight > 0) {
        fts_packet_header_t header;
        fts_ack_payload_t ack;
        
        fm_err_t err = fts_recv_packet(session, &header, &ack, sizeof(ack));
        if (err != FM_OK) {
            tsc_kprintf("[FTS] Warning: timeout waiting for ACKs\n");
            return err;
        }
        
        if (header.cmd == FTS_CMD_ACK) {
            session->last_acked = ack.acked_sequence;
            session->in_flight = session->sequence - session->last_acked - 1;
        }
    }
    return FM_OK;
}

/**
 * @brief Send data chunk with flow control and retry
 */
static fm_err_t fts_streaming_send_chunk(
    fts_streaming_session_t *session,
    const uint8_t *data,
    uint16_t data_len,
    bool is_last)
{
    /* Check flow control window */
    while (session->in_flight >= session->window_size) {
        fts_packet_header_t header;
        fts_ack_payload_t ack;
        
        fm_err_t err = fts_recv_packet(session, &header, &ack, sizeof(ack));
        if (err != FM_OK) {
            /* Retry on timeout */
            if (session->retry_count < session->max_retries) {
                session->retry_count++;
                tsc_kprintf("[FTS_CLIENT] ACK timeout, retry %lu/%lu\n",
                           (unsigned long)session->retry_count,
                           (unsigned long)session->max_retries);
                csp_sleep_ms(100);
                continue;
            }
            return err;
        }
        
        /* Reset retry count on success */
        session->retry_count = 0;
        
        if (header.cmd == FTS_CMD_ACK) {
            session->last_acked = ack.acked_sequence;
            session->window_size = ack.window_size;
            session->in_flight = session->sequence - session->last_acked - 1;
        } else if (header.cmd == FTS_CMD_NACK) {
            tsc_kprintf("[FTS_CLIENT] Received NACK, retransmitting\n");
            return FM_ERR_IO;
        }
    }
    
    /* Send with retry */
    fm_err_t err = FM_ERR_IO;
    for (uint32_t retry = 0; retry <= session->max_retries; retry++) {
        err = fts_build_and_send_data_packet(session, data, data_len, is_last);
        if (err == FM_OK) {
            session->retry_count = 0;
            break;
        }
        
        if (retry < session->max_retries) {
            tsc_kprintf("\n[FTS_CLIENT] Send failed, retry %lu/%lu\n",
                       (unsigned long)(retry + 1),
                       (unsigned long)session->max_retries);
            csp_sleep_ms(100);
        }
    }
    
    if (err == FM_OK) {
        /* Use bytes already transferred plus this chunk for progress */
        uint32_t total_bytes = session->stats.bytes_transferred + data_len;
        fts_client_log_progress(session, "TX",
                                total_bytes);
    }
    
    return err;
}


/**
 * @brief Close streaming session
 */
static fm_err_t fts_streaming_close(fts_streaming_session_t *session) {
    if (!session->conn) {
        return FM_OK;
    }
    
    csp_sleep_ms(100);
    csp_close(session->conn);
    session->conn = NULL;
    
    return FM_OK;
}

/*===========================================================================
 * PUBLIC CLIENT API
 *===========================================================================*/

fm_err_t fts_create_streaming(
    fts_direction_t direction,
    const char *local_path,
    const char *remote_path,
    const fts_config_t *config,
    fts_session_t **session)
{
    fts_streaming_session_t *s = fts_alloc_streaming_session();
    if (!s) {
        return FM_ERR_TOO_MANY_OPEN;
    }
    
    s->direction = direction;
    s->state = FTS_STATE_IDLE;
    strncpy(s->local_path, local_path, sizeof(s->local_path) - 1);
    strncpy(s->remote_path, remote_path, sizeof(s->remote_path) - 1);
    
    if (config) {
        s->config = *config;
    } else {
        fts_config_t default_config = FTS_DEFAULT_CONFIG;
        s->config = default_config;
    }
    
    s->chunk_buffer = malloc(s->config.chunk_size);
    if (!s->chunk_buffer) {
        fts_free_streaming_session(s);
        return FM_ERR_NOMEM;
    }
    s->chunk_buffer_size = s->config.chunk_size;
    
    /* Initialize retry settings */
    s->retry_count = 0;
    s->max_retries = 3;  /* Default 3 retries per chunk */
    
    *session = (fts_session_t*)s;
    return FM_OK;
}

fm_err_t fts_start_streaming(fts_session_t *session) {
    fts_streaming_session_t *s = (fts_streaming_session_t*)session;
    
    if (!s || !s->in_use) {
        return FM_ERR_INVAL;
    }
    
    uint8_t mode = (s->direction == FTS_DIRECTION_UPLOAD) ? 
                   FM_O_READ : (FM_O_WRITE | FM_O_CREATE);
    
    fm_err_t err = fm_open(s->local_path, mode, &s->local_fp);
    if (err != FM_OK) {
        return err;
    }
    
    if (s->direction == FTS_DIRECTION_UPLOAD) {
        fm_file_info_t info;
        if (fm_stat(s->local_path, &info) == FM_OK) {
            s->file_size = info.size;
        }
        
        /* Handle resume for upload */
        if (s->config.resume_offset > 0) {
            /* Try to load resume metadata */
            fts_resume_metadata_t metadata;
            err = fts_load_resume_metadata(s->local_path, &metadata);
            
            if (err == FM_OK) {
                /* Validate resume metadata */
                if (fts_validate_resume(&metadata, s->remote_path, s->file_size)) {
                    /* Verify partial CRC32 */
                    if (fts_verify_partial_crc32(s->local_path, metadata.bytes_transferred, metadata.partial_crc32)) {
                        s->config.resume_offset = metadata.bytes_transferred;
                        tsc_kprintf("[FTS] Resume validated: offset=%lu, CRC32=0x%08lX\n",
                                   (unsigned long)s->config.resume_offset,
                                   (unsigned long)metadata.partial_crc32);
                    } else {
                        tsc_kprintf("[FTS] Resume CRC32 validation failed, starting from beginning\n");
                        s->config.resume_offset = 0;
                        fts_delete_resume_metadata(s->local_path);
                    }
                } else {
                    tsc_kprintf("[FTS] Resume metadata validation failed, starting from beginning\n");
                    s->config.resume_offset = 0;
                    fts_delete_resume_metadata(s->local_path);
                }
            } else {
                tsc_kprintf("[FTS] No resume metadata found, using specified offset %lu\n",
                           (unsigned long)s->config.resume_offset);
            }
            
            /* Validate resume offset */
            if (s->config.resume_offset >= s->file_size) {
                tsc_kprintf("[FTS] Resume offset (%lu) >= file size (%lu), starting from beginning\n",
                           (unsigned long)s->config.resume_offset, (unsigned long)s->file_size);
                s->config.resume_offset = 0;
            } else if (s->config.resume_offset > 0) {
                err = fm_seek(s->local_fp, s->config.resume_offset);
                if (err != FM_OK) {
                    tsc_kprintf("[FTS] Failed to seek to resume offset %lu, starting from beginning\n",
                               (unsigned long)s->config.resume_offset);
                    s->config.resume_offset = 0;
                } else {
                    s->current_offset = s->config.resume_offset;
                    tsc_kprintf("[FTS] Resuming upload from offset %lu\n", (unsigned long)s->config.resume_offset);
                }
            }
        }
    } else {
        /* Handle resume for download */
        if (s->config.resume_offset > 0) {
            /* Try to load resume metadata */
            fts_resume_metadata_t metadata;
            err = fts_load_resume_metadata(s->local_path, &metadata);
            
            if (err == FM_OK) {
                /* Validate resume metadata */
                if (fts_validate_resume(&metadata, s->remote_path, 0)) {
                    /* Verify partial CRC32 */
                    if (fts_verify_partial_crc32(s->local_path, metadata.bytes_transferred, metadata.partial_crc32)) {
                        s->config.resume_offset = metadata.bytes_transferred;
                        tsc_kprintf("[FTS] Resume validated: offset=%lu, CRC32=0x%08lX\n",
                                   (unsigned long)s->config.resume_offset,
                                   (unsigned long)metadata.partial_crc32);
                    } else {
                        tsc_kprintf("[FTS] Resume CRC32 validation failed, starting from beginning\n");
                        s->config.resume_offset = 0;
                        fts_delete_resume_metadata(s->local_path);
                    }
                } else {
                    tsc_kprintf("[FTS] Resume metadata validation failed, starting from beginning\n");
                    s->config.resume_offset = 0;
                    fts_delete_resume_metadata(s->local_path);
                }
            } else {
                /* Check if local file exists and use its size as resume offset */
                fm_file_info_t local_info;
                if (fm_stat(s->local_path, &local_info) == FM_OK && !local_info.is_dir) {
                    s->config.resume_offset = local_info.size;
                    tsc_kprintf("[FTS] No resume metadata, using local file size: %lu bytes\n",
                               (unsigned long)s->config.resume_offset);
                } else {
                    s->config.resume_offset = 0;
                }
            }
            
            /* Seek to resume offset */
            if (s->config.resume_offset > 0) {
                err = fm_seek(s->local_fp, s->config.resume_offset);
                if (err != FM_OK) {
                    tsc_kprintf("[FTS] Failed to seek to resume offset %lu, starting from beginning\n",
                               (unsigned long)s->config.resume_offset);
                    s->config.resume_offset = 0;
                } else {
                    s->current_offset = s->config.resume_offset;
                    tsc_kprintf("[FTS] Resuming download from offset %lu\n", (unsigned long)s->config.resume_offset);
                }
            }
        }
    }
    
    err = fts_streaming_open(s);
    if (err != FM_OK) {
        fm_close(&s->local_fp);
        return err;
    }
    
    s->state = FTS_STATE_TRANSFERRING;
    s->start_time_ms = HAL_GetTick();
    
    return FM_OK;
}

fm_err_t fts_process_streaming(fts_session_t *session) {
    fts_streaming_session_t *s = (fts_streaming_session_t*)session;
    
    if (!s || !s->in_use || s->state != FTS_STATE_TRANSFERRING) {
        return FM_ERR_INVAL;
    }
    
    if (s->direction == FTS_DIRECTION_UPLOAD) {
        uint32_t bytes_read = 0;
        fm_err_t err = fm_read(s->local_fp, s->chunk_buffer,
                              s->chunk_buffer_size, &bytes_read);
        
        if (err != FM_OK) {
            return err;
        }
        
        if (bytes_read == 0) {
            fts_wait_for_acks(s);
            fts_streaming_close(s);
            s->state = FTS_STATE_COMPLETED;
            
            /* Final upload summary */
            uint32_t elapsed_ms = HAL_GetTick() - s->start_time_ms;
            uint32_t kb_per_sec = 0;
            if (elapsed_ms > 0U) {
                uint32_t tmp = (s->stats.bytes_transferred / 1024U);
                if (tmp > 0U) {
                    kb_per_sec = (tmp * 1000U) / elapsed_ms;
                }
            }
            uint32_t percent = 0;
            if (s->file_size > 0U) {
                uint64_t num = (uint64_t) s->stats.bytes_transferred * 100ULL;
                percent = (uint32_t) (num / (uint64_t) s->file_size);
                if (percent > 100U) {
                    percent = 100U;
                }
            }
            tsc_kprintf(
                "\n[FTS_CLIENT] Upload complete: %3lu%% %lu/%lu bytes, %lu KB/s\n",
                (unsigned long) percent,
                (unsigned long) s->stats.bytes_transferred,
                (unsigned long) s->file_size,
                (unsigned long) kb_per_sec
            );
            
            /* Delete resume metadata on successful completion */
            fts_delete_resume_metadata(s->local_path);
            
            return FM_ERR_GENERIC;
        }
        
        bool is_last = (bytes_read < s->chunk_buffer_size);
        err = fts_streaming_send_chunk(s, s->chunk_buffer, bytes_read, is_last);
        
        if (err != FM_OK) {
            return err;
        }
        
        s->stats.bytes_transferred += bytes_read;
        s->stats.chunks_sent++;
        
        /* Update partial CRC32 */
        if (s->stats.chunks_sent == 1) {
            s->partial_crc32 = crc32_init();
        }
        s->partial_crc32 = crc32_update(s->partial_crc32, s->chunk_buffer, bytes_read);
        
        /* Save resume metadata every 10 chunks */
        if (s->stats.chunks_sent % 10 == 0) {
            fts_resume_metadata_t metadata = {
                .magic = FTS_RESUME_MAGIC,
                .version = FTS_RESUME_VERSION,
                .file_size = s->file_size,
                .bytes_transferred = s->stats.bytes_transferred,
                .partial_crc32 = crc32_finalize(s->partial_crc32),
                .timestamp = HAL_GetTick()
            };
            strncpy(metadata.remote_path, s->remote_path, sizeof(metadata.remote_path) - 1);
            strncpy(metadata.local_path, s->local_path, sizeof(metadata.local_path) - 1);
            
            fts_save_resume_metadata(&metadata);
        }
        
      //  csp_sleep_ms(1);
        
    } else {
        fts_packet_header_t header;
        uint8_t payload_buf[sizeof(fts_data_payload_t) + FTS_MAX_CHUNK_SIZE];
        
        fm_err_t err = fts_recv_packet(s, &header, payload_buf, sizeof(payload_buf));
        if (err != FM_OK) {
            return err;
        }
        
        if (header.cmd == FTS_CMD_DATA) {
            fts_data_payload_t *data_payload = (fts_data_payload_t*)payload_buf;
            
            if (s->config.verify_crc) {
                uint16_t calc_crc = fts_crc16(data_payload->data, data_payload->data_len);
                if (calc_crc != data_payload->crc16) {
                    return FM_ERR_IO;
                }
            }
            
            uint32_t bytes_written = 0;
            err = fm_write(s->local_fp, data_payload->data,
                          data_payload->data_len, &bytes_written);
            
            if (err != FM_OK) {
                return err;
            }
            
            fts_ack_payload_t ack = {
                .acked_sequence = header.sequence,
                .next_expected = header.sequence + 1,
                .window_size = FTS_WINDOW_SIZE,
                .error_code = 0
            };
            
            fts_packet_header_t ack_header;
            fts_create_header(&ack_header, FTS_CMD_ACK, s->session_id,
                            s->sequence++, sizeof(ack));
            
            fts_send_packet(s, &ack_header, &ack);
            
            s->stats.bytes_transferred += bytes_written;
            
            /* Client-side progress for download (RX) */
            fts_client_log_progress(s, "RX",
                                    s->stats.bytes_transferred);
            
            /* Update partial CRC32 */
            if (s->stats.bytes_transferred == bytes_written) {
                s->partial_crc32 = crc32_init();
            }
            s->partial_crc32 = crc32_update(s->partial_crc32, data_payload->data, bytes_written);
            
            /* Save resume metadata every 10 chunks */
            if ((s->stats.bytes_transferred / s->chunk_buffer_size) % 10 == 0) {
                fts_resume_metadata_t metadata = {
                    .magic = FTS_RESUME_MAGIC,
                    .version = FTS_RESUME_VERSION,
                    .file_size = 0,  /* Unknown for download */
                    .bytes_transferred = s->stats.bytes_transferred,
                    .partial_crc32 = crc32_finalize(s->partial_crc32),
                    .timestamp = HAL_GetTick()
                };
                strncpy(metadata.remote_path, s->remote_path, sizeof(metadata.remote_path) - 1);
                strncpy(metadata.local_path, s->local_path, sizeof(metadata.local_path) - 1);
                
                fts_save_resume_metadata(&metadata);
            }
            
            if (header.flags & FTS_DATA_FLAG_LAST) {
                fts_streaming_close(s);
                s->state = FTS_STATE_COMPLETED;
                
                /* Final download summary */
                uint32_t elapsed_ms = HAL_GetTick() - s->start_time_ms;
                uint32_t kb_per_sec = 0;
                if (elapsed_ms > 0U) {
                    uint32_t tmp = (s->stats.bytes_transferred / 1024U);
                    if (tmp > 0U) {
                        kb_per_sec = (tmp * 1000U) / elapsed_ms;
                    }
                }
                tsc_kprintf(
                    "\n[FTS_CLIENT] Download complete: %lu bytes, %lu KB/s (total size unknown)\n",
                    (unsigned long) s->stats.bytes_transferred,
                    (unsigned long) kb_per_sec
                );
                
                /* Delete resume metadata on successful completion */
                fts_delete_resume_metadata(s->local_path);
                
                return FM_ERR_GENERIC;
            }
        }
    }
    
    return FM_OK;
}

fm_err_t fts_destroy_streaming(fts_session_t **session) {
    if (!session || !*session) {
        return FM_OK;
    }
    
    fts_streaming_session_t *s = (fts_streaming_session_t*)*session;
    fts_free_streaming_session(s);
    *session = NULL;
    
    return FM_OK;
}

bool fts_is_active(fts_session_t *session) {
    if (!session) return false;
    fts_streaming_session_t *s = (fts_streaming_session_t *)session;
    return (s->state == FTS_STATE_CONNECTING || 
            s->state == FTS_STATE_TRANSFERRING || 
            s->state == FTS_STATE_PAUSED);
}

bool fts_is_complete(fts_session_t *session) {
    if (!session) return false;
    fts_streaming_session_t *s = (fts_streaming_session_t *)session;
    return s->state == FTS_STATE_COMPLETED;
}

fm_err_t fts_get_error(fts_session_t *session) {
    if (!session) return FM_ERR_INVAL;
    fts_streaming_session_t *s = (fts_streaming_session_t *)session;
    return s->last_error;
}

fts_state_t fts_get_state(fts_session_t *session) {
    if (!session) return FTS_STATE_IDLE;
    fts_streaming_session_t *s = (fts_streaming_session_t *)session;
    return s->state;
}

/*===========================================================================
 * SERVER IMPLEMENTATION
 *===========================================================================*/

#define FTS_SERVER_MAX_SESSIONS 4

typedef struct {
    csp_conn_t *conn;
    uint32_t session_id;
    fm_file_handle_t *fp;
    char path[FM_MAX_PATH_LEN];
    bool is_upload;
    uint32_t file_offset;
    uint32_t file_size;
    uint32_t last_sequence;
    uint16_t window_size;
    uint16_t chunk_size;
    bool verify_crc;
    bool in_use;
    uint32_t last_activity_ms;
} fts_server_session_t;

static fts_server_session_t g_server_sessions[FTS_SERVER_MAX_SESSIONS];

/**
 * @brief Find or allocate server session
 */
static fts_server_session_t* fts_server_get_session(
    csp_conn_t *conn, uint32_t session_id, bool create)
{
    for (int i = 0; i < FTS_SERVER_MAX_SESSIONS; i++) {
        if (g_server_sessions[i].in_use &&
            g_server_sessions[i].session_id == session_id) {
            g_server_sessions[i].last_activity_ms = HAL_GetTick();
            return &g_server_sessions[i];
        }
    }
    
    if (create) {
        for (int i = 0; i < FTS_SERVER_MAX_SESSIONS; i++) {
            if (!g_server_sessions[i].in_use) {
                memset(&g_server_sessions[i], 0, sizeof(fts_server_session_t));
                g_server_sessions[i].in_use = true;
                g_server_sessions[i].conn = conn;
                g_server_sessions[i].session_id = session_id;
                g_server_sessions[i].window_size = FTS_WINDOW_SIZE;
                g_server_sessions[i].last_activity_ms = HAL_GetTick();
                return &g_server_sessions[i];
            }
        }
    }
    
    return NULL;
}

/**
 * @brief Free server session
 */
static void fts_server_free_session(fts_server_session_t *session) {
    if (session && session->in_use) {
        if (session->fp) {
            fm_close(&session->fp);
        }
        session->in_use = false;
    }
}

/**
 * @brief Cleanup stale sessions (timeout check)
 */
static void fts_cleanup_stale_sessions(void) {
    uint32_t current_time = HAL_GetTick();
    
    for (int i = 0; i < FTS_SERVER_MAX_SESSIONS; i++) {
        if (g_server_sessions[i].in_use) {
            uint32_t idle_time = current_time - g_server_sessions[i].last_activity_ms;
            
            if (idle_time > FTS_SESSION_TIMEOUT_MS) {
                tsc_kprintf("[FTS_SERVER] Session %lu timeout (idle: %lu ms), cleaning up\n",
                           (unsigned long)g_server_sessions[i].session_id,
                           (unsigned long)idle_time);
                fts_server_free_session(&g_server_sessions[i]);
            }
        }
    }
}

/**
 * @brief Check if cleanup is needed
 */
static bool fts_should_cleanup(void) {
    static uint32_t last_cleanup_time = 0;
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - last_cleanup_time > FTS_SESSION_CLEANUP_INTERVAL_MS) {
        last_cleanup_time = current_time;
        return true;
    }
    
    return false;
}

/**
 * @brief Send response packet
 */
static fm_err_t fts_server_send_response(
    csp_conn_t *conn, uint8_t cmd, uint32_t session_id,
    uint32_t sequence, const void *payload, uint16_t payload_len)
{
    size_t total_size = sizeof(fts_packet_header_t) + payload_len;
    
    csp_packet_t *packet = csp_buffer_get(total_size);
    if (!packet) {
        return FM_ERR_NOMEM;
    }
    
    fts_packet_header_t *header = (fts_packet_header_t*)packet->data;
    fts_create_header(header, cmd, session_id, sequence, payload_len);
    
    if (payload && payload_len > 0) {
        memcpy(packet->data + sizeof(fts_packet_header_t), payload, payload_len);
    }
    
    packet->length = total_size;
    
    if (!csp_send(conn, packet, 1000)) {
        csp_buffer_free(packet);
        return FM_ERR_IO;
    }
    
    return FM_OK;
}

/**
 * @brief Send ACK
 */
static fm_err_t fts_server_send_ack(
    fts_server_session_t *session, uint32_t acked_sequence, uint8_t error_code)
{
    fts_ack_payload_t ack = {
        .acked_sequence = acked_sequence,
        .next_expected = acked_sequence + 1,
        .window_size = session->window_size,
        .error_code = error_code
    };
    
    fm_err_t result = fts_server_send_response(session->conn, FTS_CMD_ACK,
                                   session->session_id, 0, &ack, sizeof(ack));
    
    /* Update activity timestamp on successful send */
    if (result == FM_OK) {
        session->last_activity_ms = HAL_GetTick();
    }
    
    return result;
}

/**
 * @brief Send error response
 */
static fm_err_t fts_server_send_error(
    csp_conn_t *conn, uint32_t session_id, uint8_t error_code, const char *message)
{
    size_t msg_len = message ? strlen(message) + 1 : 0;
    size_t payload_size = sizeof(fts_error_payload_t) + msg_len;
    
    uint8_t *payload_buf = malloc(payload_size);
    if (!payload_buf) {
        return FM_ERR_NOMEM;
    }
    
    fts_error_payload_t *error = (fts_error_payload_t*)payload_buf;
    error->error_code = error_code;
    memset(error->reserved, 0, sizeof(error->reserved));
    
    if (message && msg_len > 0) {
        memcpy(error->message, message, msg_len);
    }
    
    fm_err_t err = fts_server_send_response(conn, FTS_CMD_ERROR, session_id, 0,
                                           payload_buf, payload_size);
    
    free(payload_buf);
    return err;
}

/**
 * @brief Send DATA packet with retry logic
 */
static fm_err_t fts_server_send_data_packet(
    fts_server_session_t *session, uint32_t sequence,
    const uint8_t *data, uint16_t data_len, bool is_last)
{
    size_t payload_size = sizeof(fts_data_payload_t) + data_len;
    uint8_t *payload_buf = malloc(payload_size);
    if (!payload_buf) {
        return FM_ERR_NOMEM;
    }
    
    fts_data_payload_t *data_payload = (fts_data_payload_t*)payload_buf;
    data_payload->offset = session->file_offset;
    data_payload->data_len = data_len;
    data_payload->crc16 = fts_crc16(data, data_len);
    memcpy(data_payload->data, data, data_len);
    
    fts_packet_header_t header;
    fts_create_header(&header, FTS_CMD_DATA, session->session_id, sequence, payload_size);
    
    if (is_last) {
        header.flags |= FTS_DATA_FLAG_LAST;
        header.checksum = fts_crc16((uint8_t*)&header, 
                                   sizeof(fts_packet_header_t) - sizeof(uint16_t));
    }
    
    size_t total_size = sizeof(fts_packet_header_t) + payload_size;
    csp_packet_t *packet = csp_buffer_get(total_size);
    if (!packet) {
        free(payload_buf);
        return FM_ERR_NOMEM;
    }
    
    memcpy(packet->data, &header, sizeof(fts_packet_header_t));
    memcpy(packet->data + sizeof(fts_packet_header_t), payload_buf, payload_size);
    packet->length = total_size;
    free(payload_buf);
    
    for (int retries = 3; retries > 0; retries--) {
        if (csp_send(session->conn, packet, 2000)) {
            /* Update activity timestamp on successful send */
            session->last_activity_ms = HAL_GetTick();
            return FM_OK;
        }
        if (retries > 1) {
            csp_sleep_ms(100);
        }
    }
    
    csp_buffer_free(packet);
    return FM_ERR_IO;
}

/**
 * @brief Wait for ACK from client
 */
static fm_err_t fts_server_wait_ack(fts_server_session_t *session)
{
    csp_packet_t *ack_packet = csp_read(session->conn, 2000);
    if (!ack_packet) {
        return FM_ERR_IO;
    }
    
    fm_err_t result = FM_ERR_IO;
    if (ack_packet->length >= sizeof(fts_packet_header_t)) {
        fts_packet_header_t *ack_header = (fts_packet_header_t*)ack_packet->data;
        if (ack_header->cmd == FTS_CMD_ACK) {
            result = FM_OK;
        }
    }
    
    csp_buffer_free(ack_packet);
    return result;
}

/**
 * @brief Send file data to client (download)
 */
static fm_err_t fts_server_send_file_data(fts_server_session_t *session)
{
    if (!session || !session->fp || session->is_upload) {
        return FM_ERR_INVAL;
    }
    
    uint8_t *chunk_buffer = malloc(session->chunk_size);
    if (!chunk_buffer) {
        return FM_ERR_NOMEM;
    }
    
    uint32_t sequence = 0;
    uint32_t bytes_sent = 0;
    fm_err_t err = FM_OK;
    
    while (1) {
        uint32_t bytes_read = 0;
        err = fm_read(session->fp, chunk_buffer, session->chunk_size, &bytes_read);
        
        if (err != FM_OK || bytes_read == 0) {
            break;
        }
        
        bool is_last = (bytes_read < session->chunk_size);
        
    tsc_kprintf(
            "\r\033[K[FTS_SERVER] TX seq=%8lu size=%6lu last=%d offset=%10lu",
            (unsigned long)sequence,
            (unsigned long)bytes_read,
            is_last,
            (unsigned long)session->file_offset
        );
        
        err = fts_server_send_data_packet(session, sequence++, chunk_buffer, bytes_read, is_last);
        if (err != FM_OK) {
            break;
        }
        
        session->file_offset += bytes_read;
        bytes_sent += bytes_read;
        
        err = fts_server_wait_ack(session);
        if (err != FM_OK) {
            tsc_kprintf("[FTS_SERVER] ACK timeout\n");
            break;
        }
        
        //csp_sleep_ms(1);
        
        if (is_last) {
             tsc_kprintf(
                "\r\033[K[FTS_SERVER] Download complete: %lu bytes\n",
                (unsigned long)bytes_sent
            );
            break;
        }
    }
    
    free(chunk_buffer);
    return err;
}

/**
 * @brief Handle OPEN_UPLOAD command
 */
static void fts_server_handle_open_upload(
    csp_conn_t *conn, const fts_packet_header_t *header,
    const fts_open_payload_t *open_payload)
{
    tsc_kprintf("[FTS_SERVER] OPEN_UPLOAD: session=%lu, path=%s\n",
               (unsigned long)header->session_id, open_payload->path);
    
    /* Security: Check connection security (HMAC/XTEA) */
    if (!fts_is_connection_secure(conn)) {
        tsc_kprintf("[FTS_SERVER] Connection security check failed\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Insufficient security");
        return;
    }
    
    /* Security: Validate path */
    if (!fts_validate_path(open_payload->path)) {
        tsc_kprintf("[FTS_SERVER] Path validation failed\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Invalid path");
        return;
    }
    
    /* Security: Check path whitelist */
    if (!fts_is_path_allowed(open_payload->path)) {
        tsc_kprintf("[FTS_SERVER] Path not in whitelist\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Path not allowed");
        return;
    }
    
    /* Security: Check rate limit */
    if (!fts_check_rate_limit(csp_conn_src(conn))) {
        tsc_kprintf("[FTS_SERVER] Rate limit exceeded\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_BUSY, "Rate limit exceeded");
        return;
    }
    
    fts_server_session_t *session = fts_server_get_session(conn, header->session_id, true);
    
    if (!session) {
        fts_server_send_error(conn, header->session_id, FM_ERR_TOO_MANY_OPEN,
                            "Too many sessions");
        return;
    }
    
    session->is_upload = true;
    session->verify_crc = (open_payload->flags & FTS_OPEN_FLAG_VERIFY_CRC) != 0;
    session->file_offset = open_payload->resume_offset;
    session->chunk_size = open_payload->chunk_size;
    strncpy(session->path, open_payload->path, sizeof(session->path) - 1);
    
    uint8_t mode = FM_O_WRITE | FM_O_CREATE;
    bool is_resume = (open_payload->flags & FTS_OPEN_FLAG_RESUME) && (open_payload->resume_offset > 0);
    
    if (is_resume) {
        /* Resume mode: open existing file and seek to offset */
        mode = FM_O_WRITE;  /* Don't truncate existing file */
        tsc_kprintf("[FTS_SERVER] Resume upload from offset %lu\n", (unsigned long)open_payload->resume_offset);
    }
    
    fm_err_t err = fm_open(session->path, mode, &session->fp);
    if (err != FM_OK) {
        fts_server_send_error(conn, header->session_id, err, "Failed to open file");
        fts_server_free_session(session);
        return;
    }
    
    if (is_resume) {
        err = fm_seek(session->fp, open_payload->resume_offset);
        if (err != FM_OK) {
            tsc_kprintf("[FTS_SERVER] Failed to seek to offset %lu, starting from beginning\n",
                       (unsigned long)open_payload->resume_offset);
            fts_server_send_error(conn, header->session_id, err, "Failed to seek");
            fts_server_free_session(session);
            return;
        }
    }
    
    fts_server_send_ack(session, header->sequence, 0);
    tsc_kprintf("[FTS_SERVER] Upload session ready\n");
}

/**
 * @brief Handle OPEN_DOWNLOAD command
 */
static void fts_server_handle_open_download(
    csp_conn_t *conn, const fts_packet_header_t *header,
    const fts_open_payload_t *open_payload)
{
    tsc_kprintf("[FTS_SERVER] OPEN_DOWNLOAD: session=%lu, path=%s\n",
               (unsigned long)header->session_id, open_payload->path);
    
    /* Security: Check connection security (HMAC/XTEA) */
    if (!fts_is_connection_secure(conn)) {
        tsc_kprintf("[FTS_SERVER] Connection security check failed\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Insufficient security");
        return;
    }
    
    /* Security: Validate path */
    if (!fts_validate_path(open_payload->path)) {
        tsc_kprintf("[FTS_SERVER] Path validation failed\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Invalid path");
        return;
    }
    
    /* Security: Check path whitelist */
    if (!fts_is_path_allowed(open_payload->path)) {
        tsc_kprintf("[FTS_SERVER] Path not in whitelist\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_PERM, "Path not allowed");
        return;
    }
    
    /* Security: Check rate limit */
    if (!fts_check_rate_limit(csp_conn_src(conn))) {
        tsc_kprintf("[FTS_SERVER] Rate limit exceeded\n");
        fts_server_send_error(conn, header->session_id, FM_ERR_BUSY, "Rate limit exceeded");
        return;
    }
    
    fts_server_session_t *session = fts_server_get_session(conn, header->session_id, true);
    
    if (!session) {
        fts_server_send_error(conn, header->session_id, FM_ERR_TOO_MANY_OPEN,
                            "Too many sessions");
        return;
    }
    
    session->is_upload = false;
    session->verify_crc = (open_payload->flags & FTS_OPEN_FLAG_VERIFY_CRC) != 0;
    session->file_offset = open_payload->resume_offset;
    session->chunk_size = (open_payload->chunk_size > 0 && open_payload->chunk_size <= FTS_MAX_CHUNK_SIZE) 
                         ? open_payload->chunk_size : FTS_MAX_CHUNK_SIZE;
    strncpy(session->path, open_payload->path, sizeof(session->path) - 1);
    
    fm_err_t err = fm_open(session->path, FM_O_READ, &session->fp);
    if (err != FM_OK) {
        fts_server_send_error(conn, header->session_id, err, "Failed to open file");
        fts_server_free_session(session);
        return;
    }
    
    fm_file_info_t info;
    if (fm_stat(session->path, &info) == FM_OK) {
        session->file_size = info.size;
    }
    
    if (open_payload->resume_offset > 0) {
        err = fm_seek(session->fp, open_payload->resume_offset);
        if (err != FM_OK) {
            fts_server_send_error(conn, header->session_id, err, "Failed to seek");
            fts_server_free_session(session);
            return;
        }
    }
    
    fts_server_send_ack(session, header->sequence, 0);
    
    tsc_kprintf("[FTS_SERVER] Download session ready, starting transfer\n");
    
    err = fts_server_send_file_data(session);
    
    if (err != FM_OK) {
        tsc_kprintf("[FTS_SERVER] Download failed: %d\n", err);
    }
}

/**
 * @brief Handle DATA command (upload)
 */
static void fts_server_handle_data(
    csp_conn_t *conn, const fts_packet_header_t *header,
    const fts_data_payload_t *data_payload)
{
    fts_server_session_t *session = fts_server_get_session(conn, header->session_id, false);
    
    if (!session || !session->is_upload) {
        fts_server_send_error(conn, header->session_id, FM_ERR_INVAL, "Invalid session");
        return;
    }
    
    if (session->verify_crc) {
        uint16_t calc_crc = fts_crc16(data_payload->data, data_payload->data_len);
        if (calc_crc != data_payload->crc16) {
            tsc_kprintf("[FTS_SERVER] CRC mismatch!\n");
            fts_server_send_ack(session, header->sequence, FM_ERR_IO);
            return;
        }
    }
    
    uint32_t bytes_written = 0;
    fm_err_t err = fm_write(session->fp, data_payload->data,
                           data_payload->data_len, &bytes_written);
    
    if (err != FM_OK || bytes_written != data_payload->data_len) {
        tsc_kprintf("[FTS_SERVER] Write failed: %d\n", err);
        fts_server_send_ack(session, header->sequence, err);
        return;
    }
    
    session->file_offset += bytes_written;
    session->last_sequence = header->sequence;
    
    fts_server_send_ack(session, header->sequence, 0);
    
    if (header->flags & FTS_DATA_FLAG_LAST) {
        tsc_kprintf("[FTS_SERVER] Upload complete: %lu bytes\n",
                   (unsigned long)session->file_offset);
        fm_sync(session->fp);
    }
}

/**
 * @brief Handle CLOSE command
 */
static void fts_server_handle_close(
    csp_conn_t *conn, const fts_packet_header_t *header)
{
    tsc_kprintf("[FTS_SERVER] CLOSE: session=%lu\n", (unsigned long)header->session_id);
    csp_sleep_ms(100);
    
    fts_server_session_t *session = fts_server_get_session(conn, header->session_id, false);
    
    if (session) {
        fts_server_free_session(session);
    }
    
    fts_ack_payload_t ack = {0};
    fts_server_send_response(conn, FTS_CMD_ACK, header->session_id, 0, &ack, sizeof(ack));
}

/**
 * @brief Main streaming server handler
 */
void fts_server_handler(csp_conn_t *conn) {
    if (!conn) {
        return;
    }
    
    tsc_kprintf("[FTS_SERVER] New streaming connection\n");
    
    /* Cleanup stale sessions before starting */
    if (fts_should_cleanup()) {
        fts_cleanup_stale_sessions();
    }
    
    while (1) {
        csp_packet_t *packet = csp_read(conn, 1000);
        
        if (!packet) {
            /* Periodic cleanup check during idle */
            if (fts_should_cleanup()) {
                fts_cleanup_stale_sessions();
            }
            break;
        }
        
        if (packet->length < sizeof(fts_packet_header_t)) {
            csp_buffer_free(packet);
            continue;
        }
        
        fts_packet_header_t *header = (fts_packet_header_t*)packet->data;
        
        if (!fts_validate_header(header)) {
            csp_buffer_free(packet);
            continue;
        }
        
        void *payload = (packet->length > sizeof(fts_packet_header_t)) ?
                       (packet->data + sizeof(fts_packet_header_t)) : NULL;
        
        switch (header->cmd) {
            case FTS_CMD_OPEN_UPLOAD:
                fts_server_handle_open_upload(conn, header, (fts_open_payload_t*)payload);
                break;
                
            case FTS_CMD_OPEN_DOWNLOAD:
                fts_server_handle_open_download(conn, header, (fts_open_payload_t*)payload);
                break;
                
            case FTS_CMD_DATA:
                fts_server_handle_data(conn, header, (fts_data_payload_t*)payload);
                break;
                
            case FTS_CMD_CLOSE:
                fts_server_handle_close(conn, header);
                csp_buffer_free(packet);
                goto cleanup;
                
            default:
                fts_server_send_error(conn, header->session_id, FM_ERR_INVAL, "Unknown command");
                break;
        }
        
        csp_buffer_free(packet);
        
        /* Periodic cleanup check */
        if (fts_should_cleanup()) {
            fts_cleanup_stale_sessions();
        }
    }
    
cleanup:
    tsc_kprintf("[FTS_SERVER] Connection closed\n");
    
    for (int i = 0; i < FTS_SERVER_MAX_SESSIONS; i++) {
        if (g_server_sessions[i].in_use && g_server_sessions[i].conn == conn) {
            fts_server_free_session(&g_server_sessions[i]);
        }
    }
}

/**
 * @brief Initialize streaming server
 */
void fts_server_init(void) {
    memset(g_server_sessions, 0, sizeof(g_server_sessions));
    tsc_kprintf("[FTS_SERVER] Streaming server initialized\n");
}

/*===========================================================================
 * SERVER TASK SETUP
 *===========================================================================*/
#if CSP_ENABLE_FILE
typedef struct {
    uint8_t port;
    csp_thread_handle_t thread;
} csp_streaming_server_context_t;

static csp_streaming_server_context_t streaming_ctx = {
    .port = FT_STREAMING_PORT
};

CSP_DEFINE_TASK(csp_streaming_server) {
    csp_streaming_server_context_t *ctx = param;

    csp_log_info("Streaming Server task started on port %d", ctx->port);

    csp_socket_t *sock = csp_socket(CSP_SO_NONE);
    if (!sock) {
        csp_log_error("Failed to create streaming server socket");
        return CSP_TASK_RETURN;
    }

    if (csp_bind(sock, ctx->port) == CSP_ERR_USED) {
        csp_log_warn("Port %d already in use, reusing socket", ctx->port);
        csp_close(sock);
        sock = csp_port_get_socket(ctx->port);
    } else {
        csp_listen(sock, 4);
    }

    csp_log_info("Streaming Server listening on port %d", ctx->port);

    while (1) {
        thread_wd_kick_self();

        csp_conn_t *conn = csp_accept(sock, 1000);
        if (!conn) {
            continue;
        }

        csp_log_info("Streaming connection accepted from node %d", csp_conn_src(conn));

        fts_server_handler(conn);

        csp_log_info("Streaming connection closed");
        csp_close(conn);
    }

    return CSP_TASK_RETURN;
}

TSC_THREAD_REGISTER(
    "STREAM_SRV",
    csp_streaming_server,
    0,
    &streaming_ctx,
    2048,
    2,
    0, 0, 0, 1
);

#endif