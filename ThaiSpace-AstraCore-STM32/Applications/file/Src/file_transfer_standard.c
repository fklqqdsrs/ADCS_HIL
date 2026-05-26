/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Standard file transfer protocol (Client + Server)
 * 
 * This file combines both client and server standard protocol implementations
 * for the simple request-response file transfer on port 11.
 */

#include "file_transfer_standard.h"
#include "file_core.h"
#include <string.h>
#include <kservice.h>

#ifdef FT_DEBUG_ENABLED
#define FT_DEBUG(fmt, ...) tsc_kprintf("[FT] " fmt "\n", ##__VA_ARGS__)
#else
#define FT_DEBUG(fmt, ...) ((void)0)
#endif

/*===========================================================================
 * SHARED HELPERS
 *===========================================================================*/

static csp_packet_t* ft_create_path_request(uint8_t cmd, const char *path) {
	size_t path_len = strlen(path) + 1;
	size_t packet_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;

	if (packet_size > FT_CSP_MTU) {
		return NULL;
	}

	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (!packet) {
		return NULL;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
	header->cmd = cmd;
	header->flags = FT_FLAG_NONE;
	header->status = 0;
	header->seq = 0;
	header->payload_len = 0;

	uint16_t *path_len_ptr = (uint16_t*) (packet->data + sizeof(ft_packet_header_t));
	*path_len_ptr = (uint16_t) path_len;

	memcpy(packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t), path, path_len);
	packet->length = packet_size;

	return packet;
}

static csp_packet_t* ft_send_request(uint8_t node, csp_packet_t *req_packet) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, FT_CSP_PORT, FT_CSP_TIMEOUT_MS, CSP_O_NONE);
	if (!conn) {
		if (req_packet) csp_buffer_free(req_packet);
		return NULL;
	}

	if (!csp_send(conn, req_packet, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(req_packet);
		csp_close(conn);
		return NULL;
	}

	csp_packet_t *response = csp_read(conn, FT_CSP_TIMEOUT_MS);
	csp_close(conn);
	return response;
}

static const char* extract_path(csp_packet_t *packet, uint16_t *out_path_len) {
	size_t min_size = sizeof(ft_packet_header_t) + sizeof(uint16_t);

	if (packet->length < min_size) {
		return NULL;
	}

	uint16_t path_len = *(uint16_t*) (packet->data + sizeof(ft_packet_header_t));

	if (out_path_len) {
		*out_path_len = path_len;
	}

	if (min_size + path_len > packet->length) {
		return NULL;
	}

	const char *path = (const char*) (packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
	
	// Verify path is null-terminated
	if (path_len > 0 && path[path_len - 1] != '\0') {
		return NULL;
	}

	return path;
}

/*===========================================================================
 * CLIENT IMPLEMENTATION
 *===========================================================================*/

#define FT_MAX_HANDLES 8

typedef struct {
	uint8_t in_use;
	uint8_t node;
	char path[FM_MAX_PATH_LEN];
	uint8_t mode;
	uint32_t read_offset;
} ft_handle_t;

static ft_handle_t g_handles[FT_MAX_HANDLES] = { 0 };

fm_err_t ft_stat(uint8_t node, const char *path, fm_file_info_t *info) {
	csp_packet_t *packet = ft_create_path_request(FT_CMD_STAT, path);
	if (!packet) return FM_ERR_IO;

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) return FM_ERR_IO;

	ft_packet_header_t *header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(header->status);

	if (res == FM_OK && info && (response->length > sizeof(ft_packet_header_t) + sizeof(uint16_t))) {
		uint16_t path_len;
		memcpy(&path_len, response->data + sizeof(ft_packet_header_t), sizeof(uint16_t));

		size_t payload_offset = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;

		// Server sends compact format: size(4) + attributes(2) + date(2) + time(2) + is_dir(1) = 11 bytes
		if (response->length >= payload_offset + 11) {
			const uint8_t *compact_info = response->data + payload_offset;
			
			// Clear info struct
			memset(info, 0, sizeof(fm_file_info_t));
			
			// Extract filename from path
			const char *filename = strrchr(path, '/');
			if (filename) {
				filename++; // Skip the '/'
			} else {
				filename = path;
			}
			strncpy(info->name, filename, FM_MAX_NAME_LEN - 1);
			info->name[FM_MAX_NAME_LEN - 1] = '\0';
			
			// Unpack compact data
			memcpy(&info->size, compact_info, 4);
			memcpy(&info->attributes, compact_info + 4, 2);
			memcpy(&info->date, compact_info + 6, 2);
			memcpy(&info->time, compact_info + 8, 2);
			memcpy(&info->is_dir, compact_info + 10, 1);
		}
	}

	csp_buffer_free(response);
	return res;
}

fm_err_t ft_mkdir(uint8_t node, const char *path) {
	csp_packet_t *packet = ft_create_path_request(FT_CMD_MKDIR, path);
	if (!packet) return FM_ERR_IO;

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) return FM_ERR_IO;

	ft_packet_header_t *header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(header->status);

	csp_buffer_free(response);
	return res;
}

fm_err_t ft_rm(uint8_t node, const char *path) {
	csp_packet_t *packet = ft_create_path_request(FT_CMD_DELETE, path);
	if (!packet) return FM_ERR_IO;

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) return FM_ERR_IO;

	ft_packet_header_t *header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(header->status);

	csp_buffer_free(response);
	return res;
}

fm_err_t ft_mv(uint8_t node, const char *src, const char *dst) {
	size_t src_len = strlen(src) + 1;
	size_t dst_len = strlen(dst) + 1;
	size_t packet_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + src_len + dst_len;

	if (packet_size > FT_CSP_MTU) return FM_ERR_INVAL;

	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (!packet) return FM_ERR_IO;

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
	header->cmd = FT_CMD_RENAME;
	header->flags = FT_FLAG_NONE;
	header->status = 0;
	header->seq = 0;

	uint16_t *path_len_ptr = (uint16_t*) (packet->data + sizeof(ft_packet_header_t));
	*path_len_ptr = (uint16_t) (src_len + dst_len);
	header->payload_len = src_len + dst_len;

	char *p = (char*) (packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
	memcpy(p, src, src_len);
	memcpy(p + src_len, dst, dst_len);

	packet->length = packet_size;

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) return FM_ERR_IO;

	header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(header->status);

	csp_buffer_free(response);
	return res;
}

fm_err_t ft_cp(uint8_t node, const char *src, const char *dst) {
	size_t src_len = strlen(src) + 1;
	size_t dst_len = strlen(dst) + 1;
	size_t packet_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + src_len + dst_len;

	if (packet_size > FT_CSP_MTU) {
		return FM_ERR_INVAL;
	}

	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (!packet) {
		return FM_ERR_IO;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
	header->cmd = FT_CMD_COPY;
	header->flags = FT_FLAG_NONE;
	header->status = 0;
	header->seq = 0;

	uint16_t *path_len_ptr = (uint16_t*) (packet->data + sizeof(ft_packet_header_t));
	*path_len_ptr = (uint16_t) (src_len + dst_len);
	header->payload_len = src_len + dst_len;

	char *p = (char*) (packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
	memcpy(p, src, src_len);
	memcpy(p + src_len, dst, dst_len);

	packet->length = packet_size;

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) {
		return FM_ERR_IO;
	}

	header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(header->status);

	csp_buffer_free(response);
	return res;
}

fm_err_t ft_get_space(uint8_t node, uint32_t *total_kb, uint32_t *free_kb) {
	csp_packet_t *packet = csp_buffer_get(sizeof(ft_packet_header_t));
	if (!packet) return FM_ERR_IO;

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
	header->cmd = FT_CMD_GET_SPACE;
	header->flags = FT_FLAG_NONE;
	header->status = 0;
	header->seq = 0;
	header->payload_len = 0;
	packet->length = sizeof(ft_packet_header_t);

	csp_packet_t *response = ft_send_request(node, packet);
	if (!response) return FM_ERR_IO;

	ft_packet_header_t *resp_header = (ft_packet_header_t*) response->data;
	fm_err_t res = ft_int16_to_fm_err(resp_header->status);

	if (res == FM_OK && response->length >= sizeof(ft_packet_header_t) + 8) {
		uint32_t *data = (uint32_t*)(response->data + sizeof(ft_packet_header_t));
		if (total_kb) *total_kb = data[0];
		if (free_kb) *free_kb = data[1];
	}

	csp_buffer_free(response);
	return res;
}

int32_t ft_read_file(uint8_t node, const char *path, void *buf, uint32_t len) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, FT_CSP_PORT, FT_CSP_TIMEOUT_MS, CSP_O_NONE);
	if (!conn) return -FM_ERR_IO;

	csp_packet_t *packet = ft_create_path_request(FT_CMD_READ, path);
	if (!packet) {
		csp_close(conn);
		return -FM_ERR_IO;
	}

	if (!csp_send(conn, packet, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(packet);
		csp_close(conn);
		return -FM_ERR_IO;
	}

	uint32_t total_read = 0;
	uint16_t expected_seq = 0;
	int32_t result = 0;

	while (total_read < len) {
		csp_packet_t *response = csp_read(conn, FT_CSP_TIMEOUT_MS);
		if (!response) {
			result = -FM_ERR_IO;
			break;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) response->data;

		if (header->flags & FT_FLAG_ERROR) {
			result = -ft_fm_err_to_int32(ft_int16_to_fm_err(header->status));
			csp_buffer_free(response);
			break;
		}

		if (header->seq != expected_seq) {
			result = -FM_ERR_IO;
			csp_buffer_free(response);
			break;
		}
		expected_seq++;

		uint16_t path_len;
		memcpy(&path_len, response->data + sizeof(ft_packet_header_t), sizeof(uint16_t));

		size_t payload_offset = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;

		if (response->length > payload_offset) {
			size_t payload_len = response->length - payload_offset;

			if (payload_len > 0) {
				uint32_t copy_len = (total_read + payload_len > len) ? (len - total_read) : payload_len;
				memcpy((uint8_t*) buf + total_read, response->data + payload_offset, copy_len);
				total_read += copy_len;
			}
		}

		if (header->flags & FT_FLAG_EOF) {
			result = total_read;
			csp_buffer_free(response);
			break;
		}

		csp_buffer_free(response);
	}

	csp_close(conn);
	return result;
}

int32_t ft_read_chunk(uint8_t node, const char *path, uint32_t offset, uint32_t length, void *buf) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, FT_CSP_PORT, FT_CSP_TIMEOUT_MS, CSP_O_NONE);
	if (!conn) return -FM_ERR_IO;

	size_t path_len = strlen(path) + 1;
	size_t packet_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len + sizeof(uint32_t) * 2;

	if (packet_size > FT_CSP_MTU) {
		csp_close(conn);
		return -FM_ERR_INVAL;
	}

	csp_packet_t *packet = csp_buffer_get(packet_size);
	if (!packet) {
		csp_close(conn);
		return -FM_ERR_IO;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
	header->cmd = FT_CMD_READ_CHUNK;
	header->flags = FT_FLAG_EOF;
	header->status = 0;
	header->seq = 0;

	uint16_t *path_len_ptr = (uint16_t*) (packet->data + sizeof(ft_packet_header_t));
	*path_len_ptr = (uint16_t) path_len;
	header->payload_len = sizeof(uint32_t) * 2;

	char *p = (char*) (packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
	memcpy(p, path, path_len);
	memcpy(p + path_len, &offset, sizeof(uint32_t));
	memcpy(p + path_len + sizeof(uint32_t), &length, sizeof(uint32_t));

	packet->length = packet_size;

	if (!csp_send(conn, packet, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(packet);
		csp_close(conn);
		return -FM_ERR_IO;
	}

	csp_packet_t *response = csp_read(conn, FT_CSP_TIMEOUT_MS);
	csp_close(conn);

	if (!response) {
		return -FM_ERR_IO;
	}

	header = (ft_packet_header_t*) response->data;

	if (header->flags & FT_FLAG_ERROR) {
		csp_buffer_free(response);
		return -ft_fm_err_to_int32(ft_int16_to_fm_err(header->status));
	}

	uint16_t resp_path_len;
	memcpy(&resp_path_len, response->data + sizeof(ft_packet_header_t), sizeof(uint16_t));

	size_t payload_offset = sizeof(ft_packet_header_t) + sizeof(uint16_t) + resp_path_len;

	uint32_t bytes_read = header->payload_len;
	if (bytes_read > 0 && response->length > payload_offset) {
		uint32_t available = response->length - payload_offset;
		uint32_t copy_len = (bytes_read > length) ? length : bytes_read;
		if (copy_len > available) copy_len = available;
		memcpy(buf, response->data + payload_offset, copy_len);
	}

	csp_buffer_free(response);
	return bytes_read;
}

fm_err_t ft_write_file(uint8_t node, const char *path, const void *buf, uint32_t len) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, FT_CSP_PORT, FT_CSP_TIMEOUT_MS, CSP_O_NONE);
	if (!conn) return FM_ERR_IO;

	size_t path_len = strlen(path) + 1;
	size_t header_path_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;
	size_t max_payload = FT_CSP_MTU - header_path_size;

	uint32_t offset = 0;
	uint16_t seq = 0;
	fm_err_t result = FM_OK;

	while (offset < len) {
		size_t chunk_size = (len - offset > max_payload) ? max_payload : (len - offset);
		uint8_t is_eof = (offset + chunk_size >= len) ? FT_FLAG_EOF : FT_FLAG_MORE;

		csp_packet_t *packet = csp_buffer_get(header_path_size + chunk_size);
		if (!packet) {
			result = FM_ERR_IO;
			break;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) packet->data;
		header->cmd = FT_CMD_WRITE;
		header->flags = is_eof;
		header->status = 0;
		header->seq = seq;

		uint16_t *path_len_ptr = (uint16_t*) (packet->data + sizeof(ft_packet_header_t));
		*path_len_ptr = (uint16_t) path_len;
		header->payload_len = chunk_size;

		memcpy(packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t), path, path_len);
		memcpy(packet->data + header_path_size, (uint8_t*) buf + offset, chunk_size);

		packet->length = header_path_size + chunk_size;

		if (!csp_send(conn, packet, FT_CSP_TIMEOUT_MS)) {
			csp_buffer_free(packet);
			result = FM_ERR_IO;
			break;
		}

		if (is_eof & FT_FLAG_EOF) {
			csp_packet_t *response = csp_read(conn, FT_CSP_TIMEOUT_MS);
			if (response) {
				header = (ft_packet_header_t*) response->data;
				result = ft_int16_to_fm_err(header->status);
				csp_buffer_free(response);
			} else {
				result = FM_ERR_IO;
			}
		}

		offset += chunk_size;
		seq++;
	}

	csp_close(conn);
	return result;
}

void ft_list(uint8_t node, const char *path) {
	csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, FT_CSP_PORT, FT_CSP_TIMEOUT_MS, CSP_O_NONE);
	if (!conn) {
		tsc_kprintf("Error: Cannot connect to node %d\n", node);
		return;
	}

	csp_packet_t *packet = ft_create_path_request(FT_CMD_LIST, path);
	if (!packet) {
		csp_close(conn);
		tsc_kprintf("Error: Cannot create packet\n");
		return;
	}

	if (!csp_send(conn, packet, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(packet);
		csp_close(conn);
		tsc_kprintf("Error: Cannot send request\n");
		return;
	}

	tsc_kprintf("Listing %s:\n", path);

	uint16_t expected_seq = 0;
	int count = 0;

	while (1) {
		csp_packet_t *response = csp_read(conn, FT_CSP_TIMEOUT_MS);
		if (!response) {
			tsc_kprintf("Error: Timeout reading response\n");
			break;
		}

		if (response->length < sizeof(ft_packet_header_t)) {
			csp_buffer_free(response);
			continue;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) response->data;

		if (header->cmd != FT_CMD_LIST) {
			csp_buffer_free(response);
			continue;
		}

		if (header->flags & FT_FLAG_ERROR) {
			tsc_kprintf("Error: %d\n", header->status);
			csp_buffer_free(response);
			break;
		}

		if (header->seq != expected_seq) {
			tsc_kprintf("Error: Sequence mismatch (expected %u got %u)\n", expected_seq, header->seq);
			csp_buffer_free(response);
			break;
		}

		expected_seq++;

		if (response->length < sizeof(ft_packet_header_t) + sizeof(uint16_t)) {
			csp_buffer_free(response);
			break;
		}

		uint16_t path_len;
		memcpy(&path_len, response->data + sizeof(ft_packet_header_t), sizeof(uint16_t));

		size_t payload_offset = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;

		if (response->length < payload_offset + sizeof(uint16_t)) {
			csp_buffer_free(response);
			break;
		}

		uint16_t name_len;
		memcpy(&name_len, response->data + payload_offset, sizeof(uint16_t));

		size_t total_payload_size = sizeof(uint16_t) + name_len + sizeof(uint32_t) + sizeof(uint8_t);
		if (response->length < payload_offset + total_payload_size) {
			csp_buffer_free(response);
			break;
		}

		char *p = (char*) (response->data + payload_offset + sizeof(uint16_t));

		char name[FM_MAX_NAME_LEN];
		uint16_t copy_len = (name_len < FM_MAX_NAME_LEN - 1) ? name_len : FM_MAX_NAME_LEN - 1;
		memcpy(name, p, copy_len);
		name[copy_len] = '\0';
		p += name_len;

		uint32_t size;
		memcpy(&size, p, sizeof(uint32_t));
		p += sizeof(uint32_t);

		uint8_t is_dir;
		memcpy(&is_dir, p, sizeof(uint8_t));

		char type = is_dir ? 'd' : '-';

		tsc_kprintf("%-30s %-10c %-10lu\n", name, type, (unsigned long) size);

		count++;

		if (header->flags & FT_FLAG_EOF) {
			csp_buffer_free(response);
			break;
		}

		csp_buffer_free(response);
	}

	tsc_kprintf("Total entries: %d\n", count);
	csp_close(conn);
}

int32_t ft_open(uint8_t node, const char *path, uint8_t mode) {
	int32_t handle = -1;
	for (int i = 0; i < FT_MAX_HANDLES; i++) {
		if (!g_handles[i].in_use) {
			handle = i;
			break;
		}
	}

	if (handle < 0) {
		return -FM_ERR_IO;
	}

	if ((mode & FA_READ) && !(mode & FA_WRITE)) {
		fm_file_info_t info;
		fm_err_t err = ft_stat(node, path, &info);
		if (err != FM_OK) {
			return -err;
		}
		if (info.is_dir) {
			return -FM_ERR_INVAL;
		}
	}

	g_handles[handle].in_use = 1;
	g_handles[handle].node = node;
	strncpy(g_handles[handle].path, path, FM_MAX_PATH_LEN - 1);
	g_handles[handle].path[FM_MAX_PATH_LEN - 1] = '\0';
	g_handles[handle].mode = mode;
	g_handles[handle].read_offset = 0;

	return handle + 1;
}

int32_t ft_read(uint8_t node, int32_t handle, void *buf, uint32_t len) {
	(void) node;

	int32_t idx = handle - 1;
	if (idx < 0 || idx >= FT_MAX_HANDLES || !g_handles[idx].in_use) {
		return -FM_ERR_INVAL;
	}

	ft_handle_t *h = &g_handles[idx];

	int32_t result = ft_read_chunk(h->node, h->path, h->read_offset, len, buf);
	if (result > 0) {
		h->read_offset += result;
	}

	return result;
}

int32_t ft_write(uint8_t node, int32_t handle, const void *buf, uint32_t len) {
	(void) node;

	int32_t idx = handle - 1;
	if (idx < 0 || idx >= FT_MAX_HANDLES || !g_handles[idx].in_use) {
		return -FM_ERR_INVAL;
	}

	ft_handle_t *h = &g_handles[idx];

	fm_err_t err = ft_write_file(h->node, h->path, buf, len);

	return (err == FM_OK) ? (int32_t) len : -FM_ERR_IO;
}

fm_err_t ft_close(uint8_t node, int32_t handle) {
	(void) node;

	int32_t idx = handle - 1;
	if (idx < 0 || idx >= FT_MAX_HANDLES || !g_handles[idx].in_use) {
		return FM_ERR_INVAL;
	}

	memset(&g_handles[idx], 0, sizeof(ft_handle_t));

	return FM_OK;
}

int32_t ft_opendir(uint8_t node, const char *path) {
	(void) path;
	fm_file_info_t info;
	fm_err_t err = ft_stat(node, path, &info);
	if (err == FM_OK && info.is_dir) {
		return 1;
	}
	return -err;
}

fm_err_t ft_readdir(uint8_t node, int32_t handle, fm_file_info_t *info) {
	(void) node;
	(void) handle;
	(void) info;
	return FM_ERR_NOT_SUPPORTED;
}

fm_err_t ft_closedir(uint8_t node, int32_t handle) {
	(void) node;
	(void) handle;
	return FM_OK;
}


/*===========================================================================
 * SERVER IMPLEMENTATION
 *===========================================================================*/

typedef struct {
	uint8_t port;
	csp_thread_handle_t thread;
} csp_file_server_context_t;

typedef struct {
	fm_file_handle_t *fp;
	char path[FM_MAX_PATH_LEN];
} ft_write_ctx_t;

#define FT_MAX_CONNS 8
typedef struct {
	csp_conn_t *conn;
	ft_write_ctx_t ctx;
	uint8_t in_use;
} ft_conn_state_t;

static ft_conn_state_t g_conn_states[FT_MAX_CONNS] = { 0 };
static csp_file_server_context_t def_file_ctx = { .port = FT_CSP_PORT };

#if CSP_ENABLE_FILE
CSP_DEFINE_TASK(csp_ft_server);

TSC_THREAD_REGISTER(FT_SERVER_THREAD_NAME, csp_ft_server, 0, &def_file_ctx, 
                   FT_SERVER_STACK_SIZE, FT_SERVER_PRIORITY, 0, 0, 0, 1);
#endif

static void send_simple_response(csp_conn_t *conn, uint8_t cmd, uint16_t seq, fm_err_t status) {
	csp_packet_t *response = csp_buffer_get(sizeof(ft_packet_header_t));
	if (!response) {
		return;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) response->data;
	header->cmd = cmd;
	header->flags = FT_FLAG_EOF;
	header->status = ft_fm_err_to_int16(status);
	header->seq = seq;
	header->payload_len = 0;
	response->length = sizeof(ft_packet_header_t);

	if (!csp_send(conn, response, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(response);
	}
}

static void send_path_response(csp_conn_t *conn, uint8_t cmd, uint16_t seq, 
                               fm_err_t status, const char *path, 
                               uint16_t payload_len, const void *payload) {
	size_t path_len = strlen(path) + 1;
	size_t packet_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len + payload_len;

	if (packet_size > FT_CSP_MTU) {
		send_simple_response(conn, cmd, seq, FM_ERR_INVAL);
		return;
	}

	csp_packet_t *response = csp_buffer_get(packet_size);
	if (!response) {
		return;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) response->data;
	header->cmd = cmd;
	header->flags = FT_FLAG_EOF;
	header->status = ft_fm_err_to_int16(status);
	header->seq = seq;
	header->payload_len = payload_len;

	uint16_t *path_len_ptr = (uint16_t*) (response->data + sizeof(ft_packet_header_t));
	*path_len_ptr = (uint16_t) path_len;

	char *p = (char*) (response->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
	memcpy(p, path, path_len);

	if (payload && payload_len > 0) {
		memcpy(p + path_len, payload, payload_len);
	}

	response->length = packet_size;

	if (!csp_send(conn, response, FT_CSP_TIMEOUT_MS)) {
		csp_buffer_free(response);
	}
}

static ft_write_ctx_t* get_write_ctx(csp_conn_t *conn) {
	for (int i = 0; i < FT_MAX_CONNS; i++) {
		if (g_conn_states[i].in_use && g_conn_states[i].conn == conn) {
			return &g_conn_states[i].ctx;
		}
	}

	for (int i = 0; i < FT_MAX_CONNS; i++) {
		if (!g_conn_states[i].in_use) {
			g_conn_states[i].conn = conn;
			g_conn_states[i].in_use = 1;
			memset(&g_conn_states[i].ctx, 0, sizeof(ft_write_ctx_t));
			return &g_conn_states[i].ctx;
		}
	}

	return NULL;
}

static void clear_write_ctx(csp_conn_t *conn) {
	for (int i = 0; i < FT_MAX_CONNS; i++) {
		if (g_conn_states[i].in_use && g_conn_states[i].conn == conn) {
			if (g_conn_states[i].ctx.fp) {
				const fm_vfs_t *vfs = fm_get_vfs(g_conn_states[i].ctx.path);
				if (vfs) {
					vfs->close(&g_conn_states[i].ctx.fp);
				}
				g_conn_states[i].ctx.fp = NULL;
			}

			memset(&g_conn_states[i], 0, sizeof(ft_conn_state_t));
			break;
		}
	}
}

static void handle_get_space(csp_conn_t *conn, csp_packet_t *packet) {
	ft_packet_header_t *req_header = (ft_packet_header_t*) packet->data;
	
	// Get VFS for SD card using standard prefix
	const fm_vfs_t *vfs = fm_get_vfs("/D/");
	if (!vfs || !vfs->statfs) {
		send_simple_response(conn, FT_CMD_GET_SPACE, req_header->seq, FM_ERR_NOT_SUPPORTED);
		return;
	}

	fm_fs_stats_t stats;
	fm_err_t res = vfs->statfs("/D/", &stats);

	if (res == FM_OK) {
		csp_packet_t *response = csp_buffer_get(sizeof(ft_packet_header_t) + 8);
		if (!response) {
			send_simple_response(conn, FT_CMD_GET_SPACE, req_header->seq, FM_ERR_IO);
			return;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) response->data;
		header->cmd = FT_CMD_GET_SPACE;
		header->flags = FT_FLAG_NONE;
		header->status = ft_fm_err_to_int16(FM_OK);
		header->seq = req_header->seq;
		header->payload_len = 8;

		uint32_t *data = (uint32_t*)(response->data + sizeof(ft_packet_header_t));
		data[0] = (uint32_t)(stats.total_bytes / 1024);  // Convert to KB
		data[1] = (uint32_t)(stats.free_bytes / 1024);   // Convert to KB

		response->length = sizeof(ft_packet_header_t) + 8;

		if (!csp_send(conn, response, FT_CSP_TIMEOUT_MS)) {
			csp_buffer_free(response);
		}
	} else {
		send_simple_response(conn, FT_CMD_GET_SPACE, req_header->seq, res);
	}
}

static void handle_stat(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	
	if (!path) {
		send_simple_response(conn, FT_CMD_STAT, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_STAT, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer to avoid static buffer issues
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';

	fm_file_info_t info;
	fm_err_t res = vfs->stat(local_path, &info);

	if (res == FM_OK) {
		// Send only essential fields (not the full struct with 256-byte name)
		// Format: size(4) + attributes(2) + date(2) + time(2) + is_dir(1) = 11 bytes
		uint8_t compact_info[11];
		memcpy(compact_info, &info.size, 4);
		memcpy(compact_info + 4, &info.attributes, 2);
		memcpy(compact_info + 6, &info.date, 2);
		memcpy(compact_info + 8, &info.time, 2);
		memcpy(compact_info + 10, &info.is_dir, 1);
		
		send_path_response(conn, FT_CMD_STAT, 0, FM_OK, path, 11, compact_info);
	} else {
		send_simple_response(conn, FT_CMD_STAT, 0, res);
	}
}

static void handle_mkdir(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_MKDIR, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_MKDIR, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';
	
	fm_err_t res = vfs->mkdir(local_path);

	send_simple_response(conn, FT_CMD_MKDIR, 0, res);
}

static void handle_delete(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_DELETE, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_DELETE, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';
	
	fm_err_t res = vfs->remove(local_path);

	send_simple_response(conn, FT_CMD_DELETE, 0, res);
}

static void handle_rename(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *paths = extract_path(packet, &path_len);
	if (!paths || path_len < 2) {
		send_simple_response(conn, FT_CMD_RENAME, 0, FM_ERR_INVAL);
		return;
	}

	const char *src = paths;
	size_t src_len = strlen(src) + 1;
	const char *dst = paths + src_len;

	if (src_len >= path_len) {
		send_simple_response(conn, FT_CMD_RENAME, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(src);
	if (!vfs) {
		tsc_kprintf("[FT] handle_rename: VFS not found for path: %s\n", src);
		send_simple_response(conn, FT_CMD_RENAME, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to separate local buffers to avoid static buffer collision
	char local_src[FM_MAX_PATH_LEN];
	char local_dst[FM_MAX_PATH_LEN];
	
	const char *stripped_src = fm_strip_prefix(src);
	strncpy(local_src, stripped_src, sizeof(local_src) - 1);
	local_src[sizeof(local_src) - 1] = '\0';
	
	const char *stripped_dst = fm_strip_prefix(dst);
	strncpy(local_dst, stripped_dst, sizeof(local_dst) - 1);
	local_dst[sizeof(local_dst) - 1] = '\0';
	
	tsc_kprintf("[FT] handle_rename: %s -> %s (local: %s -> %s)\n", src, dst, local_src, local_dst);
	
	fm_err_t res = vfs->rename(local_src, local_dst);
	
	if (res != FM_OK) {
		tsc_kprintf("[FT] handle_rename: rename failed (err=%d)\n", res);
	}

	send_simple_response(conn, FT_CMD_RENAME, 0, res);
}

static void handle_copy(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *paths = extract_path(packet, &path_len);
	if (!paths || path_len < 2) {
		send_simple_response(conn, FT_CMD_COPY, 0, FM_ERR_INVAL);
		return;
	}

	const char *src = paths;
	size_t src_len = strlen(src) + 1;
	const char *dst = paths + src_len;

	if (src_len >= path_len) {
		send_simple_response(conn, FT_CMD_COPY, 0, FM_ERR_INVAL);
		return;
	}

	fm_err_t res = fm_copy(src, dst);

	send_simple_response(conn, FT_CMD_COPY, 0, res);
}

static void handle_read(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_READ, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		tsc_kprintf("[FT] handle_read: VFS not found for path: %s\n", path);
		send_simple_response(conn, FT_CMD_READ, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';

	fm_file_handle_t *fp = NULL;
	fm_err_t res = vfs->open(&fp, local_path, FM_O_READ);
	if (res != FM_OK) {
		tsc_kprintf("[FT] handle_read: open failed for %s -> %s (err=%d)\n", path, local_path, res);
		send_simple_response(conn, FT_CMD_READ, 0, res);
		return;
	}

	uint16_t seq = 0;
	size_t header_path_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;
	size_t max_payload = FT_CSP_MTU - header_path_size;

	uint8_t *read_buf = malloc(max_payload);
	if (!read_buf) {
		vfs->close(&fp);
		send_simple_response(conn, FT_CMD_READ, 0, FM_ERR_IO);
		return;
	}

	uint32_t total_bytes_sent = 0;

	while (1) {
		uint32_t bytes_read = 0;
		res = vfs->read(fp, read_buf, max_payload, &bytes_read);

		if (res != FM_OK) {
			csp_packet_t *err_packet = csp_buffer_get(header_path_size);
			if (err_packet) {
				ft_packet_header_t *header = (ft_packet_header_t*) err_packet->data;
				header->cmd = FT_CMD_READ;
				header->flags = FT_FLAG_EOF | FT_FLAG_ERROR;
				header->status = ft_fm_err_to_int16(res);
				header->seq = seq;
				header->payload_len = 0;

				uint16_t *path_len_ptr = (uint16_t*) (err_packet->data + sizeof(ft_packet_header_t));
				*path_len_ptr = path_len;
				memcpy(err_packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t), path, path_len);
				err_packet->length = header_path_size;

				csp_send(conn, err_packet, FT_CSP_TIMEOUT_MS);
			}
			break;
		}

		uint8_t is_eof = (bytes_read < max_payload) ? FT_FLAG_EOF : FT_FLAG_MORE;
		size_t packet_size = header_path_size + bytes_read;

		csp_packet_t *response = csp_buffer_get(packet_size);
		if (!response) {
			break;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) response->data;
		header->cmd = FT_CMD_READ;
		header->flags = is_eof;
		header->status = 0;
		header->seq = seq;
		header->payload_len = bytes_read;

		uint16_t *path_len_ptr = (uint16_t*) (response->data + sizeof(ft_packet_header_t));
		*path_len_ptr = path_len;

		char *p = (char*) (response->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
		memcpy(p, path, path_len);
		memcpy(p + path_len, read_buf, bytes_read);

		response->length = packet_size;

		if (!csp_send(conn, response, FT_CSP_TIMEOUT_MS)) {
			csp_buffer_free(response);
			break;
		}

		total_bytes_sent += bytes_read;
		seq++;

		if (is_eof & FT_FLAG_EOF) {
			break;
		}
	}

	free(read_buf);
	vfs->close(&fp);
}

static void handle_write(csp_conn_t *conn, csp_packet_t *packet, ft_write_ctx_t *ctx) {
	ft_packet_header_t *req_header = (ft_packet_header_t*) packet->data;

	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_WRITE, req_header->seq, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_WRITE, req_header->seq, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';

	size_t header_path_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;
	void *data = packet->data + header_path_size;
	uint16_t data_len = req_header->payload_len;

	if (req_header->seq == 0) {
		if (ctx->fp) {
			vfs->close(&ctx->fp);
			ctx->fp = NULL;
		}

		fm_err_t res = vfs->open(&ctx->fp, local_path, FM_O_WRITE | FM_O_CREATE);
		if (res != FM_OK) {
			send_simple_response(conn, FT_CMD_WRITE, req_header->seq, res);
			ctx->fp = NULL;
			return;
		}

		strncpy(ctx->path, path, sizeof(ctx->path) - 1);
		ctx->path[sizeof(ctx->path) - 1] = '\0';
	}

	if (ctx->fp && strcmp(ctx->path, path) != 0) {
		vfs->close(&ctx->fp);
		ctx->fp = NULL;
		ctx->path[0] = '\0';
		send_simple_response(conn, FT_CMD_WRITE, req_header->seq, FM_ERR_INVAL);
		return;
	}

	fm_err_t res = FM_OK;
	if (data_len > 0 && ctx->fp) {
		uint32_t bytes_written = 0;
		res = vfs->write(ctx->fp, data, data_len, &bytes_written);
	}

	if (req_header->flags & FT_FLAG_EOF) {
		if (ctx->fp) {
			vfs->close(&ctx->fp);
			ctx->fp = NULL;
			ctx->path[0] = '\0';
		}
		send_simple_response(conn, FT_CMD_WRITE, req_header->seq, res);
	}
}

static void handle_list(csp_conn_t *conn, csp_packet_t *packet) {
	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_LIST, 0, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_LIST, 0, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';

	fm_dir_handle_t *dp = NULL;
	fm_err_t res = vfs->opendir(&dp, local_path);

	if (res != FM_OK) {
		send_simple_response(conn, FT_CMD_LIST, 0, res);
		return;
	}

	uint16_t seq = 0;
	fm_file_info_t info;

	while (1) {
		osDelay(5);

		res = vfs->readdir(dp, &info);

		if (res != FM_OK || info.name[0] == '\0') {
			size_t header_size = sizeof(ft_packet_header_t);
			size_t meta_size = sizeof(uint16_t) + path_len;
			size_t eof_packet_size = header_size + meta_size;

			csp_packet_t *eof_packet = csp_buffer_get(eof_packet_size);

			if (!eof_packet) {
				break;
			}

			ft_packet_header_t *header = (ft_packet_header_t*) eof_packet->data;

			header->cmd = FT_CMD_LIST;
			header->flags = FT_FLAG_EOF;
			header->status = 0;
			header->seq = seq;
			header->payload_len = 0;

			uint16_t *path_len_ptr = (uint16_t*) (eof_packet->data + sizeof(ft_packet_header_t));
			*path_len_ptr = path_len;

			memcpy(eof_packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t), path, path_len);

			eof_packet->length = eof_packet_size;

			if (!csp_send(conn, eof_packet, FT_CSP_TIMEOUT_MS)) {
				csp_buffer_free(eof_packet);
			}
			break;
		}

		size_t name_len = strlen(info.name);
		size_t payload_size = sizeof(uint16_t) + name_len + sizeof(uint32_t) + sizeof(uint8_t);
		size_t header_size = sizeof(ft_packet_header_t);
		size_t meta_size = sizeof(uint16_t) + path_len;
		size_t packet_size = header_size + meta_size + payload_size;

		size_t max_name_len = FT_CSP_MTU - 17 - path_len;

		if (name_len > max_name_len) {
			name_len = max_name_len;
			payload_size = sizeof(uint16_t) + name_len + sizeof(uint32_t) + sizeof(uint8_t);
			packet_size = header_size + meta_size + payload_size;
		}

		if (packet_size > FT_CSP_MTU) {
			continue;
		}

		csp_packet_t *entry_packet = csp_buffer_get(packet_size);

		if (!entry_packet) {
			continue;
		}

		ft_packet_header_t *header = (ft_packet_header_t*) entry_packet->data;
		header->cmd = FT_CMD_LIST;
		header->flags = FT_FLAG_MORE;
		header->status = 0;
		header->seq = seq;
		header->payload_len = payload_size;

		uint16_t *path_len_ptr = (uint16_t*) (entry_packet->data + sizeof(ft_packet_header_t));
		*path_len_ptr = path_len;

		char *p = (char*) (entry_packet->data + sizeof(ft_packet_header_t) + sizeof(uint16_t));
		memcpy(p, path, path_len);
		p += path_len;

		*((uint16_t*) p) = (uint16_t) name_len;
		p += sizeof(uint16_t);
		memcpy(p, info.name, name_len);
		p += name_len;
		*((uint32_t*) p) = info.size;
		p += sizeof(uint32_t);
		*((uint8_t*) p) = info.is_dir;

		entry_packet->length = packet_size;

		if (!csp_send(conn, entry_packet, FT_CSP_TIMEOUT_MS)) {
			csp_buffer_free(entry_packet);
		}

		seq++;
	}

	vfs->closedir(&dp);
}

static void handle_read_chunk(csp_conn_t *conn, csp_packet_t *packet) {
	ft_packet_header_t *req_header = (ft_packet_header_t*) packet->data;
	uint16_t req_seq = req_header->seq;

	uint16_t path_len;
	const char *path = extract_path(packet, &path_len);
	if (!path) {
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, FM_ERR_INVAL);
		return;
	}

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, FM_ERR_NOENT);
		return;
	}

	// Copy to local buffer
	char local_path[FM_MAX_PATH_LEN];
	const char *stripped = fm_strip_prefix(path);
	strncpy(local_path, stripped, sizeof(local_path) - 1);
	local_path[sizeof(local_path) - 1] = '\0';

	size_t header_path_size = sizeof(ft_packet_header_t) + sizeof(uint16_t) + path_len;

	if (packet->length < header_path_size + sizeof(uint32_t) * 2) {
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, FM_ERR_INVAL);
		return;
	}

	uint32_t offset = *((uint32_t*) (packet->data + header_path_size));
	uint32_t length = *((uint32_t*) (packet->data + header_path_size + sizeof(uint32_t)));

	fm_file_handle_t *fp = NULL;
	fm_err_t res = vfs->open(&fp, local_path, FM_O_READ);
	if (res != FM_OK) {
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, res);
		return;
	}

	res = vfs->seek(fp, offset);
	if (res != FM_OK) {
		vfs->close(&fp);
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, res);
		return;
	}

	uint32_t bytes_to_read = (length > FT_MAX_DATA_LEN) ? FT_MAX_DATA_LEN : length;

	uint8_t *read_buf = malloc(bytes_to_read);
	if (!read_buf) {
		vfs->close(&fp);
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, FM_ERR_IO);
		return;
	}

	uint32_t bytes_read = 0;
	res = vfs->read(fp, read_buf, bytes_to_read, &bytes_read);

	if (res == FM_OK) {
		send_path_response(conn, FT_CMD_READ_CHUNK, req_seq, FM_OK, path, bytes_read, read_buf);
	} else {
		send_simple_response(conn, FT_CMD_READ_CHUNK, req_seq, res);
	}

	free(read_buf);
	vfs->close(&fp);
}

void ft_server_handler(csp_conn_t *conn, csp_packet_t *packet) {
	if (!packet || !conn) {
		return;
	}

	ft_packet_header_t *header = (ft_packet_header_t*) packet->data;

	switch (header->cmd) {
	case FT_CMD_STAT:
		handle_stat(conn, packet);
		break;

	case FT_CMD_GET_SPACE:
		handle_get_space(conn, packet);
		break;

	case FT_CMD_MKDIR:
		handle_mkdir(conn, packet);
		break;

	case FT_CMD_DELETE:
		handle_delete(conn, packet);
		break;

	case FT_CMD_RENAME:
		handle_rename(conn, packet);
		break;

	case FT_CMD_COPY:
		handle_copy(conn, packet);
		break;

	case FT_CMD_READ:
		handle_read(conn, packet);
		csp_buffer_free(packet);
		return;

	case FT_CMD_WRITE: {
		ft_write_ctx_t *ctx = get_write_ctx(conn);
		if (!ctx) {
			send_simple_response(conn, FT_CMD_WRITE, 0, FM_ERR_IO);
		} else {
			handle_write(conn, packet, ctx);
		}
		break;
	}

	case FT_CMD_LIST:
		handle_list(conn, packet);
		csp_buffer_free(packet);
		return;

	case FT_CMD_READ_CHUNK:
		handle_read_chunk(conn, packet);
		break;

	default:
		send_simple_response(conn, header->cmd, 0, FM_ERR_INVAL);
		break;
	}

	csp_buffer_free(packet);
}

CSP_DEFINE_TASK(csp_ft_server) {
	csp_file_server_context_t *csx = param;
	fm_err_t err = file_manager_init();
	if (err != FM_OK) {
		csp_log_error("File Manager initialization failed: %d", err);
	}
	csp_socket_t *sock = csp_socket(CSP_SO_NONE);

	if (csp_bind(sock, csx->port) == CSP_ERR_USED) {
		csp_close(sock);
		sock = csp_port_get_socket(csx->port);
	} else {
		csp_listen(sock, CSP_FT_SERVER_CON_MAX);
	}

	while (1) {
		thread_wd_kick_self();

		csp_conn_t *conn;
		if ((conn = csp_accept(sock, 1000)) == NULL) {
			continue;
		}

		csp_packet_t *packet;
		while ((packet = csp_read(conn, 100)) != NULL) {
			if (csp_conn_dport(conn) == csx->port) {
				ft_server_handler(conn, packet);
			}
		}

		clear_write_ctx(conn);

		csp_close(conn);
	}
	return CSP_TASK_RETURN;
}
