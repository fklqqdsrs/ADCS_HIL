#include <stdint.h>
#include <stdarg.h>
#include <tsc_log_core.h>
#include "hw_config.h"

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
#include "ringbuffer.h"
#include "ringblk_buf.h"
#endif

#ifdef TSC_LOG_USING_SYSLOG
#include <syslog.h>
#endif

#ifdef TSC_LOG_TIME_USING_TIMESTAMP
#include <sys/time.h>
#endif

#if CSP_ENABLE_LOG

/* the number which is max stored line logs */
#ifndef TSC_LOG_ASYNC_OUTPUT_STORE_LINES
#define TSC_LOG_ASYNC_OUTPUT_STORE_LINES  (TSC_LOG_ASYNC_OUTPUT_BUF_SIZE * 3 / 2 / 80)
#endif

#ifdef TSC_LOG_USING_COLOR
/**
 * CSI(Control Sequence Introducer/Initiator) sign
 * more information on https://en.wikipedia.org/wiki/ANSI_escape_code
 */
#define CSI_START                      "\033["
#define CSI_END                        "\033[0m"
/* output log front color */
#define F_BLACK                        "30m"
#define F_RED                          "31m"
#define F_GREEN                        "32m"
#define F_YELLOW                       "33m"
#define F_BLUE                         "34m"
#define F_MAGENTA                      "35m"
#define F_CYAN                         "36m"
#define F_WHITE                        "37m"

/* output log default color definition */
#ifndef TSC_LOG_COLOR_DEBUG
#define TSC_LOG_COLOR_DEBUG               NULL
#endif
#ifndef TSC_LOG_COLOR_INFO
#define TSC_LOG_COLOR_INFO                (F_GREEN)
#endif
#ifndef TSC_LOG_COLOR_WARN
#define TSC_LOG_COLOR_WARN                (F_YELLOW)
#endif
#ifndef TSC_LOG_COLOR_ERROR
#define TSC_LOG_COLOR_ERROR               (F_RED)
#endif
#ifndef TSC_LOG_COLOR_ASSERT
#define TSC_LOG_COLOR_ASSERT              (F_MAGENTA)
#endif
#endif /* TSC_LOG_USING_COLOR */

#if TSC_LOG_LINE_BUF_SIZE < 80
#error "the log line buffer size must more than 80"
#endif

struct tsc_log {
	bool init_ok;
	bool output_lock_enabled;
	osMutexId_t output_locker;
	/* all backends */
	tsc_slist_t backend_list;
	/* the thread log's line buffer */
	char log_buf_th[TSC_LOG_LINE_BUF_SIZE + 1];

#ifdef TSC_LOG_USING_ISR_LOG
	/* the ISR log's line buffer */
	long output_locker_isr_lvl;
	char log_buf_isr[TSC_LOG_LINE_BUF_SIZE + 1];
#endif /* TSC_LOG_USING_ISR_LOG */

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	bool async_enabled;
	rbb_t async_rbb;
	/* ringbuffer for log_raw function only */
	struct ringbuffer *async_rb;
	osThreadId_t async_th;
	osSemaphoreId_t async_notice;
#endif

#ifdef TSC_LOG_USING_FILTER
	struct {
		/* all tag's level filter */
		tsc_slist_t tag_lvl_list;
		/* global filter level, tag and keyword */
		uint32_t level;
		char tag[TSC_LOG_FILTER_TAG_MAX_LEN + 1];
		char keyword[TSC_LOG_FILTER_KW_MAX_LEN + 1];
	} filter;
#endif /* TSC_LOG_USING_FILTER */
};

/* level output info */
static const char *const level_output_info[] = { "A/",
NULL,
NULL, "E/", "W/",
NULL, "I/", "D/", };

#ifdef TSC_LOG_USING_COLOR
/* color output info */
static const char *const color_output_info[] = {
TSC_LOG_COLOR_ASSERT,
NULL,
NULL,
TSC_LOG_COLOR_ERROR,
TSC_LOG_COLOR_WARN,
NULL,
TSC_LOG_COLOR_INFO,
TSC_LOG_COLOR_DEBUG, };
#endif /* TSC_LOG_USING_COLOR */

/* tsc_log local object */
static struct tsc_log tsc_log = { 0 };

size_t tsc_log_strcpy(size_t cur_len, char *dst, const char *src) {
	const char *src_old = src;

	ASSERT(dst);
	ASSERT(src);

	while (*src != 0) {
		/* make sure destination has enough space */
		if (cur_len++ < TSC_LOG_LINE_BUF_SIZE) {
			*dst++ = *src++;
		} else {
			break;
		}
	}
	return src - src_old;
}

size_t tsc_log_ultoa(char *s, unsigned long int n) {
	size_t i = 0, j = 0, len = 0;
	char swap;

	do {
		s[len++] = n % 10 + '0';
	} while (n /= 10);
	s[len] = '\0';
	/* reverse string */
	for (i = 0, j = len - 1; i < j; ++i, --j) {
		swap = s[i];
		s[i] = s[j];
		s[j] = swap;
	}
	return len;
}

static void output_unlock(void) {
	/* earlier stage */
	if (tsc_log.output_lock_enabled == TSC_FALSE) {
		return;
	}

	/* If the scheduler is started and in thread context */
	if (__get_IPSR() == 0 && osThreadGetId() != NULL) {
		osMutexRelease(tsc_log.output_locker);
	} else {
#ifdef TSC_LOG_USING_ISR_LOG
		hw_interrupt_enable(tsc_log.output_locker_isr_lvl);
#endif
	}
}

static void output_lock(void) {
	/* earlier stage */
	if (tsc_log.output_lock_enabled == TSC_FALSE) {
		return;
	}

	/* If the scheduler is started and in thread context */
	if (__get_IPSR() == 0 && osThreadGetId() != NULL) {
		osMutexAcquire(tsc_log.output_locker, osWaitForever);
	} else {
#ifdef TSC_LOG_USING_ISR_LOG
		tsc_log.output_locker_isr_lvl = hw_interrupt_disable();
#endif
	}
}

void tsc_log_output_lock_enabled(bool enabled) {
	tsc_log.output_lock_enabled = enabled;
}

static char* get_log_buf(void) {
	/* is in thread context */
	if (__get_IPSR() == 0) {
		return tsc_log.log_buf_th;
	} else {
#ifdef TSC_LOG_USING_ISR_LOG
		return tsc_log.log_buf_isr;
#else
        tsc_kprintf("Error: Current mode not supported run in ISR. Please enable TSC_LOG_USING_ISR_LOG.\n");
        return NULL;
#endif
	}
}

__weak size_t tsc_log_formater(char *log_buf, uint32_t level, const char *tag, bool newline, const char *format, va_list args) {
	/* the caller has locker, so it can use static variable for reduce stack usage */
	static size_t log_len, newline_len;
	static int fmt_result;

	ASSERT(log_buf);
	ASSERT(level <= LOG_LVL_DBG);
	ASSERT(tag);
	ASSERT(format);

	log_len = 0;
	newline_len = tsc_strlen(TSC_LOG_NEWLINE_SIGN);

#ifdef TSC_LOG_USING_COLOR
	/* add CSI start sign and color info */
	if (color_output_info[level]) {
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, CSI_START);
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, color_output_info[level]);
	}
#endif /* TSC_LOG_USING_COLOR */

	log_buf[log_len] = '\0';

#ifdef TSC_LOG_OUTPUT_TIME
	/* add time info */
	{
#ifdef TSC_LOG_TIME_USING_TIMESTAMP
		static struct timeval now;
		static struct tm *tm, tm_tmp;
		static bool check_usec_support = TSC_FALSE, usec_is_support = TSC_FALSE;

		if (gettimeofday(&now, NULL) >= 0) {
			time_t t = now.tv_sec;
			tm = localtime_r(&t, &tm_tmp);
			/* show the time format MM-DD HH:MM:SS */
			tsc_snprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE - log_len, "%02d-%02d %02d:%02d:%02d", tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
					tm->tm_min, tm->tm_sec);
			/* check the microseconds support when kernel is startup */
			if (!check_usec_support && osThreadGetId() != NULL) {
				long old_usec = now.tv_usec;
				/* delay some time for wait microseconds changed */
				osDelay(10);
				gettimeofday(&now, NULL);
				check_usec_support = TSC_TRUE;
				/* the microseconds is not equal between two gettimeofday calls */
				if (now.tv_usec != old_usec) usec_is_support = TSC_TRUE;
			}
			if (usec_is_support) {
				/* show the millisecond */
				log_len += tsc_strlen(log_buf + log_len);
				tsc_snprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE - log_len, ".%03d", now.tv_usec / 1000);
			}
		}

#else
        static size_t tick_len = 0;

        log_buf[log_len] = '[';
        tick_len = tsc_log_ultoa(log_buf + log_len + 1, tick_get());
        log_buf[log_len + 1 + tick_len] = ']';
        log_buf[log_len + 1 + tick_len + 1] = '\0';
#endif /* TSC_LOG_TIME_USING_TIMESTAMP */

		log_len += tsc_strlen(log_buf + log_len);
	}
#endif /* TSC_LOG_OUTPUT_TIME */

#ifdef TSC_LOG_OUTPUT_LEVEL

#ifdef TSC_LOG_OUTPUT_TIME
	log_len += tsc_log_strcpy(log_len, log_buf + log_len, " ");
#endif

	/* add level info */
	log_len += tsc_log_strcpy(log_len, log_buf + log_len, level_output_info[level]);
#endif /* TSC_LOG_OUTPUT_LEVEL */

#ifdef TSC_LOG_OUTPUT_TAG

#if !defined(TSC_LOG_OUTPUT_LEVEL) && defined(TSC_LOG_OUTPUT_TIME)
    log_len += tsc_log_strcpy(log_len, log_buf + log_len, " ");
#endif

	/* add tag info */
	log_len += tsc_log_strcpy(log_len, log_buf + log_len, tag);
#endif /* TSC_LOG_OUTPUT_TAG */

#ifdef TSC_LOG_OUTPUT_THREAD_NAME
	/* add thread info */
	{

#if defined(TSC_LOG_OUTPUT_TIME) || defined(TSC_LOG_OUTPUT_LEVEL) || defined(TSC_LOG_OUTPUT_TAG)
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, " ");
#endif

		/* is not in interrupt context */
		if (__get_IPSR() == 0) {
			size_t name_len = 0;
			const char *thread_name = "N/A";
			if (osThreadGetId()) {
				thread_name = osThreadGetName(osThreadGetId());
			}
			name_len = tsc_strnlen(thread_name, TSC_NAME_MAX);
			tsc_strncpy(log_buf + log_len, thread_name, name_len);
			log_len += name_len;
		} else {
			log_len += tsc_log_strcpy(log_len, log_buf + log_len, "ISR");
		}
	}
#endif /* TSC_LOG_OUTPUT_THREAD_NAME */

	log_len += tsc_log_strcpy(log_len, log_buf + log_len, ": ");
	fmt_result = tsc_vsnprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE - log_len, format, args);

	/* calculate log length */
	if ((log_len + fmt_result <= TSC_LOG_LINE_BUF_SIZE) && (fmt_result > -1)) {
		log_len += fmt_result;
	} else {
		/* using max length */
		log_len = TSC_LOG_LINE_BUF_SIZE;
	}

	/* overflow check and reserve some space for CSI end sign and newline sign */
#ifdef TSC_LOG_USING_COLOR
	if (log_len + (sizeof(CSI_END) - 1) + newline_len > TSC_LOG_LINE_BUF_SIZE) {
		/* using max length */
		log_len = TSC_LOG_LINE_BUF_SIZE;
		/* reserve some space for CSI end sign */
		log_len -= (sizeof(CSI_END) - 1);
#else
    if (log_len + newline_len > TSC_LOG_LINE_BUF_SIZE)
    {
        /* using max length */
        log_len = TSC_LOG_LINE_BUF_SIZE;
#endif /* TSC_LOG_USING_COLOR */
		/* reserve some space for newline sign */
		log_len -= newline_len;
	}

	/* package newline sign */
	if (newline) {
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, TSC_LOG_NEWLINE_SIGN);
	}

#ifdef TSC_LOG_USING_COLOR
	/* add CSI end sign  */
	if (color_output_info[level]) {
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, CSI_END);
	}
#endif /* TSC_LOG_USING_COLOR */

	return log_len;
}

void tsc_log_output_to_all_backend(uint32_t level, const char *tag, bool is_raw, const char *log, size_t size) {
	tsc_slist_t *node;
	tsc_log_backend_t backend;

	if (!tsc_log.init_ok) return;

	/* if there is no backend */
	if (!tsc_slist_first(&tsc_log.backend_list)) {
		tsc_kputs(log);
		return;
	}

	/* output for all backends */
	for (node = tsc_slist_first(&tsc_log.backend_list); node; node = tsc_slist_next(node)) {
		backend = tsc_slist_entry(node, struct tsc_log_backend, list);
		if (backend->out_level < level) {
			continue;
		}
#if !defined(TSC_LOG_USING_COLOR) || defined(TSC_LOG_USING_SYSLOG)
        backend->output(backend, level, tag, is_raw, log, size);
#else
		if (backend->filter && backend->filter(backend, level, tag, is_raw, log, size) == TSC_FALSE) {
			/* backend's filter is not match, so skip output */
			continue;
		}
		if (backend->support_color || is_raw) {
			backend->output(backend, level, tag, is_raw, log, size);
		} else {
			/* recalculate the log start address and log size when backend not supported color */
			size_t color_info_len = 0, output_size = size;
			const char *output_log = log;

			if (color_output_info[level] != NULL) color_info_len = tsc_strlen(color_output_info[level]);

			if (color_info_len) {
				size_t color_hdr_len = tsc_strlen(CSI_START) + color_info_len;

				output_log += color_hdr_len;
				output_size -= (color_hdr_len + (sizeof(CSI_END) - 1));
			}
			backend->output(backend, level, tag, is_raw, output_log, output_size);
		}
#endif /* !defined(TSC_LOG_USING_COLOR) || defined(TSC_LOG_USING_SYSLOG) */
	}
}

static void do_output(uint32_t level, const char *tag, bool is_raw, const char *log_buf, size_t log_len) {
#ifdef TSC_LOG_USING_ASYNC_OUTPUT

	if (is_raw == TSC_FALSE) {
		rbb_blk_t log_blk;
		tsc_log_frame_t log_frame;

		/* allocate log frame */
		log_blk = rbb_blk_alloc(tsc_log.async_rbb, TSC_ALIGN(sizeof(struct tsc_log_frame) + log_len, TSC_ALIGN_SIZE));
		if (log_blk) {
			/* package the log frame */
			log_frame = (tsc_log_frame_t) log_blk->buf;
			log_frame->magic = TSC_LOG_FRAME_MAGIC;
			log_frame->is_raw = is_raw;
			log_frame->level = level;
			log_frame->log_len = log_len;
			log_frame->tag = tag;
			log_frame->log = (const char*) log_blk->buf + sizeof(struct tsc_log_frame);
			/* copy log data */
			tsc_memcpy(log_blk->buf + sizeof(struct tsc_log_frame), log_buf, log_len);
			/* put the block */
			rbb_blk_put(log_blk);
			/* send a notice */
			osSemaphoreRelease(tsc_log.async_notice);
		} else {
			static bool already_output = TSC_FALSE;
			if (already_output == TSC_FALSE) {
				tsc_kprintf("Warning: There is no enough buffer for saving async log,"
						" please increase the TSC_LOG_ASYNC_OUTPUT_BUF_SIZE option.\n");
				already_output = TSC_TRUE;
			}
		}
	} else if (tsc_log.async_rb) {
		ringbuffer_put(tsc_log.async_rb, (const uint8_t*) log_buf, log_len);
		/* send a notice */
		osSemaphoreRelease(tsc_log.async_notice);
	}
#else
    /* is in thread context */
    if (__get_IPSR() == 0)
    {
        /* output to all backends */
        tsc_log_output_to_all_backend(level, tag, is_raw, log_buf, log_len);
    }
    else
    {
#ifdef TSC_LOG_BACKEND_USING_CONSOLE
        /* We can't ensure that all backends support ISR context output.
         * So only using tsc_kprintf when context is ISR */
        extern void tsc_log_console_backend_output(struct tsc_log_backend *backend, uint32_t level, const char *tag,
                bool is_raw, const char *log, size_t len);
        tsc_log_console_backend_output(NULL, level, tag, is_raw, log_buf, log_len);
#endif /* TSC_LOG_BACKEND_USING_CONSOLE */
    }
#endif /* TSC_LOG_USING_ASYNC_OUTPUT */
}

/**
 * output the log by variable argument list
 *
 * @param level level
 * @param tag tag
 * @param newline has_newline
 * @param format output format
 * @param args variable argument list
 */
void tsc_log_voutput(uint32_t level, const char *tag, bool newline, const char *format, va_list args) {
	static bool tsc_log_voutput_recursion = TSC_FALSE;
	char *log_buf = NULL;
	size_t log_len = 0;

	ASSERT(tag);
	ASSERT(format);
#ifndef TSC_LOG_USING_SYSLOG
	ASSERT(level <= LOG_LVL_DBG);
#else
    ASSERT(LOG_PRI(level) <= LOG_DEBUG);
#endif /* TSC_LOG_USING_SYSLOG */

	if (!tsc_log.init_ok) {
		return;
	}

#ifdef TSC_LOG_USING_FILTER
	/* level filter */
#ifndef TSC_LOG_USING_SYSLOG
	if (level > tsc_log.filter.level || level > tsc_log_tag_lvl_filter_get(tag)) {
		return;
	}
#else
    if (((LOG_MASK(LOG_PRI(level)) & tsc_log.filter.level) == 0)
            || ((LOG_MASK(LOG_PRI(level)) & tsc_log_tag_lvl_filter_get(tag)) == 0))
    {
        return;
    }
#endif /* TSC_LOG_USING_SYSLOG */

	/* tag filter */
	if (tsc_log.filter.tag[0] != '\0' && !tsc_strstr(tag, tsc_log.filter.tag)) {
		return;
	}
#endif /* TSC_LOG_USING_FILTER */

	/* get log buffer */
	log_buf = get_log_buf();

	/* lock output */
	output_lock();

	/* If there is a recursion, we use a simple way */
	if (tsc_log_voutput_recursion == TSC_TRUE) {
		tsc_kprintf(format, args);
		if (newline == TSC_TRUE) {
			tsc_kprintf(TSC_LOG_NEWLINE_SIGN);
		}
		output_unlock();
		return;
	}

	tsc_log_voutput_recursion = TSC_TRUE;

#ifndef TSC_LOG_USING_SYSLOG
	log_len = tsc_log_formater(log_buf, level, tag, newline, format, args);
#else
    extern size_t syslog_formater(char *log_buf, uint8_t level, const char *tag, bool newline, const char *format, va_list args);
    log_len = syslog_formater(log_buf, level, tag, newline, format, args);
#endif /* TSC_LOG_USING_SYSLOG */

#ifdef TSC_LOG_USING_FILTER
	/* keyword filter */
	if (tsc_log.filter.keyword[0] != '\0') {
		/* add string end sign */
		log_buf[log_len] = '\0';
		/* find the keyword */
		if (!tsc_strstr(log_buf, tsc_log.filter.keyword)) {
			tsc_log_voutput_recursion = TSC_FALSE;
			/* unlock output */
			output_unlock();
			return;
		}
	}
#endif /* TSC_LOG_USING_FILTER */
	/* do log output */
	do_output(level, tag, TSC_FALSE, log_buf, log_len);

	tsc_log_voutput_recursion = TSC_FALSE;

	/* unlock output */
	output_unlock();
}

/**
 * output the log
 *
 * @param level level
 * @param tag tag
 * @param newline has newline
 * @param format output format
 * @param ... args
 */
void tsc_log_output(uint32_t level, const char *tag, bool newline, const char *format, ...) {
	va_list args;

	/* args point to the first variable parameter */
	va_start(args, format);

	tsc_log_voutput(level, tag, newline, format, args);

	va_end(args);
}

/**
 * output RAW string format log
 *
 * @param format output format
 * @param ... args
 */
void tsc_log_raw(const char *format, ...) {
	size_t log_len = 0;
	char *log_buf = NULL;
	va_list args;
	int fmt_result;

	ASSERT(tsc_log.init_ok);

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	if (tsc_log.async_rb == NULL) {
		tsc_log.async_rb = ringbuffer_create(TSC_LOG_ASYNC_OUTPUT_BUF_SIZE);
	}
#endif

	/* get log buffer */
	log_buf = get_log_buf();

	/* lock output */
	output_lock();

	/* args point to the first variable parameter */
	va_start(args, format);
	fmt_result = tsc_vsnprintf(log_buf, TSC_LOG_LINE_BUF_SIZE, format, args);
	va_end(args);

	/* calculate log length */
	if ((fmt_result > -1) && (fmt_result <= TSC_LOG_LINE_BUF_SIZE)) {
		log_len = fmt_result;
	} else {
		log_len = TSC_LOG_LINE_BUF_SIZE;
	}

	/* do log output */
	do_output(LOG_LVL_DBG, NULL, TSC_TRUE, log_buf, log_len);

	/* unlock output */
	output_unlock();
}

/**
 * dump the hex format data to log
 *
 * @param tag name for hex object, it will show on log header
 * @param width hex number for every line, such as: 16, 32
 * @param buf hex buffer
 * @param size buffer size
 */
void tsc_log_hexdump(const char *tag, size_t width, uint8_t *buf, size_t size) {
#define __is_print(ch)       ((unsigned int)((ch) - ' ') < 127u - ' ')

	size_t i, j;
	size_t log_len = 0, name_len = tsc_strlen(tag);
#ifdef TSC_LOG_OUTPUT_TIME
	size_t time_head_len = 0;
#endif
	char *log_buf = NULL, dump_string[8];
	int fmt_result;

	ASSERT(tsc_log.init_ok);

#ifdef TSC_LOG_USING_FILTER
	/* level filter */
#ifndef TSC_LOG_USING_SYSLOG
	if (LOG_LVL_DBG > tsc_log.filter.level || LOG_LVL_DBG > tsc_log_tag_lvl_filter_get(tag)) {
		return;
	}
#else
    if ((LOG_MASK(LOG_DEBUG) & tsc_log.filter.level) == 0)
    {
        return;
    }
#endif /* TSC_LOG_USING_SYSLOG */
	else if (!tsc_strstr(tag, tsc_log.filter.tag)) {
		/* tag filter */
		return;
	}
#endif /* TSC_LOG_USING_FILTER */

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	if (tsc_log.async_rb == NULL) {
		tsc_log.async_rb = ringbuffer_create(TSC_LOG_ASYNC_OUTPUT_BUF_SIZE);
	}
#endif

	/* get log buffer */
	log_buf = get_log_buf();

	/* lock output */
	output_lock();

	for (i = 0, log_len = 0; i < size; i += width) {
		/* package header */
		if (i == 0) {
#ifdef TSC_LOG_OUTPUT_TIME
			/* add time info */
#ifdef TSC_LOG_TIME_USING_TIMESTAMP
			static time_t now;
			static struct tm *tm, tm_tmp;

			now = time(NULL);
			tm = gmtime_r(&now, &tm_tmp);

#ifdef TSC_USING_SOFT_RTC
            tsc_snprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE - log_len, "%02d-%02d %02d:%02d:%02d.%03d ", tm->tm_mon + 1,
                tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tick_get() % 1000);
#else
			tsc_snprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE - log_len, "%02d-%02d %02d:%02d:%02d ", tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
					tm->tm_min, tm->tm_sec);
#endif /* TSC_USING_SOFT_RTC */

#else
            static size_t tick_len = 0;

            log_buf[log_len] = '[';
            tick_len = tsc_log_ultoa(log_buf + log_len + 1, tick_get());
            log_buf[log_len + 1 + tick_len] = ']';
            log_buf[log_len + 2 + tick_len] = ' ';
            log_buf[log_len + 3 + tick_len] = '\0';
#endif /* TSC_LOG_TIME_USING_TIMESTAMP */
			time_head_len = tsc_strlen(log_buf + log_len);
			log_len += time_head_len;
#endif /* TSC_LOG_OUTPUT_TIME */
			log_len += tsc_log_strcpy(log_len, log_buf + log_len, "D/HEX ");
			log_len += tsc_log_strcpy(log_len, log_buf + log_len, tag);
			log_len += tsc_log_strcpy(log_len, log_buf + log_len, ": ");
		} else {
			log_len = 6 + name_len + 2;
#ifdef TSC_LOG_OUTPUT_TIME
			log_len += time_head_len;
#endif
			tsc_memset(log_buf, ' ', log_len);
		}
		fmt_result = tsc_snprintf(log_buf + log_len, TSC_LOG_LINE_BUF_SIZE, "%04X-%04X: ", i, i + width - 1);
		/* calculate log length */
		if ((fmt_result > -1) && (fmt_result <= TSC_LOG_LINE_BUF_SIZE)) {
			log_len += fmt_result;
		} else {
			log_len = TSC_LOG_LINE_BUF_SIZE;
		}
		/* dump hex */
		for (j = 0; j < width; j++) {
			if (i + j < size) {
				tsc_snprintf(dump_string, sizeof(dump_string), "%02X ", buf[i + j]);
			} else {
				tsc_strncpy(dump_string, "   ", sizeof(dump_string));
			}
			log_len += tsc_log_strcpy(log_len, log_buf + log_len, dump_string);
			if ((j + 1) % 8 == 0) {
				log_len += tsc_log_strcpy(log_len, log_buf + log_len, " ");
			}
		}
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, "  ");
		/* dump char for hex */
		for (j = 0; j < width; j++) {
			if (i + j < size) {
				tsc_snprintf(dump_string, sizeof(dump_string), "%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
				log_len += tsc_log_strcpy(log_len, log_buf + log_len, dump_string);
			}
		}
		/* overflow check and reserve some space for newline sign */
		if (log_len + tsc_strlen(TSC_LOG_NEWLINE_SIGN) > TSC_LOG_LINE_BUF_SIZE) {
			log_len = TSC_LOG_LINE_BUF_SIZE - tsc_strlen(TSC_LOG_NEWLINE_SIGN);
		}
		/* package newline sign */
		log_len += tsc_log_strcpy(log_len, log_buf + log_len, TSC_LOG_NEWLINE_SIGN);
		/*add string end sign*/
		log_buf[log_len] = '\0';
		/* do log output */
		do_output(LOG_LVL_DBG, NULL, TSC_TRUE, log_buf, log_len);
	}
	/* unlock output */
	output_unlock();
}

#ifdef TSC_LOG_USING_FILTER
/**
 * Set the filter's level by different backend.
 * The log on this backend which level is less than it will stop output.
 *
 * @param be_name backend name
 * @param level The filter level. When the level is LOG_FILTER_LVL_SILENT, the log enter silent mode.
 *        When the level is LOG_FILTER_LVL_ALL, it will remove this tag's level filer.
 *        Then all level log will resume output.
 *
 * @return  0 : success
 *         -10: level is out of range
 */
int tsc_log_be_lvl_filter_set(const char *be_name, uint32_t level) {
	tsc_slist_t *node = NULL;
	tsc_log_backend_t backend;
	int result = TSC_EOK;

	if (level > LOG_FILTER_LVL_ALL) return -TSC_EINVAL;

	if (!tsc_log.init_ok) return result;

	for (node = tsc_slist_first(&tsc_log.backend_list); node; node = tsc_slist_next(node)) {
		backend = tsc_slist_entry(node, struct tsc_log_backend, list);
		if (tsc_strncmp(backend->name, be_name, TSC_NAME_MAX) == 0) {
			backend->out_level = level;
		}
	}

	return result;
}

/**
 * Set the filter's level by different tag.
 * The log on this tag which level is less than it will stop output.
 *
 * example:
 *     // the example tag log enter silent mode
 *     tsc_log_set_filter_lvl("example", LOG_FILTER_LVL_SILENT);
 *     // the example tag log which level is less than INFO level will stop output
 *     tsc_log_set_filter_lvl("example", LOG_LVL_INFO);
 *     // remove example tag's level filter, all level log will resume output
 *     tsc_log_set_filter_lvl("example", LOG_FILTER_LVL_ALL);
 *
 * @param tag log tag
 * @param level The filter level. When the level is LOG_FILTER_LVL_SILENT, the log enter silent mode.
 *        When the level is LOG_FILTER_LVL_ALL, it will remove this tag's level filer.
 *        Then all level log will resume output.
 *
 * @return  0 : success
 *         -5 : no memory
 *         -10: level is out of range
 */
int tsc_log_tag_lvl_filter_set(const char *tag, uint32_t level) {
	tsc_slist_t *node;
	tsc_log_tag_lvl_filter_t tag_lvl = NULL;
	int result = TSC_EOK;

	if (level > LOG_FILTER_LVL_ALL) return -TSC_EINVAL;

	if (!tsc_log.init_ok) return result;

	/* lock output */
	output_lock();
	/* find the tag in list */
	for (node = tsc_slist_first(tsc_log_tag_lvl_list_get()); node; node = tsc_slist_next(node)) {
		tag_lvl = tsc_slist_entry(node, struct tsc_log_tag_lvl_filter, list);
		if (!tsc_strncmp(tag_lvl->tag, tag, TSC_LOG_FILTER_TAG_MAX_LEN)) {
			break;
		} else {
			tag_lvl = NULL;
		}
	}
	/* find OK */
	if (tag_lvl) {
		if (level == LOG_FILTER_LVL_ALL) {
			/* remove current tag's level filter when input level is the lowest level */
			tsc_slist_remove(tsc_log_tag_lvl_list_get(), &tag_lvl->list);
			tsc_free(tag_lvl);
		} else {
			/* update level */
			tag_lvl->level = level;
		}
	} else {
		/* only add the new tag's level filer when level is not LOG_FILTER_LVL_ALL */
		if (level != LOG_FILTER_LVL_ALL) {
			/* new a tag's level filter */
			tag_lvl = (tsc_log_tag_lvl_filter_t) tsc_malloc(sizeof(struct tsc_log_tag_lvl_filter));
			if (tag_lvl) {
				tsc_memset(tag_lvl->tag, 0, sizeof(tag_lvl->tag));
				tsc_strncpy(tag_lvl->tag, tag, TSC_LOG_FILTER_TAG_MAX_LEN);
				tag_lvl->level = level;
				tsc_slist_append(tsc_log_tag_lvl_list_get(), &tag_lvl->list);
			} else {
				result = -TSC_ENOMEM;
			}
		}
	}
	/* unlock output */
	output_unlock();

	return result;
}

/**
 * get the level on tag's level filer
 *
 * @param tag log tag
 *
 * @return It will return the lowest level when tag was not found.
 *         Other level will return when tag was found.
 */
uint32_t tsc_log_tag_lvl_filter_get(const char *tag) {
	tsc_slist_t *node;
	tsc_log_tag_lvl_filter_t tag_lvl = NULL;
	uint32_t level = LOG_FILTER_LVL_ALL;

	if (!tsc_log.init_ok) return level;

	/* lock output */
	output_lock();
	/* find the tag in list */
	for (node = tsc_slist_first(tsc_log_tag_lvl_list_get()); node; node = tsc_slist_next(node)) {
		tag_lvl = tsc_slist_entry(node, struct tsc_log_tag_lvl_filter, list);
		if (!tsc_strncmp(tag_lvl->tag, tag, TSC_LOG_FILTER_TAG_MAX_LEN)) {
			level = tag_lvl->level;
			break;
		}
	}
	/* unlock output */
	output_unlock();

	return level;
}

/**
 * get the tag's level list on filter
 *
 * @return tag's level list
 */
tsc_slist_t* tsc_log_tag_lvl_list_get(void) {
	return &tsc_log.filter.tag_lvl_list;
}

/**
 * set log global filter level
 *
 * @param level log level: LOG_LVL_ASSERT, LOG_LVL_ERROR, LOG_LVL_WARNING, LOG_LVL_INFO, LOG_LVL_DBG
 *              LOG_FILTER_LVL_SILENT: disable all log output, except assert level
 *              LOG_FILTER_LVL_ALL: enable all log output
 */
void tsc_log_global_filter_lvl_set(uint32_t level) {
	ASSERT(level <= LOG_FILTER_LVL_ALL);

	tsc_log.filter.level = level;
}

/**
 * get log global filter level
 *
 * @return log level: LOG_LVL_ASSERT, LOG_LVL_ERROR, LOG_LVL_WARNING, LOG_LVL_INFO, LOG_LVL_DBG
 *              LOG_FILTER_LVL_SILENT: disable all log output, except assert level
 *              LOG_FILTER_LVL_ALL: enable all log output
 */
uint32_t tsc_log_global_filter_lvl_get(void) {
	return tsc_log.filter.level;
}

/**
 * set log global filter tag
 *
 * @param tag tag
 */
void tsc_log_global_filter_tag_set(const char *tag) {
	ASSERT(tag);

	tsc_strncpy(tsc_log.filter.tag, tag, TSC_LOG_FILTER_TAG_MAX_LEN);
}

/**
 * get log global filter tag
 *
 * @return tag
 */
const char* tsc_log_global_filter_tag_get(void) {
	return tsc_log.filter.tag;
}

/**
 * set log global filter keyword
 *
 * @param keyword keyword
 */
void tsc_log_global_filter_kw_set(const char *keyword) {
	ASSERT(keyword);

	tsc_strncpy(tsc_log.filter.keyword, keyword, TSC_LOG_FILTER_KW_MAX_LEN);
}

/**
 * get log global filter keyword
 *
 * @return keyword
 */
const char* tsc_log_global_filter_kw_get(void) {
	return tsc_log.filter.keyword;
}

#ifdef TSC_USING_FINSH
#include <finsh.h>

static void _print_lvl_info(void) {
#ifndef TSC_LOG_USING_SYSLOG
	tsc_kprintf("Assert  : 0\n");
	tsc_kprintf("Error   : 3\n");
	tsc_kprintf("Warning : 4\n");
	tsc_kprintf("Info    : 6\n");
	tsc_kprintf("Debug   : 7\n");
#else
    tsc_kprintf("EMERG   :   1 (1 << 0)\n");
    tsc_kprintf("ALERT   :   2 (1 << 1)\n");
    tsc_kprintf("CRIT    :   4 (1 << 2)\n");
    tsc_kprintf("ERR     :   8 (1 << 3)\n");
    tsc_kprintf("WARNING :  16 (1 << 4)\n");
    tsc_kprintf("NOTICE  :  32 (1 << 5)\n");
    tsc_kprintf("INFO    :  64 (1 << 6)\n");
    tsc_kprintf("DEBUG   : 128 (1 << 7)\n");
#endif /* TSC_LOG_USING_SYSLOG */
}

static void tsc_log_be_lvl(uint8_t argc, char **argv) {
	if (argc > 2) {
		if ((atoi(argv[2]) <= LOG_FILTER_LVL_ALL) && (atoi(argv[2]) >= 0)) {
			tsc_log_be_lvl_filter_set(argv[1], atoi(argv[2]));
		} else {
			tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		}
	} else {
		tsc_kprintf("Please input: tsc_log_be_lvl <be_name> <level>.\n");
		_print_lvl_info();
	}
}
TSC_CMD_EXPORT(tsc_log_be_lvl, Set tsc_log filter level by different backend.);

static void tsc_log_tag_lvl(uint8_t argc, char **argv) {
	if (argc > 2) {
		if ((atoi(argv[2]) <= LOG_FILTER_LVL_ALL) && (atoi(argv[2]) >= 0)) {
			tsc_log_tag_lvl_filter_set(argv[1], atoi(argv[2]));
		} else {
			tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		}
	} else {
		tsc_kprintf("Please input: tsc_log_tag_lvl <tag> <level>.\n");
		_print_lvl_info();
	}
}
TSC_CMD_EXPORT(tsc_log_tag_lvl, Set tsc_log filter level by different tag.);

static void tsc_log_lvl(uint8_t argc, char **argv) {
	if (argc > 1) {
		if ((atoi(argv[1]) <= LOG_FILTER_LVL_ALL) && (atoi(argv[1]) >= 0)) {
			tsc_log_global_filter_lvl_set(atoi(argv[1]));
		} else {
			tsc_kprintf("Please input correct level (0-%d).\n", LOG_FILTER_LVL_ALL);
		}
	} else {
		tsc_kprintf("Please input: tsc_log_lvl <level>.\n");
		_print_lvl_info();
	}
}
TSC_CMD_EXPORT(tsc_log_lvl, Set tsc_log global filter level.);

static void tsc_log_tag(uint8_t argc, char **argv) {
	if (argc > 1) {
		if (tsc_strlen(argv[1]) <= TSC_LOG_FILTER_TAG_MAX_LEN) {
			tsc_log_global_filter_tag_set(argv[1]);
		} else {
			tsc_kprintf("The tag length is too long. Max is %d.\n", TSC_LOG_FILTER_TAG_MAX_LEN);
		}
	} else {
		tsc_log_global_filter_tag_set("");
	}
}
TSC_CMD_EXPORT(tsc_log_tag, Set tsc_log global filter tag);

static void tsc_log_kw(uint8_t argc, char **argv) {
	if (argc > 1) {
		if (tsc_strlen(argv[1]) <= TSC_LOG_FILTER_KW_MAX_LEN) {
			tsc_log_global_filter_kw_set(argv[1]);
		} else {
			tsc_kprintf("The keyword length is too long. Max is %d.\n", TSC_LOG_FILTER_KW_MAX_LEN);
		}
	} else {
		tsc_log_global_filter_kw_set("");
	}
}
TSC_CMD_EXPORT(tsc_log_kw, Set tsc_log global filter keyword);

static void tsc_log_filter(uint8_t argc, char **argv) {
#ifndef TSC_LOG_USING_SYSLOG
	const char *lvl_name[] = { "Assert ", "Error  ", "Error  ", "Error  ", "Warning", "Info   ", "Info   ", "Debug  " };
#endif
	const char *tag = tsc_log_global_filter_tag_get(), *kw = tsc_log_global_filter_kw_get();
	tsc_slist_t *node;
	tsc_log_tag_lvl_filter_t tag_lvl = NULL;

	tsc_kprintf("--------------------------------------\n");
	tsc_kprintf("tsc_log global filter:\n");

#ifndef TSC_LOG_USING_SYSLOG
	tsc_kprintf("level   : %s\n", lvl_name[tsc_log_global_filter_lvl_get()]);
#else
    tsc_kprintf("level   : %d\n", tsc_log_global_filter_lvl_get());
#endif

	tsc_kprintf("tag     : %s\n", tsc_strlen(tag) == 0 ? "NULL" : tag);
	tsc_kprintf("keyword : %s\n", tsc_strlen(kw) == 0 ? "NULL" : kw);

	tsc_kprintf("--------------------------------------\n");
	tsc_kprintf("tsc_log tag's level filter:\n");
	if (tsc_slist_isempty(tsc_log_tag_lvl_list_get())) {
		tsc_kprintf("settings not found\n");
	} else {
		/* lock output */
		output_lock();
		/* show the tag level list */
		for (node = tsc_slist_first(tsc_log_tag_lvl_list_get()); node; node = tsc_slist_next(node)) {
			tag_lvl = tsc_slist_entry(node, struct tsc_log_tag_lvl_filter, list);
			tsc_kprintf("%-*.s: ", TSC_LOG_FILTER_TAG_MAX_LEN, tag_lvl->tag);

#ifndef TSC_LOG_USING_SYSLOG
			tsc_kprintf("%s\n", lvl_name[tag_lvl->level]);
#else
            tsc_kprintf("%d\n", tag_lvl->level);
#endif

		}
		/* unlock output */
		output_unlock();
	}
}
TSC_CMD_EXPORT(tsc_log_filter, Show tsc_log filter settings);
#endif /* TSC_USING_FINSH */
#endif /* TSC_LOG_USING_FILTER */

int tsc_log_backend_register(tsc_log_backend_t backend, const char *name, bool support_color) {
	long level;

	ASSERT(backend);
	ASSERT(name);
	ASSERT(tsc_log.init_ok);
	ASSERT(backend->output);

	if (backend->init) {
		backend->init(backend);
	}

	backend->support_color = support_color;
	backend->out_level = LOG_FILTER_LVL_ALL;
	tsc_strncpy(backend->name, name, TSC_NAME_MAX);

	level = hw_interrupt_disable();
	tsc_slist_append(&tsc_log.backend_list, &backend->list);
	hw_interrupt_enable(level);

	return TSC_EOK;
}

int tsc_log_backend_unregister(tsc_log_backend_t backend) {
	long level;

	ASSERT(backend);
	ASSERT(tsc_log.init_ok);

	if (backend->deinit) {
		backend->deinit(backend);
	}

	level = hw_interrupt_disable();
	tsc_slist_remove(&tsc_log.backend_list, &backend->list);
	hw_interrupt_enable(level);

	return TSC_EOK;
}

int tsc_log_backend_set_filter(tsc_log_backend_t backend, tsc_log_backend_filter_t filter) {
	long level;
	ASSERT(backend);

	level = hw_interrupt_disable();
	backend->filter = filter;
	hw_interrupt_enable(level);

	return TSC_EOK;
}

tsc_log_backend_t tsc_log_backend_find(const char *name) {
	long level;
	tsc_slist_t *node;
	tsc_log_backend_t backend;

	ASSERT(tsc_log.init_ok);

	level = hw_interrupt_disable();
	for (node = tsc_slist_first(&tsc_log.backend_list); node; node = tsc_slist_next(node)) {
		backend = tsc_slist_entry(node, struct tsc_log_backend, list);
		if (tsc_strncmp(backend->name, name, TSC_NAME_MAX) == 0) {
			hw_interrupt_enable(level);
			return backend;
		}
	}

	hw_interrupt_enable(level);
	return NULL;
}

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
/**
 * asynchronous output logs to all backends
 *
 * @note you must call this function when TSC_LOG_ASYNC_OUTPUT_BY_THREAD is disable
 */
void tsc_log_async_output(void) {
	rbb_blk_t log_blk;
	tsc_log_frame_t log_frame;

	if (!tsc_log.async_enabled) {
		return;
	}

	while ((log_blk = rbb_blk_get(tsc_log.async_rbb)) != NULL) {
		log_frame = (tsc_log_frame_t) log_blk->buf;
		if (log_frame->magic == TSC_LOG_FRAME_MAGIC) {
			/* output to all backends */
			tsc_log_output_to_all_backend(log_frame->level, log_frame->tag, log_frame->is_raw, log_frame->log, log_frame->log_len);
		}
		rbb_blk_free(tsc_log.async_rbb, log_blk);
	}
	/* output the log_raw format log */
	if (tsc_log.async_rb) {
		size_t log_len = ringbuffer_data_len(tsc_log.async_rb);
		char *log = tsc_malloc(log_len);
		if (log) {
			size_t len = ringbuffer_get(tsc_log.async_rb, (uint8_t*) log, log_len);
			tsc_log_output_to_all_backend(LOG_LVL_DBG, NULL, TSC_TRUE, log, len);
			tsc_free(log);
		}
	}
}

/**
 * enable or disable asynchronous output mode
 * the log will be output directly when mode is disabled
 *
 * @param enabled TSC_TRUE: enabled, TSC_FALSE: disabled
 */
void tsc_log_async_output_enabled(bool enabled) {
	tsc_log.async_enabled = enabled;
}

/**
 * waiting for get asynchronous output log
 *
 * @param time the waiting time
 */
void tsc_log_async_waiting_log(int32_t time) {
    /* Clear any pending notifications without deleting the semaphore */
    while (osSemaphoreAcquire(tsc_log.async_notice, 0) == osOK);

    /* Wait for the next notification with the specified timeout */
    osSemaphoreAcquire(tsc_log.async_notice, time);
}

static void async_output_thread_entry(void *param) {
	/* Wait for other initializations to complete (e.g. FATFS mount) */
	osDelay(1000);

	tsc_log_async_output();

	while (1) {
		tsc_log_async_waiting_log(osWaitForever);
		tsc_log_async_output();
	}
}
#endif /* TSC_LOG_USING_ASYNC_OUTPUT */

/**
 * flush all backends's log
 */
void tsc_log_flush(void) {
	tsc_slist_t *node;
	tsc_log_backend_t backend;

	if (!tsc_log.init_ok) return;

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	tsc_log_async_output();
#endif

	/* flush all backends */
	for (node = tsc_slist_first(&tsc_log.backend_list); node; node = tsc_slist_next(node)) {
		backend = tsc_slist_entry(node, struct tsc_log_backend, list);
		if (backend->flush) {
			backend->flush(backend);
		}
	}
}

int tsc_log_init(void) {
	if (tsc_log.init_ok) return 0;

	const osMutexAttr_t output_locker_attr = { .name = "tsc_log",           		// Name (optional)
			.attr_bits = osMutexPrioInherit, 	// Priority inheritance
			.cb_mem = NULL,             		// No custom control block memory
			.cb_size = 0 };
	tsc_log.output_locker = osMutexNew(&output_locker_attr);
	tsc_log.output_lock_enabled = TSC_TRUE;
	tsc_slist_init(&tsc_log.backend_list);

#ifdef TSC_LOG_USING_FILTER
	tsc_slist_init(tsc_log_tag_lvl_list_get());
#endif

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	ASSERT(TSC_LOG_ASYNC_OUTPUT_STORE_LINES >= 2);
	tsc_log.async_enabled = TSC_TRUE;
	/* async output ring block buffer */
	tsc_log.async_rbb = rbb_create(TSC_ALIGN(TSC_LOG_ASYNC_OUTPUT_BUF_SIZE, TSC_ALIGN_SIZE), TSC_LOG_ASYNC_OUTPUT_STORE_LINES);
	if (tsc_log.async_rbb == NULL) {
		tsc_kprintf("Error: tsc_log init failed! No memory for async rbb.\n");
		osMutexDelete(tsc_log.output_locker);
		return -TSC_ENOMEM;
	}

	const osSemaphoreAttr_t tsc_log_sem_attr = { .name = "tsc_log" };

	tsc_log.async_notice = osSemaphoreNew(1, 0, &tsc_log_sem_attr);
#endif /* TSC_LOG_USING_ASYNC_OUTPUT */

#ifdef TSC_LOG_USING_FILTER
	tsc_log_global_filter_lvl_set(LOG_FILTER_LVL_ALL);
#endif

	tsc_log.init_ok = TSC_TRUE;

	return 0;
}
//INIT_BOARD_EXPORT(tsc_log_init);

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
int tsc_log_async_init(void) {
	if (tsc_log.async_th == NULL) {
		// Thread attributes
		const osThreadAttr_t tsc_log_async_attr = { .name = "tsc_log_async", .priority = (osPriority_t) TSC_LOG_ASYNC_OUTPUT_THREAD_PRIORITY,
				.stack_size = TSC_LOG_ASYNC_OUTPUT_THREAD_STACK
		// .attr_bits  = 0,  // Optional
		// .cb_mem / .cb_size for static control block if needed
				};

		// Create and start thread
		tsc_log.async_th = osThreadNew(async_output_thread_entry, &tsc_log, &tsc_log_async_attr);
		if (tsc_log.async_th == NULL) {
			tsc_kprintf("Error: tsc_log init failed! No memory for async output thread.\n");
			return -1; // CMSIS returns NULL on failure, so use generic error code
		}
	}
	return 0;
}
//INIT_PREV_EXPORT(tsc_log_async_init);
#endif /* TSC_LOG_USING_ASYNC_OUTPUT */

void tsc_log_deinit(void) {
	tsc_slist_t *node;
	tsc_log_backend_t backend;

	if (!tsc_log.init_ok) return;

	/* deinit all backends */
	for (node = tsc_slist_first(&tsc_log.backend_list); node; node = tsc_slist_next(node)) {
		backend = tsc_slist_entry(node, struct tsc_log_backend, list);
		if (backend->deinit) {
			backend->deinit(backend);
		}
	}

#ifdef TSC_LOG_USING_FILTER
	/* deinit tag's level filter */
	{
		tsc_log_tag_lvl_filter_t tag_lvl;
		for (node = tsc_slist_first(tsc_log_tag_lvl_list_get()); node; node = tsc_slist_next(node)) {
			tag_lvl = tsc_slist_entry(node, struct tsc_log_tag_lvl_filter, list);
			tsc_free(tag_lvl);
		}
	}
#endif /* TSC_LOG_USING_FILTER */

	osMutexDelete(tsc_log.output_locker);

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
	rbb_destroy(tsc_log.async_rbb);
	osThreadTerminate(tsc_log.async_th);
	if (tsc_log.async_rb) ringbuffer_destroy(tsc_log.async_rb);
#endif

	tsc_log.init_ok = TSC_FALSE;
}

#endif /* CSP_ENABLE_LOG */
