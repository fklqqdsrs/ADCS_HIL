#include "app_modules_config.h"
#if ENABLE_SCRIPT_MANAGER

/**
 * @file script_scheduler.c
 * @brief Mini telecommand scheduler core. No malloc after init.
 */

#include "script_scheduler.h"

#include <stdio.h>
#include <string.h>
#include <kservice.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>

/* FreeRTOS critical, fallback to empty if not available */
#if defined(__has_include)
#if __has_include("FreeRTOS.h")
#include "FreeRTOS.h"
#include "task.h"
#define SCHED_CRITICAL_ENTER() taskENTER_CRITICAL()
#define SCHED_CRITICAL_EXIT() taskEXIT_CRITICAL()
#else
#define SCHED_CRITICAL_ENTER()
#define SCHED_CRITICAL_EXIT()
#endif
#else
#define SCHED_CRITICAL_ENTER()
#define SCHED_CRITICAL_EXIT()
#endif

sched_entry_t g_sched[SCRIPT_MAX_SCHED];

static int find_free_slot(void) {
	for (int i = 0; i < (int) SCRIPT_MAX_SCHED; ++i) {
		if (!g_sched[i].used) return i;
	}
	return -1;
}

static void mark_dirty(void) {
	(void) script_persist_save();
}

static uint16_t ymd_compact(int y, int m, int d) {
	/* Compact Y-M-D into 16 bits: (y-2000)<<9 | (m<<5) | d ; valid until 2063 */
	uint16_t yy = (uint16_t) ((y >= 2000) ? (y - 2000) : 0);
	return (uint16_t) ((yy << 9) | ((uint16_t) m << 5) | (uint16_t) d);
}

/* ===== Public API implementation ===== */

int script_scheduler_init(void) {
	memset(g_sched, 0, sizeof(g_sched));
	(void) script_persist_load();
#if SCRIPT_SCHED_LOG_ON_BOOT
	printf("[SCHED] Restored schedule table:\r\n");
	script_schedule_list();
#endif
	/* Create scheduler task */
	extern void task_script_scheduler(void *param);
	if (csp_thread_create(task_script_scheduler, "Sched",
	SCRIPT_SCHED_THREAD_STACK_SIZE,
	NULL, osPriorityAboveNormal,
	NULL) != CSP_ERR_NONE) {
		return -1;
	}
	return 0;
}

static int add_common_preamble(const char *file, int *slot_out) {
	if ((file == NULL) || (file[0] == '\0')) {
		return -1;
	}
	int slot = find_free_slot();
	if (slot < 0) {
		return -2;
	}
	SCHED_CRITICAL_ENTER();
	sched_entry_t *e = &g_sched[slot];
	memset(e, 0, sizeof(*e));
	e->used = true;
	e->enabled = true;
	e->policy = MISSED_CATCH_UP_ONCE; /* default */
	(void) strncpy(e->file, file, (size_t) SCRIPT_SCHED_FNAME_MAX - 1u);
	e->file[SCRIPT_SCHED_FNAME_MAX - 1u] = '\0';
	SCHED_CRITICAL_EXIT();
	*slot_out = slot;
	return 0;
}

int script_schedule_at_abs(uint32_t utc_s, const char *file) {
	int slot = -1;
	if (add_common_preamble(file, &slot) != 0) return -1;
	SCHED_CRITICAL_ENTER();
	g_sched[slot].kind = SCHED_KIND_ABS_UTC;
	g_sched[slot].t.abs_at.utc_s = utc_s;
	g_sched[slot].remain = 1u;
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return slot;
}

int script_schedule_after(uint32_t delay_ms, const char *file) {
	int slot = -1;
	if (add_common_preamble(file, &slot) != 0) return -1;
	uint32_t now = script_platform_millis();
	SCHED_CRITICAL_ENTER();
	g_sched[slot].kind = SCHED_KIND_REL_MS;
	g_sched[slot].t.rel_at.due_ms = now + delay_ms;
	g_sched[slot].remain = 1u;
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return slot;
}

static bool parse_hhmm(const char *s, uint8_t *out_hh, uint8_t *out_mm) {
	if ((s == NULL) || (strlen(s) < 4u)) return false;
	int hh = 0, mm = 0;
	if (sscanf(s, "%2d:%2d", &hh, &mm) != 2) return false;
	if ((hh < 0) || (hh > 23) || (mm < 0) || (mm > 59)) return false;
	*out_hh = (uint8_t) hh;
	*out_mm = (uint8_t) mm;
	return true;
}

int script_schedule_daily(const char *hhmm, const char *file) {
	int slot = -1;
	uint8_t hh = 0, mm = 0;
	if (!parse_hhmm(hhmm, &hh, &mm)) return -1;
	if (add_common_preamble(file, &slot) != 0) return -1;
	SCHED_CRITICAL_ENTER();
	g_sched[slot].kind = SCHED_KIND_DAILY_HHMM;
	g_sched[slot].t.daily.hh = hh;
	g_sched[slot].t.daily.mm = mm;
	g_sched[slot].t.daily.last_ymd = 0u; /* not run yet */
	g_sched[slot].remain = 0u; /* infinite */
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return slot;
}

int script_schedule_every(uint32_t period_ms, const char *file) {
	if (period_ms == 0u) return -1;
	int slot = -1;
	if (add_common_preamble(file, &slot) != 0) return -1;
	uint32_t now = script_platform_millis();
	SCHED_CRITICAL_ENTER();
	g_sched[slot].kind = SCHED_KIND_PERIODIC_MS;
	g_sched[slot].t.periodic.period_ms = period_ms;
	g_sched[slot].t.periodic.next_ms = now + period_ms;
	g_sched[slot].remain = 0u; /* infinite */
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return slot;
}

int script_schedule_set_enabled(int id, bool en) {
	if ((id < 0) || (id >= (int) SCRIPT_MAX_SCHED)) return -1;
	SCHED_CRITICAL_ENTER();
	if (!g_sched[id].used) {
		SCHED_CRITICAL_EXIT();
		return -1;
	}
	g_sched[id].enabled = en;
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return 0;
}

int script_schedule_set_policy(int id, missed_policy_t p) {
	if ((id < 0) || (id >= (int) SCRIPT_MAX_SCHED)) return -1;
	SCHED_CRITICAL_ENTER();
	if (!g_sched[id].used) {
		SCHED_CRITICAL_EXIT();
		return -1;
	}
	g_sched[id].policy = p;
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return 0;
}

int script_schedule_set_remain(int id, uint16_t times) {
	if ((id < 0) || (id >= (int) SCRIPT_MAX_SCHED)) return -1;
	SCHED_CRITICAL_ENTER();
	if (!g_sched[id].used) {
		SCHED_CRITICAL_EXIT();
		return -1;
	}
	g_sched[id].remain = times;
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return 0;
}

static void delete_slot_nolock(int id) {
	if ((id < 0) || (id >= (int) SCRIPT_MAX_SCHED)) return;
	memset(&g_sched[id], 0, sizeof(g_sched[id]));
}

int script_schedule_delete(int id) {
	if ((id < 0) || (id >= (int) SCRIPT_MAX_SCHED)) return -1;
	SCHED_CRITICAL_ENTER();
	delete_slot_nolock(id);
	SCHED_CRITICAL_EXIT();
	mark_dirty();
	return 0;
}

void script_schedule_clear(void) {
	SCHED_CRITICAL_ENTER();
	memset(g_sched, 0, sizeof(g_sched));
	SCHED_CRITICAL_EXIT();
	mark_dirty();
}

void script_schedule_list(void) {
	printf("ID En Kind Policy Remain  Detail  File\r\n");
	for (int i = 0; i < (int) SCRIPT_MAX_SCHED; ++i) {
		SCHED_CRITICAL_ENTER();
		sched_entry_t e = g_sched[i];
		SCHED_CRITICAL_EXIT();
		if (!e.used) continue;
		const char *kind = (e.kind == SCHED_KIND_ABS_UTC) ? "ABS" : (e.kind == SCHED_KIND_REL_MS) ? "REL" :
							(e.kind == SCHED_KIND_DAILY_HHMM) ? "DAY" : (e.kind == SCHED_KIND_PERIODIC_MS) ? "PER" : "?";
		const char *pol = (e.policy == MISSED_SKIP) ? "skip" : "catch";
		char detail[48];
		if (e.kind == SCHED_KIND_ABS_UTC) {
			(void) snprintf(detail, sizeof(detail), "utc=%lu", (unsigned long) e.t.abs_at.utc_s);
		} else if (e.kind == SCHED_KIND_REL_MS) {
			uint32_t now = script_platform_millis();
			uint32_t rem = (e.t.rel_at.due_ms > now) ? (e.t.rel_at.due_ms - now) : 0u;
			(void) snprintf(detail, sizeof(detail), "rem=%lu", (unsigned long) rem);
		} else if (e.kind == SCHED_KIND_DAILY_HHMM) {
			(void) snprintf(detail, sizeof(detail), "%02u:%02u", (unsigned) e.t.daily.hh, (unsigned) e.t.daily.mm);
		} else if (e.kind == SCHED_KIND_PERIODIC_MS) {
			(void) snprintf(detail, sizeof(detail), "per=%lu", (unsigned long) e.t.periodic.period_ms);
		} else {
			(void) snprintf(detail, sizeof(detail), "-");
		}
		printf("%2d %2u %-4s %-6s %6u  %-8s %s\r\n", i, (unsigned) e.enabled, kind, pol, (unsigned) e.remain, detail, e.file);
	}
}

/* ===== Snapshot for persistence ===== */

uint16_t script_scheduler_snapshot(script_persist_record_t *out, uint16_t max, uint32_t now_ms) {
	if ((out == NULL) || (max == 0u)) return 0u;
	uint16_t count = 0u;
	SCHED_CRITICAL_ENTER();
	for (int i = 0; i < (int) SCRIPT_MAX_SCHED; ++i) {
		if ((!g_sched[i].used) || (count >= max)) {
			continue;
		}
		script_persist_record_t *r = &out[count];
		memset(r, 0, sizeof(*r));
		r->enabled = (uint8_t) g_sched[i].enabled;
		r->kind = (uint8_t) g_sched[i].kind;
		r->policy = (uint8_t) g_sched[i].policy;
		r->remain_times = g_sched[i].remain;
		(void) strncpy(r->file, g_sched[i].file, (size_t) SCRIPT_SCHED_FNAME_MAX - 1u);
		r->file[SCRIPT_SCHED_FNAME_MAX - 1u] = '\0';
		switch (g_sched[i].kind) {
		case SCHED_KIND_ABS_UTC:
			r->at_utc_s = g_sched[i].t.abs_at.utc_s;
			break;
		case SCHED_KIND_REL_MS: {
			uint32_t due = g_sched[i].t.rel_at.due_ms;
			r->delay_ms = (due > now_ms) ? (due - now_ms) : 0u;
			break;
		}
		case SCHED_KIND_DAILY_HHMM:
			r->hh = g_sched[i].t.daily.hh;
			r->mm = g_sched[i].t.daily.mm;
			break;
		case SCHED_KIND_PERIODIC_MS:
			r->period_ms = g_sched[i].t.periodic.period_ms;
			break;
		default:
			break;
		}
		count++;
	}
	SCHED_CRITICAL_EXIT();
	return count;
}

/* ===== Scheduler task ===== */

static bool should_run_and_update(int idx, uint32_t now_ms, uint32_t *utc_now_s) {
	sched_entry_t *e = &g_sched[idx];
	if (!e->used || !e->enabled) return false;

	switch (e->kind) {
	case SCHED_KIND_ABS_UTC: {
		uint32_t now_s = (utc_now_s != NULL) ? *utc_now_s : script_platform_time_utc_s();
		if (utc_now_s != NULL) {
			*utc_now_s = now_s;
		}
		if (now_s >= e->t.abs_at.utc_s) {
			bool missed = (now_s > e->t.abs_at.utc_s);
			if ((e->policy == MISSED_SKIP) && missed) {
				/* Skip and delete */
				delete_slot_nolock(idx);
				mark_dirty();
				return false;
			}
			/* Run once and delete */
			if (e->remain > 0u) {
				e->remain--;
			}
			delete_slot_nolock(idx);
			mark_dirty();
			return true;
		}
		return false;
	}
	case SCHED_KIND_REL_MS:
		if (now_ms >= e->t.rel_at.due_ms) {
			bool missed = (now_ms > e->t.rel_at.due_ms);
			if ((e->policy == MISSED_SKIP) && missed) {
				delete_slot_nolock(idx);
				mark_dirty();
				return false;
			}
			if (e->remain > 0u) {
				e->remain--;
			}
			delete_slot_nolock(idx);
			mark_dirty();
			return true;
		}
		return false;
	case SCHED_KIND_DAILY_HHMM: {
		int y = 0, m = 0, d = 0, hh = 0, mm = 0, ss = 0;
		(void) script_platform_get_datetime(&y, &m, &d, &hh, &mm, &ss);
		uint16_t ymd = ymd_compact(y, m, d);
		bool time_eq = ((uint8_t) hh == e->t.daily.hh) && ((uint8_t) mm == e->t.daily.mm);
		bool time_passed = (((uint8_t) hh > e->t.daily.hh) || (((uint8_t) hh == e->t.daily.hh) && ((uint8_t) mm > e->t.daily.mm)));
		if ((e->t.daily.last_ymd != ymd) && (time_eq || ((e->policy == MISSED_CATCH_UP_ONCE) && time_passed))) {
			e->t.daily.last_ymd = ymd;
			if (e->remain > 0u) {
				e->remain--;
				if (e->remain == 0u) {
					delete_slot_nolock(idx);
				}
			}
			mark_dirty();
			return true;
		}
		return false;
	}
	case SCHED_KIND_PERIODIC_MS:
		if (now_ms >= e->t.periodic.next_ms) {
			if (e->policy == MISSED_SKIP) {
				e->t.periodic.next_ms = now_ms + e->t.periodic.period_ms;
				mark_dirty();
				return false;
			}
			/* catch up once */
			e->t.periodic.next_ms = now_ms + e->t.periodic.period_ms;
			if (e->remain > 0u) {
				e->remain--;
				if (e->remain == 0u) {
					delete_slot_nolock(idx);
				}
			}
			mark_dirty();
			return true;
		}
		return false;
	default:
		return false;
	}
}

CSP_DEFINE_TASK(task_script_scheduler) {
	(void) param;
	for (;;) {
		uint32_t now_ms = script_platform_millis();
		/* Try persistence flush (debounced) */
		script_persist_try_flush(now_ms);

		int to_run = -1;
		char fname[SCRIPT_SCHED_FNAME_MAX];
		uint32_t utc_now_s = 0u;

		SCHED_CRITICAL_ENTER();
		for (int i = 0; i < (int) SCRIPT_MAX_SCHED; ++i) {
			if (!g_sched[i].used || !g_sched[i].enabled) continue;
			if (g_sched[i].kind == SCHED_KIND_ABS_UTC) {
				/* Cache UTC read once per tick */
				if (utc_now_s == 0u) {
					utc_now_s = script_platform_time_utc_s();
				}
			}
			if (should_run_and_update(i, now_ms, &utc_now_s)) {
				to_run = i;
				(void) strncpy(fname, g_sched[i].file, (size_t) SCRIPT_SCHED_FNAME_MAX - 1u);
				fname[SCRIPT_SCHED_FNAME_MAX - 1u] = '\0';
				break; /* one item per tick */
			}
		}
		SCHED_CRITICAL_EXIT();

		if (to_run >= 0) {
			script_run_local(fname); /* run outside lock */
		}

		csp_sleep_ms(SCRIPT_SCHED_TICK_MS);
	}
	/* not reached */
	return CSP_TASK_RETURN;
}

/* ===== Weak platform hooks (override in platform) ===== */

__attribute__((weak)) int script_platform_get_hhmm(int *hh, int *mm) {
	if ((hh == NULL) || (mm == NULL)) return -1;
	*hh = 0;
	*mm = 0;
	return 0;
}

__attribute__((weak)) int script_platform_get_datetime(int *y, int *m, int *d, int *hh, int *mm, int *ss) {
	if (!y || !m || !d || !hh || !mm || !ss) return -1;
	*y = 2000;
	*m = 1;
	*d = 1;
	*hh = 0;
	*mm = 0;
	*ss = 0;
	return 0;
}

__attribute__((weak))  uint32_t script_platform_millis(void) {
	static uint32_t fake = 0u;
	fake += 10u;
	return fake;
}

__attribute__((weak))  uint32_t script_platform_time_utc_s(void) {
	return 0u;
}


#endif /* ENABLE_SCRIPT_MANAGER */
