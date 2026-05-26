#ifndef _TSC_LOG_H_
#define _TSC_LOG_H_

#include <kservice.h>
#include <tsc_log_def.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "app_modules_config.h"

#if CSP_ENABLE_LOG

/*
 * tsc_log init and deint
 */
int tsc_log_init(void);
int tsc_log_async_init(void);
void tsc_log_output_lock_enabled(bool enabled);
void tsc_log_deinit(void);

/*
 * output different level log by LOG_X API
 *
 * NOTE: The `LOG_TAG` and `LOG_LVL` must be defined before including the <tsc_log.h> when you want to use LOG_X API.
 *
 * #define LOG_TAG              "example"
 * #define LOG_LVL              LOG_LVL_DBG
 * #include <tsc_log.h>
 *
 * Then you can using LOG_X API to output log
 *
 * LOG_D("this is a debug log!");
 * LOG_E("this is a error log!");
 */
#define LOG_E(...)                      tsc_log_e(LOG_TAG, __VA_ARGS__)
#define LOG_W(...)                      tsc_log_w(LOG_TAG, __VA_ARGS__)
#define LOG_I(...)                      tsc_log_i(LOG_TAG, __VA_ARGS__)
#define LOG_D(...)                      tsc_log_d(LOG_TAG, __VA_ARGS__)
#define LOG_RAW(...)                    tsc_log_raw(__VA_ARGS__)
#define LOG_HEX(name, width, buf, size) tsc_log_hex(name, width, buf, size)

/*
 * backend register and unregister
 */
int tsc_log_backend_register(tsc_log_backend_t backend, const char *name, bool support_color);
int tsc_log_backend_unregister(tsc_log_backend_t backend);
int tsc_log_backend_set_filter(tsc_log_backend_t backend, tsc_log_backend_filter_t filter);
tsc_log_backend_t tsc_log_backend_find(const char *name);

#ifdef TSC_LOG_USING_FILTER
/*
 * log filter setting
 */
int tsc_log_tag_lvl_filter_set(const char *tag, uint32_t level);
uint32_t tsc_log_tag_lvl_filter_get(const char *tag);
tsc_slist_t *tsc_log_tag_lvl_list_get(void);
int tsc_log_be_lvl_filter_set(const char *be_name, uint32_t level);
void tsc_log_global_filter_lvl_set(uint32_t level);
uint32_t tsc_log_global_filter_lvl_get(void);
void tsc_log_global_filter_tag_set(const char *tag);
const char *tsc_log_global_filter_tag_get(void);
void tsc_log_global_filter_kw_set(const char *keyword);
const char *tsc_log_global_filter_kw_get(void);
#endif /* TSC_LOG_USING_FILTER */

/*
 * flush all backends's log
 */
void tsc_log_flush(void);

#ifdef TSC_LOG_USING_ASYNC_OUTPUT
/*
 * asynchronous output API
 */
void tsc_log_async_output(void);
void tsc_log_async_output_enabled(bool enabled);
void tsc_log_async_waiting_log(int32_t time);
#endif

/*
 * dump the hex format data to log
 */
void tsc_log_hexdump(const char *tag, size_t width, uint8_t *buf, size_t size);

/*
 * Another log output API. This API is more difficult to use than LOG_X API.
 */
void tsc_log_voutput(uint32_t level, const char *tag, bool newline, const char *format, va_list args);
void tsc_log_output(uint32_t level, const char *tag, bool newline, const char *format, ...);
void tsc_log_raw(const char *format, ...);

#else /* CSP_ENABLE_LOG */

/* Stubs when logging is disabled */
#define LOG_E(...)
#define LOG_W(...)
#define LOG_I(...)
#define LOG_D(...)
#define LOG_RAW(...)
#define LOG_HEX(name, width, buf, size)

static inline int tsc_log_init(void) { return 0; }
static inline int tsc_log_async_init(void) { return 0; }
static inline void tsc_log_output_lock_enabled(bool enabled) { (void)enabled; }
static inline void tsc_log_deinit(void) {}

static inline int tsc_log_backend_register(tsc_log_backend_t backend, const char *name, bool support_color) { (void)backend; (void)name; (void)support_color; return 0; }
static inline int tsc_log_backend_unregister(tsc_log_backend_t backend) { (void)backend; return 0; }
static inline int tsc_log_backend_set_filter(tsc_log_backend_t backend, tsc_log_backend_filter_t filter) { (void)backend; (void)filter; return 0; }
static inline tsc_log_backend_t tsc_log_backend_find(const char *name) { (void)name; return NULL; }

static inline void tsc_log_flush(void) {}

static inline void tsc_log_hexdump(const char *tag, size_t width, uint8_t *buf, size_t size) { (void)tag; (void)width; (void)buf; (void)size; }

static inline void tsc_log_voutput(uint32_t level, const char *tag, bool newline, const char *format, va_list args) { (void)level; (void)tag; (void)newline; (void)format; (void)args; }
static inline void tsc_log_output(uint32_t level, const char *tag, bool newline, const char *format, ...) { (void)level; (void)tag; (void)newline; (void)format; }
static inline void tsc_log_raw(const char *format, ...) { (void)format; }

#endif /* CSP_ENABLE_LOG */

#ifdef __cplusplus
}
#endif

#endif /* _TSC_LOG_H_ */
