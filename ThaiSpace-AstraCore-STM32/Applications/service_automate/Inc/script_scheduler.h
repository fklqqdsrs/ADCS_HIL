#ifndef SCRIPT_SCHEDULER_H
#define SCRIPT_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ===== Config (override via -D or before include) ===== */
#ifndef SCRIPT_MAX_SCHED
#define SCRIPT_MAX_SCHED               (8u)
#endif

#ifndef SCRIPT_SCHED_FNAME_MAX
#define SCRIPT_SCHED_FNAME_MAX         (128u)
#endif

#ifndef SCRIPT_MAX_INCLUDE_DEPTH
#define SCRIPT_MAX_INCLUDE_DEPTH       (4u)
#endif

#ifndef SCRIPT_MAX_LINES
#define SCRIPT_MAX_LINES               (0u) /* 0 = unlimited */
#endif

#ifndef SCRIPT_MAX_RUNTIME_MS
#define SCRIPT_MAX_RUNTIME_MS          (0u) /* 0 = disabled */
#endif

#ifndef SCHED_SAVE_MIN_INTERVAL_MS
#define SCHED_SAVE_MIN_INTERVAL_MS     (3000u)
#endif

#ifndef SCRIPT_SCHED_THREAD_STACK_SIZE
#define SCRIPT_SCHED_THREAD_STACK_SIZE (4096u)
#endif

#ifndef SCRIPT_SCHED_TICK_MS
#define SCRIPT_SCHED_TICK_MS           (1000u)
#endif

#ifndef SCRIPT_SCHED_FILENAME
#define SCRIPT_SCHED_FILENAME          "sched.txt"
#endif

#ifndef SCRIPT_SCHED_FILENAME_TMP
#define SCRIPT_SCHED_FILENAME_TMP      "sched.tmp"
#endif

#ifndef SCRIPT_SCHED_LOG_ON_BOOT
#define SCRIPT_SCHED_LOG_ON_BOOT 1  
#endif

/**
 * @brief Kind of schedule.
 */
typedef enum {
	SCHED_KIND_NONE = 0, SCHED_KIND_ABS_UTC, SCHED_KIND_REL_MS, SCHED_KIND_DAILY_HHMM, SCHED_KIND_PERIODIC_MS
} sched_kind_t;

/**
 * @brief Policy for missed executions.
 */
typedef enum {
	MISSED_SKIP = 0, MISSED_CATCH_UP_ONCE
} missed_policy_t;

typedef struct {
	bool used;
	bool enabled;
	sched_kind_t kind;
	missed_policy_t policy;
	uint16_t remain; /* 0 = infinite */
	char file[SCRIPT_SCHED_FNAME_MAX];
	union {
		struct {
			uint32_t utc_s;
		} abs_at;
		struct {
			uint32_t due_ms;
		} rel_at;
		struct {
			uint8_t hh;
			uint8_t mm;
			uint16_t last_ymd;
		} daily;
		struct {
			uint32_t period_ms;
			uint32_t next_ms;
		} periodic;
	} t;
} sched_entry_t;

/**
 * @brief Initialize scheduler task and load persisted table.
 * @return 0 on success; negative on failure.
 */
int script_scheduler_init(void);

/**
 * @brief Schedule by absolute UTC seconds.
 * @param utc_s Absolute time (s since epoch, UTC).
 * @param file  Script filename (FatFs).
 * @return slot id >= 0 on success, negative on failure.
 */
int script_schedule_at_abs(uint32_t utc_s, const char *file);

/**
 * @brief Schedule after relative delay in ms (monotonic).
 * @param delay_ms Delay from now.
 * @param file     Script filename (FatFs).
 * @return slot id >= 0 on success, negative on failure.
 */
int script_schedule_after(uint32_t delay_ms, const char *file);

/**
 * @brief Schedule daily HH:MM local time.
 * @param hhmm "HH:MM" string.
 * @param file Script filename (FatFs).
 * @return slot id >= 0 on success, negative on failure.
 */
int script_schedule_daily(const char *hhmm, const char *file);

/**
 * @brief Schedule periodic every period_ms.
 * @param period_ms Period in milliseconds.
 * @param file      Script filename (FatFs).
 * @return slot id >= 0 on success, negative on failure.
 */
int script_schedule_every(uint32_t period_ms, const char *file);

/**
 * @brief Enable/disable a schedule slot.
 * @param id Slot id.
 * @param en true to enable.
 * @return 0 on success.
 */
int script_schedule_set_enabled(int id, bool en);

/**
 * @brief Change missed policy for a slot.
 */
int script_schedule_set_policy(int id, missed_policy_t p);

/**
 * @brief Set remaining times (0 = infinite).
 */
int script_schedule_set_remain(int id, uint16_t times);

/**
 * @brief Delete a slot by id.
 */
int script_schedule_delete(int id);

/**
 * @brief List all schedules (prints to CLI).
 */
void script_schedule_list(void);

/**
 * @brief Clear all schedules.
 */
void script_schedule_clear(void);

/* ===== Persistence API (LittleFS-backed) ===== */

/**
 * @brief Mark table dirty; real flush debounced in scheduler loop.
 * @return 0
 */
int script_persist_save(void);

/**
 * @brief Load table from LittleFS (atomic file with CRC); rebuilds table.
 * @return 0 on success, negative on format/IO error.
 */
int script_persist_load(void);

/**
 * @brief Called by scheduler loop with monotonic now_ms to try flush.
 */
void script_persist_try_flush(uint32_t now_ms);

/* ===== Runner hook ===== */

/* ===== Platform time hooks (provide weak stubs; override in platform) ===== */

/**
 * @brief Get local HH:MM (24h).
 */
int script_platform_get_hhmm(int *hh, int *mm);

/**
 * @brief Get full datetime components (UTC/local per platform config).
 */
int script_platform_get_datetime(int *y, int *m, int *d, int *hh, int *mm, int *ss);

/**
 * @brief Monotonic milliseconds since boot.
 */
uint32_t script_platform_millis(void);

/**
 * @brief UTC seconds since epoch.
 */
uint32_t script_platform_time_utc_s(void);

/* ===== Snapshot record for persistence ===== */

#pragma pack(push, 1)
typedef struct {
	uint8_t enabled; /* 0/1 */
	uint8_t kind; /* sched_kind_t */
	uint8_t policy; /* missed_policy_t */
	uint8_t hh; /* DAILY target, else 0 */
	uint8_t mm; /* DAILY target, else 0 */
	uint8_t rsv0; /* reserved */
	uint16_t rsv1; /* align */
	uint32_t at_utc_s; /* ABS UTC */
	uint32_t delay_ms; /* REL remaining */
	uint32_t period_ms; /* PERIODIC */
	uint16_t remain_times; /* 0 = infinite */
	uint16_t rsv2; /* reserved */
	char file[SCRIPT_SCHED_FNAME_MAX]; /* NUL-terminated */
} script_persist_record_t;
#pragma pack(pop)

/**
 * @brief Fill records snapshot; called by persistence during flush.
 * @param out     Output array.
 * @param max     Max records to fill.
 * @param now_ms  Current monotonic time for REL remaining snapshot.
 * @return number of records filled.
 */
uint16_t script_scheduler_snapshot(script_persist_record_t *out, uint16_t max, uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif /* SCRIPT_SCHEDULER_H */

