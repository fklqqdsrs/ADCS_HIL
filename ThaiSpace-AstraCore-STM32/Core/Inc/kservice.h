/*
 * kservice.h
 *
 *  Created on: 11 May 2025
 *      Author: Pongpot
 */

#ifndef INC_KSERVICE_H_
#define INC_KSERVICE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#include <cmsis_os.h>
#include <hw_config.h>
#include "stm32_lock.h"
#include "sys/time.h"

#include <ksdef.h>
#include <list_service.h>


/**
 * @addtogroup KernelService
 */

/**@{*/

#ifdef TSC_USING_HEAP
/*
 * heap memory interface
 */
void tsc_system_heap_init(void *begin_addr, void *end_addr);

void* tsc_malloc(size_t nbytes);
void tsc_free(void *ptr);
void* tsc_realloc(void *ptr, size_t nbytes);
void* tsc_calloc(size_t count, size_t size);
void* tsc_malloc_align(size_t size, size_t align);
void tsc_free_align(void *ptr);

void tsc_memory_info(size_t *total, size_t *used, size_t *max_used);
#endif

/*
 * general kernel service
 */
#ifndef TSC_USING_CONSOLE
#define tsc_kprintf(...)
#define tsc_kputs(str)
#else
int tsc_kprintf(const char *fmt, ...);
void tsc_kputs(const char *str);
#endif

int tsc_vsprintf(char *dest, const char *format, va_list arg_ptr);
int tsc_vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int tsc_sprintf(char *buf, const char *format, ...);
int tsc_snprintf(char *buf, size_t size, const char *format, ...);

int __tsc_ffs(int value);

#ifndef TSC_KSERVICE_USING_STDLIB_MEMSET
void* tsc_memset(void *src, int c, unsigned long n);
#endif /* TSC_KSERVICE_USING_STDLIB_MEMSET */
#ifndef TSC_KSERVICE_USING_STDLIB_MEMCPY
void* tsc_memcpy(void *dest, const void *src, unsigned long n);
#endif /* TSC_KSERVICE_USING_STDLIB_MEMCPY */
char* tsc_strdup(const char *s);

#ifndef TSC_KSERVICE_USING_STDLIB
void* tsc_memmove(void *dest, const void *src, size_t n);
int32_t tsc_memcmp(const void *cs, const void *ct, size_t count);
char* tsc_strstr(const char *str1, const char *str2);
int32_t tsc_strcasecmp(const char *a, const char *b);
char* tsc_strcpy(char *dst, const char *src);
char* tsc_strncpy(char *dest, const char *src, size_t n);
int32_t tsc_strncmp(const char *cs, const char *ct, size_t count);
int32_t tsc_strcmp(const char *cs, const char *ct);
size_t tsc_strlen(const char *src);
#else
#include <string.h>
#ifdef TSC_KSERVICE_USING_STDLIB_MEMSET
#define tsc_memset(s, c, count)      memset(s, c, count)
#endif /* TSC_KSERVICE_USING_STDLIB_MEMSET */
#ifdef TSC_KSERVICE_USING_STDLIB_MEMCPY
#define tsc_memcpy(dst, src, count)  memcpy(dst, src, count)
#endif /* TSC_KSERVICE_USING_STDLIB_MEMCPY */
#define tsc_memmove(dest, src, n)    memmove(dest, src, n)
#define tsc_memcmp(cs, ct, count)    memcmp(cs, ct, count)
#define tsc_strstr(str1, str2)       strstr(str1, str2)
#define tsc_strcasecmp(a, b)         strcasecmp(a, b)
#define tsc_strcpy(dest, src)        strcpy(dest, src)
#define tsc_strncpy(dest, src, n)    strncpy(dest, src, n)
#define tsc_strncmp(cs, ct, count)   strncmp(cs, ct, count)
#define tsc_strcmp(cs, ct)           strcmp(cs, ct)
#define tsc_strlen(src)              strlen(src)
#endif /*TSC_KSERVICE_USING_STDLIB*/

#if !defined(TSC_KSERVICE_USING_STDLIB) || defined(__ARMCC_VERSION)
size_t tsc_strnlen(const char *s, unsigned long maxlen);
#else
#define tsc_strnlen(s, maxlen)       strnlen(s, maxlen)
#endif /* !defined(TSC_KSERVICE_USING_STDLIB) || defined(__ARMCC_VERSION) */

#ifdef __ARMCC_VERSION
/* MDK doesn't have these APIs */
char* strdup(const char* str);
size_t strnlen(const char *s, size_t maxlen);
#endif /* __ARMCC_VERSION */


#ifdef TSC_DEBUG
extern void (*tsc_asseert_hook)(const char *ex, const char *func, size_t line);
void tsc_assert_set_hook(void (*hook)(const char *ex, const char *func, size_t line));

void tsc_assert_handler(const char *ex, const char *func, size_t line);
#endif /* RT_DEBUG */

#ifdef TSC_USING_FINSH
#include <finsh.h>
#endif

/**@}*/

#endif /* INC_KSERVICE_H_ */
