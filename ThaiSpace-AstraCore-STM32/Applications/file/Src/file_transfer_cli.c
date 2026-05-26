#include "app_modules_config.h"
#if CSP_ENABLE_FILE

/*
 * Project: File V3 for TSCNarit
 * Author: Chaturong Khachaban
 * Description: Flight-grade embedded system module
 */

#include <kservice.h>
#include <csp/csp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#ifdef TSC_USING_FINSH
#include "finsh.h"

#include "file_transfer_standard.h"
#include "file_transfer_highlevel.h"
#include "file_core.h"
#include "ft_config.h"

#define FT_CLI_BUF_SIZE 256

static void __ft_stat(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_stat <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	fm_file_info_t info;
	fm_err_t err = ft_stat(node, path, &info);
	if (err == FM_OK) {
		char type = info.is_dir ? 'd' : '-';
		tsc_kprintf("%c %10lu  %s\r\n", type, (unsigned long) info.size, info.name);
	} else {
		tsc_kprintf("ft_stat failed: %d\r\n", err);
	}
}

static void __ft_mkdir(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_mkdir <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	fm_err_t err = ft_mkdir(node, path);
	if (err == FM_OK) {
		tsc_kprintf("Directory created: %s\r\n", path);
	} else {
		tsc_kprintf("ft_mkdir failed: %d\r\n", err);
	}
}

static void __ft_rm(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_rm <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	fm_err_t err = ft_rm(node, path);
	if (err == FM_OK) {
		tsc_kprintf("Removed: %s\r\n", path);
	} else {
		tsc_kprintf("ft_rm failed: %d\r\n", err);
	}
}

static void __ft_mv(int argc, char **argv) {
	if (argc < 4) {
		tsc_kprintf("Usage: ft_mv <node> <src> <dst>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *src = argv[2];
	const char *dst = argv[3];

	fm_err_t err = ft_mv(node, src, dst);
	if (err == FM_OK) {
		tsc_kprintf("Moved: %s -> %s\r\n", src, dst);
	} else {
		tsc_kprintf("ft_mv failed: %d\r\n", err);
	}
}

static void __ft_space(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: ft_space <node>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);

	uint32_t total, free;
	fm_err_t err = ft_get_space(node, &total, &free);
	if (err == FM_OK) {
		tsc_kprintf("Disk space: %lu KB total, %lu KB free\r\n", (unsigned long) total, (unsigned long) free);
	} else {
		tsc_kprintf("ft_get_space failed: %d\r\n", err);
	}
}

static void __ft_ls(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_ls <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	tsc_kprintf("Listing %s:\r\n", path);
	ft_list(node, path);
}

static void __ft_cp(int argc, char **argv) {
	if (argc < 4) {
		tsc_kprintf("Usage: ft_cp <node> <src> <dst>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *src = argv[2];
	const char *dst = argv[3];

	fm_err_t err = ft_cp(node, src, dst);
	if (err == FM_OK) {
		tsc_kprintf("Copied: %s -> %s\r\n", src, dst);
	} else {
		tsc_kprintf("ft_cp failed: %d\r\n", err);
	}
}

static void __ft_cat(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_cat <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	int32_t handle = ft_open(node, path, FA_READ);
	if (handle < 0) {
		tsc_kprintf("Failed to open file: %d\r\n", -handle);
		return;
	}

	uint8_t buf[FT_CLI_BUF_SIZE];
	int32_t r;
	uint32_t total = 0;

	while ((r = ft_read(node, handle, buf, sizeof(buf))) > 0) {
		for (int i = 0; i < r; i++) {
			if (buf[i] == '\r') {
				tsc_kprintf("\r\n");
			} else if (buf[i] >= 32 && buf[i] < 127) {
				tsc_kprintf("%c", buf[i]);
			} else if (buf[i] == '\n') {
				tsc_kprintf("\r\n");
			}
		}
		total += r;
	}

	if (r < 0) {
		tsc_kprintf("\r\nft_read error: %d\r\n", -r);
	} else {
		tsc_kprintf("\r\n[Total: %lu bytes]\r\n", (unsigned long) total);
	}

	ft_close(node, handle);
}

static void __ft_echo(int argc, char **argv) {
	if (argc < 4) {
		tsc_kprintf("Usage: ft_echo <node> <text> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *text = argv[2];
	const char *path = argv[3];

	int32_t handle = ft_open(node, path, FA_WRITE | FA_CREATE_ALWAYS);
	if (handle < 0) {
		tsc_kprintf("Failed to open file: %d\r\n", -handle);
		return;
	}

	size_t len = strlen(text);
	int32_t w = ft_write(node, handle, text, len);
	ft_close(node, handle);

	if (w >= 0) {
		tsc_kprintf("Wrote %d bytes to %s\r\n", w, path);
	} else {
		tsc_kprintf("ft_write failed: %d\r\n", -w);
	}
}

static void __ft_append(int argc, char **argv) {
	if (argc < 4) {
		tsc_kprintf("Usage: ft_append <node> <text> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *text = argv[2];
	const char *path = argv[3];

	int32_t handle = ft_open(node, path, FA_WRITE | FA_OPEN_APPEND);
	if (handle < 0) {
		tsc_kprintf("Failed to open file: %d\r\n", -handle);
		return;
	}

	size_t len = strlen(text);
	int32_t w = ft_write(node, handle, text, len);
	ft_close(node, handle);

	if (w >= 0) {
		tsc_kprintf("Appended %d bytes to %s\r\n", w, path);
	} else {
		tsc_kprintf("ft_write failed: %d\r\n", -w);
	}
}

static void __ft_touch(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: ft_touch <node> <path>\r\n");
		return;
	}
	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *path = argv[2];

	int32_t handle = ft_open(node, path, FA_WRITE | FA_CREATE_ALWAYS);
	if (handle >= 0) {
		ft_close(node, handle);
		tsc_kprintf("Created: %s\r\n", path);
	} else {
		tsc_kprintf("ft_touch failed: %d\r\n", -handle);
	}
}

static void __ls(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: ls <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_dir_handle_t *dp = NULL;
	fm_err_t err = fm_opendir(path, &dp);
	if (err != FM_OK) {
		tsc_kprintf("fm_opendir failed: %d\r\n", err);
		return;
	}

	tsc_kprintf("%-30s %-10s %-10s\r\n", "Name", "Type", "Size");
	tsc_kprintf("%-30s %-10s %-10s\r\n", "------------------------------", "----------", "----------");

	fm_file_info_t info;
	int count = 0;

	while (1) {
		err = fm_readdir(dp, &info);
		if (err != FM_OK || (info.name[0] == '\0' || info.name[0] == 0)) {
			break;
		}

		char type = info.is_dir ? 'd' : '-';
		tsc_kprintf("%-30s %-10c %-10lu\r\n", info.name, type, (unsigned long) info.size);
		count++;
	}

	fm_closedir(&dp);
	tsc_kprintf("Total entries: %d\r\n", count);
}

static void __stat(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: stat <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_file_info_t info;
	fm_err_t err = fm_stat(path, &info);
	if (err == FM_OK) {
		char type = info.is_dir ? 'd' : '-';
		tsc_kprintf("%c %10lu  %s\r\n", type, (unsigned long) info.size, info.name);
	} else {
		tsc_kprintf("fm_stat failed: %d\r\n", err);
	}
}

static void __mkdir(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: mkdir <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_err_t err = fm_mkdir(path);
	if (err == FM_OK) {
		tsc_kprintf("Directory created: %s\r\n", path);
	} else {
		tsc_kprintf("fm_mkdir failed: %d\r\n", err);
	}
}

static void __rm(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: rm <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_err_t err = fm_rm(path);
	if (err == FM_OK) {
		tsc_kprintf("Removed: %s\r\n", path);
	} else {
		tsc_kprintf("fm_rm failed: %d\r\n", err);
	}
}

static void __mv(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: mv <src> <dst>\r\n");
		return;
	}
	const char *src = argv[1];
	const char *dst = argv[2];

	fm_err_t err = fm_mv(src, dst);
	if (err == FM_OK) {
		tsc_kprintf("Moved: %s -> %s\r\n", src, dst);
	} else {
		tsc_kprintf("fm_mv failed: %d\r\n", err);
	}
}

static void __cp(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: cp <src> <dst>\r\n");
		tsc_kprintf("  Copy file or directory\r\n");
		tsc_kprintf("  Supports cross-filesystem copy (SD <-> NOR)\r\n");
		tsc_kprintf("  Examples:\r\n");
		tsc_kprintf("    cp /sd/file.txt /nor/backup/file.txt\r\n");
		tsc_kprintf("    cp /sd/logs /nor/logs_backup\r\n");
		return;
	}
	const char *src = argv[1];
	const char *dst = argv[2];

	fm_file_info_t src_info;
	fm_err_t err = fm_stat(src, &src_info);
	if (err != FM_OK) {
		tsc_kprintf("Source not found: %s (err: %d)\r\n", src, err);
		return;
	}

	if (src_info.is_dir) {
		tsc_kprintf("Copying directory: %s -> %s\r\n", src, dst);
	} else {
		tsc_kprintf("Copying file: %s -> %s (%lu bytes)\r\n", src, dst, (unsigned long) src_info.size);
	}

	err = fm_copy(src, dst);
	if (err == FM_OK) {
		tsc_kprintf("Copy completed successfully\r\n");
	} else {
		tsc_kprintf("fm_copy failed: %d\r\n", err);
	}
}

static void __space(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: space <path>\r\n");
		return;
	}
	const char *path = argv[1];

	const fm_vfs_t *vfs = fm_get_vfs(path);
	if (!vfs) {
		tsc_kprintf("No VFS found for path: %s\r\n", path);
		return;
	}

	if (!vfs->statfs) {
		tsc_kprintf("statfs not supported by this VFS\r\n");
		return;
	}

	const char *local_path = fm_strip_prefix(path);

	fm_fs_stats_t stats;
	fm_err_t err = vfs->statfs(local_path, &stats);
	if (err == FM_OK) {
		uint32_t total_kb = stats.total_bytes / 1024;
		uint32_t free_kb = stats.free_bytes / 1024;
		uint32_t used_kb = (stats.total_bytes - stats.free_bytes) / 1024;
		tsc_kprintf("Filesystem: %s\r\n", path);
		tsc_kprintf("  Total: %lu KB\r\n", (unsigned long) total_kb);
		tsc_kprintf("  Used:  %lu KB\r\n", (unsigned long) used_kb);
		tsc_kprintf("  Free:  %lu KB\r\n", (unsigned long) free_kb);
	} else {
		tsc_kprintf("statfs failed: %d\r\n", err);
	}
}

static void __cat(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: cat <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_file_handle_t *fp = NULL;
	fm_err_t err = fm_open(path, FM_O_READ, &fp);
	if (err != FM_OK) {
		tsc_kprintf("Failed to open file: %d\r\n", err);
		return;
	}

	uint8_t buf[FT_CLI_BUF_SIZE];
	uint32_t br = 0;
	uint32_t total = 0;

	while (1) {
		err = fm_read(fp, buf, sizeof(buf), &br);
		if (err != FM_OK || br == 0) {
			break;
		}

		for (uint32_t i = 0; i < br; i++) {
			if (buf[i] == '\r') {
				tsc_kprintf("\r\n");
			} else if (buf[i] >= 32 && buf[i] < 127) {
				tsc_kprintf("%c", buf[i]);
			} else if (buf[i] == '\n') {
				tsc_kprintf("\r\n");
			}
		}
		total += br;
	}

	if (err != FM_OK) {
		tsc_kprintf("\r\nfm_read error: %d\r\n", err);
	} else {
		tsc_kprintf("\r\n[Total: %lu bytes]\r\n", (unsigned long) total);
	}

	fm_close(&fp);
}

static void __echo(int argc, char **argv) {
	if (argc < 3) {
		tsc_kprintf("Usage: echo <text> <path>\r\n");
		return;
	}
	const char *text = argv[1];
	const char *path = argv[2];

	fm_file_handle_t *fp = NULL;
	fm_err_t err = fm_open(path, FM_O_WRITE | FM_O_CREATE, &fp);
	if (err != FM_OK) {
		tsc_kprintf("Failed to open file: %d\r\n", err);
		return;
	}

	size_t len = strlen(text);
	uint32_t bw = 0;
	err = fm_write(fp, text, len, &bw);
	fm_close(&fp);

	if (err == FM_OK) {
		tsc_kprintf("Wrote %lu bytes to %s\r\n", (unsigned long) bw, path);
	} else {
		tsc_kprintf("fm_write failed: %d\r\n", err);
	}
}

static void __touch_cmd(int argc, char **argv) {
	if (argc < 2) {
		tsc_kprintf("Usage: touch <path>\r\n");
		return;
	}
	const char *path = argv[1];

	fm_file_handle_t *fp = NULL;
	fm_err_t err = fm_open(path, FM_O_WRITE | FM_O_CREATE, &fp);
	if (err == FM_OK) {
		fm_close(&fp);
		tsc_kprintf("Created: %s\r\n", path);
	} else {
		tsc_kprintf("fm_open failed: %d\r\n", err);
	}
}

static void __ft_upload(int argc, char **argv) {
	/* Usage: ft_upload <node> <local_path> <remote_path> [options]
	 * Options: --resume auto-resume, --offset <bytes>, -c<size> chunk size, -t<ms> timeout, -R<count> retries, -n disable CRC
	 */
	if (argc < 4) {
		tsc_kprintf("Usage: ft_upload <node> <local_path> <remote_path> [options]\r\n");
		tsc_kprintf("Options:\r\n");
		tsc_kprintf("  --resume     Auto-resume from existing remote file size\r\n");
		tsc_kprintf("  --offset <bytes>  Resume from specific byte offset\r\n");
		tsc_kprintf("  -c<size>     Chunk size in bytes (128-200, default: 200)\r\n");
		tsc_kprintf("  -t<ms>       Timeout per chunk in ms (default: 2000)\r\n");
		tsc_kprintf("  -R<count>    Retry count (default: 3)\r\n");
		tsc_kprintf("  -n           Disable CRC32 verification\r\n");
		tsc_kprintf("Examples:\r\n");
		tsc_kprintf("  ft_upload 11 /D/file.bin /D/file.bin\r\n");
		tsc_kprintf("  ft_upload 11 /D/file.bin /D/file.bin --resume\r\n");
		tsc_kprintf("  ft_upload 11 /D/file.bin /D/file.bin --offset 102400\r\n");
		return;
	}

	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *local_path = argv[2];
	const char *remote_path = argv[3];

	/* Parse options */
	ft_options_t opt = FT_DEFAULT_OPTIONS;
	bool auto_resume = false;

	for (int i = 4; i < argc; i++) {
		char *arg = argv[i];
		if (strcmp(arg, "--resume") == 0) {
			auto_resume = true;
		} else if (strcmp(arg, "--offset") == 0 && i + 1 < argc) {
			opt.resume_offset = (uint32_t) atoi(argv[++i]);
		} else if (arg[0] == '-') {
			switch (arg[1]) {
			case 'c':
				opt.chunk_size = (uint16_t) atoi(&arg[2]);
				if (opt.chunk_size < 128) opt.chunk_size = 128;
				if (opt.chunk_size > FTS_MAX_CHUNK_SIZE) opt.chunk_size = FTS_MAX_CHUNK_SIZE;
				break;
			case 't':
				opt.timeout_ms = (uint32_t) atoi(&arg[2]);
				break;
			case 'R':
				opt.retries = (uint8_t) atoi(&arg[2]);
				break;
			case 'n':
				opt.verify_crc = 0;
				break;
			case 'p':
				opt.priority = (uint8_t) atoi(&arg[2]);
				if (opt.priority > 3) opt.priority = 3;
				break;
			}
		}
	}

	/* Auto-resume: get remote file size */
	if (auto_resume) {
		fm_file_info_t remote_info;
		fm_err_t stat_err = ft_stat(node, remote_path, &remote_info);
		if (stat_err == FM_OK && !remote_info.is_dir) {
			opt.resume_offset = remote_info.size;
			tsc_kprintf("Auto-resume: remote file size = %lu bytes\r\n", (unsigned long) opt.resume_offset);
		} else {
			tsc_kprintf("Auto-resume: remote file not found, starting from beginning\r\n");
			opt.resume_offset = 0;
		}
	}

	tsc_kprintf("Uploading: %s -> node %d:%s\r\n", local_path, node, remote_path);
	if (opt.resume_offset > 0) {
		tsc_kprintf("Resuming from offset: %lu bytes\r\n", (unsigned long) opt.resume_offset);
	}
	tsc_kprintf("Options: chunk=%u, timeout=%lu, retries=%u, prio=%u, crc=%s\r\n", opt.chunk_size, (unsigned long) opt.timeout_ms, opt.retries, opt.priority,
		opt.verify_crc ? "enabled" : "disabled");

	fm_err_t err = ft_upload(node, local_path, remote_path, &opt);
	if (err == FM_OK) {
		tsc_kprintf("Upload completed successfully\r\n");
	} else {
		tsc_kprintf("Upload failed: %d\r\n", err);
	}
}

static void __ft_download(int argc, char **argv) {
	/* Usage: ft_download <node> <remote_path> <local_path> [options]
	 * Options: --resume auto-resume, --offset <bytes>, -c<size> chunk size, -t<ms> timeout, -R<count> retries, -n disable CRC
	 */
	if (argc < 4) {
		tsc_kprintf("Usage: ft_download <node> <remote_path> <local_path> [options]\r\n");
		tsc_kprintf("Options:\r\n");
		tsc_kprintf("  --resume     Auto-resume from existing local file size\r\n");
		tsc_kprintf("  --offset <bytes>  Resume from specific byte offset\r\n");
		tsc_kprintf("  -c<size>     Chunk size in bytes (128-200, default: 200)\r\n");
		tsc_kprintf("  -t<ms>       Timeout per chunk in ms (default: 2000)\r\n");
		tsc_kprintf("  -R<count>    Retry count (default: 3)\r\n");
		tsc_kprintf("  -n           Disable CRC32 verification\r\n");
		tsc_kprintf("Examples:\r\n");
		tsc_kprintf("  ft_download 11 /D/file.bin /D/file.bin\r\n");
		tsc_kprintf("  ft_download 11 /D/file.bin /D/file.bin --resume\r\n");
		tsc_kprintf("  ft_download 11 /D/file.bin /D/file.bin --offset 102400\r\n");
		return;
	}

	uint8_t node = (uint8_t) atoi(argv[1]);
	const char *remote_path = argv[2];
	const char *local_path = argv[3];

	/* Parse options */
	ft_options_t opt = FT_DEFAULT_OPTIONS;
	bool auto_resume = false;

	for (int i = 4; i < argc; i++) {
		char *arg = argv[i];
		if (strcmp(arg, "--resume") == 0) {
			auto_resume = true;
		} else if (strcmp(arg, "--offset") == 0 && i + 1 < argc) {
			opt.resume_offset = (uint32_t) atoi(argv[++i]);
		} else if (arg[0] == '-') {
			switch (arg[1]) {
			case 'c':
				opt.chunk_size = (uint16_t) atoi(&arg[2]);
				if (opt.chunk_size < 128) opt.chunk_size = 128;
				if (opt.chunk_size > FTS_MAX_CHUNK_SIZE) opt.chunk_size = FTS_MAX_CHUNK_SIZE;
				break;
			case 't':
				opt.timeout_ms = (uint32_t) atoi(&arg[2]);
				break;
			case 'R':
				opt.retries = (uint8_t) atoi(&arg[2]);
				break;
			case 'n':
				opt.verify_crc = 0;
				break;
			case 'p':
				opt.priority = (uint8_t) atoi(&arg[2]);
				if (opt.priority > 3) opt.priority = 3;
				break;
			}
		}
	}

	/* Auto-resume: get local file size */
	if (auto_resume) {
		fm_file_info_t local_info;
		fm_err_t stat_err = fm_stat(local_path, &local_info);
		if (stat_err == FM_OK && !local_info.is_dir) {
			opt.resume_offset = local_info.size;
			tsc_kprintf("Auto-resume: local file size = %lu bytes\r\n", (unsigned long) opt.resume_offset);
		} else {
			tsc_kprintf("Auto-resume: local file not found, starting from beginning\r\n");
			opt.resume_offset = 0;
		}
	}

	tsc_kprintf("Downloading: node %d:%s -> %s\r\n", node, remote_path, local_path);
	if (opt.resume_offset > 0) {
		tsc_kprintf("Resuming from offset: %lu bytes\r\n", (unsigned long) opt.resume_offset);
	}
	tsc_kprintf("Options: chunk=%u, timeout=%lu, retries=%u, prio=%u, crc=%s\r\n", opt.chunk_size, (unsigned long) opt.timeout_ms, opt.retries, opt.priority,
		opt.verify_crc ? "enabled" : "disabled");

	fm_err_t err = ft_download(node, remote_path, local_path, &opt);
	if (err == FM_OK) {
		tsc_kprintf("Download completed successfully\r\n");
	} else {
		tsc_kprintf("Download failed: %d\r\n", err);
	}
}

TSC_CMD_EXPORT_ALIAS(__ft_upload, ft_upload, "Upload file (Ground -> Satellite)");
TSC_CMD_EXPORT_ALIAS(__ft_download, ft_download, "Download file (Satellite -> Ground)");
TSC_CMD_EXPORT_ALIAS(__ft_stat, ft_stat, "Get file/directory info (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_mkdir, ft_mkdir, "Create directory (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_rm, ft_rm, "Remove file/directory (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_mv, ft_mv, "Move/rename file/directory (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_space, ft_space, "Get disk space (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_ls, ft_ls, "List directory contents (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_cp, ft_cp, "Copy file (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_cat, ft_cat, "Display file contents (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_echo, ft_echo, "Write text to file (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_append, ft_append, "Append text to file (remote)");
TSC_CMD_EXPORT_ALIAS(__ft_touch, ft_touch, "Create empty file (remote)");

TSC_CMD_EXPORT_ALIAS(__ls, ls, "List directory (local)");
TSC_CMD_EXPORT_ALIAS(__stat, stat, "Get file/directory info (local)");
TSC_CMD_EXPORT_ALIAS(__mkdir, mkdir, "Create directory (local)");
TSC_CMD_EXPORT_ALIAS(__rm, rm, "Remove file/directory (local)");
TSC_CMD_EXPORT_ALIAS(__mv, mv, "Move/rename file/directory (local)");
TSC_CMD_EXPORT_ALIAS(__cp, cp, "Copy file/directory (local)");
TSC_CMD_EXPORT_ALIAS(__space, space, "Get disk space (local)");
TSC_CMD_EXPORT_ALIAS(__cat, cat, "Display file contents (local)");
TSC_CMD_EXPORT_ALIAS(__echo, echo, "Write text to file (local)");
TSC_CMD_EXPORT_ALIAS(__touch_cmd, touch, "Create empty file (local)");

#endif


#endif /* CSP_ENABLE_FILE */
