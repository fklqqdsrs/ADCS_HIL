#ifndef _TSC_LOG_DEF_H_
#define _TSC_LOG_DEF_H_

#include <ksdef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* logger level, the number is compatible for syslog */
#define LOG_LVL_ASSERT                 0
#define LOG_LVL_ERROR                  3
#define LOG_LVL_WARNING                4
#define LOG_LVL_INFO                   6
#define LOG_LVL_DBG                    7

/* the output silent level and all level for filter setting */
#ifndef TSC_LOG_USING_SYSLOG
#define LOG_FILTER_LVL_SILENT         0
#define LOG_FILTER_LVL_ALL            7
#else
#define LOG_FILTER_LVL_SILENT         1
#define LOG_FILTER_LVL_ALL            255
#endif /* TSC_LOG_USING_SYSLOG */

/* compatible for rtdbg */
#undef LOG_D
#undef LOG_I
#undef LOG_W
#undef LOG_E
#undef LOG_RAW
#undef DBG_ERROR
#undef DBG_WARNING
#undef DBG_INFO
#undef DBG_LOG
#undef dbg_log
#define DBG_ERROR                      LOG_LVL_ERROR
#define DBG_WARNING                    LOG_LVL_WARNING
#define DBG_INFO                       LOG_LVL_INFO
#define DBG_LOG                        LOG_LVL_DBG
#define dbg_log(level, ...)                                \
    if ((level) <= LOG_LVL)                                \
    {                                                      \
        tsc_log_output(level, LOG_TAG, RT_FALSE, __VA_ARGS__);\
    }

#if !defined(LOG_TAG)
    /* compatible for rtdbg */
    #if defined(DBG_TAG)
        #define LOG_TAG                DBG_TAG
    #elif defined(DBG_SECTION_NAME)
        #define LOG_TAG                DBG_SECTION_NAME
    #else
        #define LOG_TAG                "NO_TAG"
    #endif
#endif /* !defined(LOG_TAG) */

#if !defined(LOG_LVL)
    /* compatible for rtdbg */
    #if defined(DBG_LVL)
        #define LOG_LVL                DBG_LVL
    #elif defined(DBG_LEVEL)
        #define LOG_LVL                DBG_LEVEL
    #else
        #define LOG_LVL                LOG_LVL_DBG
    #endif
#endif /* !defined(LOG_LVL) */

#if (LOG_LVL >= LOG_LVL_DBG) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_DBG)
    #define tsc_log_d(TAG, ...)           tsc_log_output(LOG_LVL_DBG, TAG, TSC_TRUE, __VA_ARGS__)
#else
    #define tsc_log_d(TAG, ...)
#endif /* (LOG_LVL >= LOG_LVL_DBG) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_DBG) */

#if (LOG_LVL >= LOG_LVL_INFO) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_INFO)
    #define tsc_log_i(TAG, ...)           tsc_log_output(LOG_LVL_INFO, TAG, TSC_TRUE, __VA_ARGS__)
#else
    #define tsc_log_i(TAG, ...)
#endif /* (LOG_LVL >= LOG_LVL_INFO) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_INFO) */

#if (LOG_LVL >= LOG_LVL_WARNING) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_WARNING)
    #define tsc_log_w(TAG, ...)           tsc_log_output(LOG_LVL_WARNING, TAG, TSC_TRUE, __VA_ARGS__)
#else
    #define tsc_log_w(TAG, ...)
#endif /* (LOG_LVL >= LOG_LVL_WARNING) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_WARNING) */

#if (LOG_LVL >= LOG_LVL_ERROR) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_ERROR)
    #define tsc_log_e(TAG, ...)           tsc_log_output(LOG_LVL_ERROR, TAG, TSC_TRUE, __VA_ARGS__)
#else
    #define tsc_log_e(TAG, ...)
#endif /* (LOG_LVL >= LOG_LVL_ERROR) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_ERROR) */

#if (LOG_LVL >= LOG_LVL_DBG) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_DBG)
    #define tsc_log_hex(TAG, width, buf, size)     tsc_log_hexdump(TAG, width, buf, size)
#else
    #define tsc_log_hex(TAG, width, buf, size)
#endif /* (LOG_LVL >= LOG_LVL_DBG) && (TSC_LOG_OUTPUT_LVL >= LOG_LVL_DBG) */

/* assert for developer. */
#ifdef TSC_LOG_ASSERT_ENABLE
    #define TSC_LOG_ASSERT(EXPR)                                                 \
    if (!(EXPR))                                                              \
    {                                                                         \
        tsc_log_output(LOG_LVL_ASSERT, LOG_TAG, TSC_TRUE, "(%s) has assert failed at %s:%ld.", #EXPR, __FUNCTION__, __LINE__); \
        tsc_log_flush();                                                         \
        while (1);                                                            \
    }
#else
    #define TSC_LOG_ASSERT(EXPR)
#endif

/* ASSERT API definition */
#if !defined(ASSERT)
    #define ASSERT           TSC_LOG_ASSERT
#endif

/* compatible for elog */
#undef assert
#undef log_e
#undef log_w
#undef log_i
#undef log_d
#undef log_v
#undef ELOG_LVL_ASSERT
#undef ELOG_LVL_ERROR
#undef ELOG_LVL_WARN
#undef ELOG_LVL_INFO
#undef ELOG_LVL_DEBUG
#undef ELOG_LVL_VERBOSE
#define assert                         ASSERT
#define log_e                          LOG_E
#define log_w                          LOG_W
#define log_i                          LOG_I
#define log_d                          LOG_D
#define log_v                          LOG_D
#define log_raw                        LOG_RAW
#define log_hex                        LOG_HEX
#define ELOG_LVL_ASSERT                LOG_LVL_ASSERT
#define ELOG_LVL_ERROR                 LOG_LVL_ERROR
#define ELOG_LVL_WARN                  LOG_LVL_WARNING
#define ELOG_LVL_INFO                  LOG_LVL_INFO
#define ELOG_LVL_DEBUG                 LOG_LVL_DBG
#define ELOG_LVL_VERBOSE               LOG_LVL_DBG

/* setting static output log level */
#ifndef TSC_LOG_OUTPUT_LVL
#define TSC_LOG_OUTPUT_LVL                LOG_LVL_DBG
#endif

/* buffer size for every line's log */
#ifndef TSC_LOG_LINE_BUF_SIZE
#define TSC_LOG_LINE_BUF_SIZE             128
#endif

/* output filter's tag max length */
#ifndef TSC_LOG_FILTER_TAG_MAX_LEN
#define TSC_LOG_FILTER_TAG_MAX_LEN        23
#endif

/* output filter's keyword max length */
#ifndef TSC_LOG_FILTER_KW_MAX_LEN
#define TSC_LOG_FILTER_KW_MAX_LEN         15
#endif

#ifndef TSC_LOG_NEWLINE_SIGN
#define TSC_LOG_NEWLINE_SIGN              "\r\n"
#endif

#define TSC_LOG_FRAME_MAGIC               0x10

/* tag's level filter */
struct tsc_log_tag_lvl_filter
{
    char tag[TSC_LOG_FILTER_TAG_MAX_LEN + 1];
    uint32_t level;
    tsc_slist_t list;
};
typedef struct tsc_log_tag_lvl_filter *tsc_log_tag_lvl_filter_t;

struct tsc_log_frame
{
    /* magic word is 0x10 ('lo') */
    uint32_t magic:8;
    uint32_t is_raw:1;
    uint32_t log_len:23;
    uint32_t level;
    const char *log;
    const char *tag;
};
typedef struct tsc_log_frame *tsc_log_frame_t;

struct tsc_log_backend
{
    char name[TSC_NAME_MAX];
    bool support_color;
    uint32_t out_level;
    void (*init)  (struct tsc_log_backend *backend);
    void (*output)(struct tsc_log_backend *backend, uint32_t level, const char *tag, bool is_raw, const char *log, size_t len);
    void (*flush) (struct tsc_log_backend *backend);
    void (*deinit)(struct tsc_log_backend *backend);
    /* The filter will be call before output. It will return TRUE when the filter condition is math. */
    bool (*filter)(struct tsc_log_backend *backend, uint32_t level, const char *tag, bool is_raw, const char *log, size_t len);
    tsc_slist_t list;
};
typedef struct tsc_log_backend *tsc_log_backend_t;
typedef bool (*tsc_log_backend_filter_t)(struct tsc_log_backend *backend, uint32_t level, const char *tag, bool is_raw, const char *log, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _TSC_LOG_DEF_H_ */
