#include "app_modules_config.h"
#if CSP_ENABLE_LOG

#include <hw_config.h>
#include <file_core.h>
#include <tsc_log_core.h>
#include <time.h>

#define TSC_LOG_FILE_BE_NAME    "file"

#ifndef TSC_LOG_FILE_ROOT_PATH
#define TSC_LOG_FILE_ROOT_PATH  FM_SD_PREFIX "logs"
#endif

#ifndef TSC_LOG_FILE_NAME_PREFIX
#define TSC_LOG_FILE_NAME_PREFIX  "tsc_log"
#endif

#ifndef TSC_LOG_FILE_MAX_NUM
#define TSC_LOG_FILE_MAX_NUM    5
#endif
#ifndef TSC_LOG_FILE_MAX_SIZE
#define TSC_LOG_FILE_MAX_SIZE   (512 * 1024)
#endif

#define TSC_LOG_FILE_PATH_LEN   128

#if defined(TSC_LOG_ASYNC_OUTPUT_THREAD_STACK) && (TSC_LOG_ASYNC_OUTPUT_THREAD_STACK < 2048)
#error "The value of TSC_LOG_ASYNC_OUTPUT_THREAD_STACK must be greater than 2048."
#endif

static struct tsc_log_backend tsc_log_file;
static char g_file_path[TSC_LOG_FILE_PATH_LEN] = {0};
static fm_file_handle_t *g_fp = NULL;
static bool g_file_opened = TSC_FALSE;

/* cleanup old log files, keeping only TSC_LOG_FILE_MAX_NUM files */
static void tsc_log_file_cleanup(void)
{
    fm_dir_handle_t *dp = NULL;
    fm_file_info_t fno;
    char oldest_file[FM_MAX_NAME_LEN];
    int count;
    size_t prefix_len = tsc_strlen(TSC_LOG_FILE_NAME_PREFIX);

    while (1) {
        count = 0;
        oldest_file[0] = '\0';

        if (fm_opendir(TSC_LOG_FILE_ROOT_PATH, &dp) != FM_OK) {
            break;
        }

        while (fm_readdir(dp, &fno) == FM_OK && fno.name[0] != 0) {
            if (!fno.is_dir && tsc_strncmp(fno.name, TSC_LOG_FILE_NAME_PREFIX, prefix_len) == 0 &&
                tsc_strstr(fno.name, ".log")) {
                count++;
                if (oldest_file[0] == '\0' || tsc_strcmp(fno.name, oldest_file) < 0) {
                    tsc_strncpy(oldest_file, fno.name, sizeof(oldest_file) - 1);
                }
            }
        }
        fm_closedir(&dp);

        if (count >= TSC_LOG_FILE_MAX_NUM && oldest_file[0] != '\0') {
            char full_path[TSC_LOG_FILE_PATH_LEN];
            tsc_snprintf(full_path, sizeof(full_path), "%s/%s", TSC_LOG_FILE_ROOT_PATH, oldest_file);
            fm_rm(full_path);
        } else {
            break;
        }
    }
}

/* rotate the log file: just close and let output() open a new one with a new timestamp */
static bool tsc_log_file_rotate(void)
{
    /* close current file if already open */
    if (g_file_opened) {
        fm_close(&g_fp);
        g_file_opened = false;
    }
    
    /* Clear path to force new filename generation in next output */
    g_file_path[0] = '\0';

    tsc_log_file_cleanup();

    return TSC_TRUE;
}

static void tsc_log_file_backend_output(struct tsc_log_backend *backend, uint32_t level, const char *tag, bool is_raw, const char *log, size_t len)
{
    fm_err_t rc;
    static bool dir_checked = false;
    static uint32_t write_count = 0;
    uint32_t bw;

    /* check log file directory once */
    if (!dir_checked) {
        if (!fm_path_exists(TSC_LOG_FILE_ROOT_PATH)) {
            fm_mkdir(TSC_LOG_FILE_ROOT_PATH);
        }
        dir_checked = true;
    }

    /* generate filename if not already */
    if (g_file_path[0] == '\0')
    {
        time_t now = time(NULL);
        struct tm tm_now;
        gmtime_r(&now, &tm_now);

        tsc_snprintf(g_file_path, TSC_LOG_FILE_PATH_LEN, "%s/%s_%04d%02d%02d_%02d%02d%02d.log",
                     TSC_LOG_FILE_ROOT_PATH, TSC_LOG_FILE_NAME_PREFIX,
                     tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
                     tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);
        write_count = 0;
        
        /* Cleanup old logs when a new file is created (e.g., at startup or after rotation) */
        tsc_log_file_cleanup();
    }

    /* Open, write and close immediately to allow other processes to read */
    rc = fm_open(g_file_path, FM_O_WRITE | FM_O_APPEND | FM_O_CREATE, &g_fp);
    if (rc != FM_OK) {
        return;
    }

    rc = fm_write(g_fp, log, len, &bw);
    fm_sync(g_fp);
    fm_close(&g_fp);
    g_file_opened = false;

    /* check for rotation every 10 writes to reduce fm_stat overhead */
    if (++write_count >= 10) {
        fm_file_info_t fno;
        write_count = 0;
        rc = fm_stat(g_file_path, &fno);
        if (rc == FM_OK && fno.size > TSC_LOG_FILE_MAX_SIZE) {
            tsc_log_file_rotate();
        }
    }
}

/* initialize the tsc_log file backend */
int tsc_log_file_backend_init(void)
{
    tsc_log_file.output = tsc_log_file_backend_output;
    tsc_log_backend_register(&tsc_log_file, TSC_LOG_FILE_BE_NAME, TSC_FALSE);
    return 0;
}

/* uninitialize the tsc_log file backend */
int tsc_log_file_backend_deinit(void)
{
    if (g_file_opened)
    {
        fm_close(&g_fp);
        g_file_opened = false;
    }

    tsc_log_backend_unregister(&tsc_log_file);
    return 0;
}


#endif /* CSP_ENABLE_LOG */
