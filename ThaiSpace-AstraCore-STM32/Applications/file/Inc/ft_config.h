/**
 * @file ft_config.h
 * @brief Centralized configuration for file transfer system
 * 
 * This header provides centralized configuration constants for the file transfer
 * system, including port numbers, protocol selection thresholds, and buffer sizes.
 */

#ifndef FT_CONFIG_H
#define FT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Port Configuration
 * ============================================================================ */

/**
 * @brief Port number for streaming file transfer protocol
 * 
 * The streaming protocol uses persistent connections for improved performance
 * on large file transfers. This protocol is automatically selected for files
 * above the streaming threshold.
 */
#define FT_STREAMING_PORT 12

/* ============================================================================
 * Protocol Selection Configuration
 * ============================================================================ */

/**
 * @brief File size threshold for protocol selection (in bytes)
 * 
 * Files with size >= this threshold will use the streaming protocol (Port 12).
 * Files with size < this threshold will use the standard protocol (Port 11).
 * 
 * Default: 0 bytes (always use streaming protocol)
 * 
 * Rationale:
 * - Streaming protocol uses chunked transfer (200 bytes per chunk)
 * - No need to load entire file into RAM
 * - Supports resume capability
 * - More memory efficient for embedded systems
 * 
 * Performance note: Streaming protocol is ~6.9x faster for large files due to
 * persistent connections eliminating per-chunk connection overhead.
 */
#define FT_STREAMING_THRESHOLD_BYTES 0

/* ============================================================================
 * Buffer and Packet Size Configuration
 * ============================================================================ */

/**
 * @brief CSP buffer size limit
 * 
 * Maximum size of CSP buffer. All packets must fit within this limit.
 */
#define FT_CSP_BUFFER_SIZE 256

/**
 * @brief Maximum path length for streaming protocol
 * 
 * Maximum length of file path in streaming protocol packets.
 * Calculated to fit within CSP buffer:
 * 256 (buffer) - 16 (header) - 12 (open_payload) - 8 (safety) = 220 bytes
 */
#define FTS_MAX_PATH_LEN 220

/**
 * @brief Maximum CSP packet size (MTU) for standard protocol
 * 
 * This defines the maximum size of a single CSP packet including headers.
 * Affects chunk size calculations for standard protocol transfers.
 */
#define FT_CSP_MTU 196U

/**
 * @brief Maximum chunk size for streaming protocol
 * 
 * This defines the maximum data payload size per chunk in streaming transfers.
 * Must be smaller than CSP buffer size (256 bytes) minus protocol overhead.
 * 
 * Calculation: 256 (CSP buffer) - 16 (fts_packet_header_t) - 10 (fts_data_payload_t) = 230 bytes
 * Using 200 bytes for safety margin.
 */
#define FTS_MAX_CHUNK_SIZE 200

/**
 * @brief Flow control window size for streaming protocol
 * 
 * Number of unacknowledged chunks allowed in flight. Higher values improve
 * throughput but require more memory for buffering.
 */
#define FTS_WINDOW_SIZE 4

/* ============================================================================
 * Timeout Configuration
 * ============================================================================ */

/**
 * @brief CSP timeout for standard protocol operations (milliseconds)
 * 
 * Timeout for CSP send/receive operations in standard protocol.
 */
#define FT_CSP_TIMEOUT_MS 1000U

/**
 * @brief Default timeout for streaming protocol operations (milliseconds)
 * 
 * Default timeout for streaming protocol operations. Can be overridden
 * per-session via fts_config_t.
 */
#define FTS_DEFAULT_TIMEOUT_MS 5000

/* ============================================================================
 * Connection Configuration
 * ============================================================================ */

/**
 * @brief Maximum concurrent connections for standard protocol server
 * 
 * Maximum number of simultaneous client connections the standard protocol
 * server can handle.
 */
#define CSP_FT_SERVER_CON_MAX 8

/**
 * @brief Maximum concurrent sessions for streaming protocol
 * 
 * Maximum number of simultaneous streaming sessions (both client and server).
 */
#define FTS_MAX_SESSIONS 4

#ifdef __cplusplus
}
#endif

#endif /* FT_CONFIG_H */
