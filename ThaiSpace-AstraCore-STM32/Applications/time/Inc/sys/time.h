/*
 * time.h
 *
 *  Created on: 20 June 2025
 *      Author: Pongpot
 */

#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#include <kservice.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* timezone */
#define DST_NONE    0   /* not on dst */
#define DST_USA     1   /* USA style dst */
#define DST_AUST    2   /* Australian style dst */
#define DST_WET     3   /* Western European dst */
#define DST_MET     4   /* Middle European dst */
#define DST_EET     5   /* Eastern European dst */
#define DST_CAN     6   /* Canada */
#define DST_GB      7   /* Great Britain and Eire */
#define DST_RUM     8   /* Rumania */
#define DST_TUR     9   /* Turkey */
#define DST_AUSTALT 10  /* Australian style with shift in 1986 */

struct timezone
{
    int tz_minuteswest;   /* minutes west of Greenwich */
    int tz_dsttime;       /* type of dst correction */
};

#if !defined(_TIMEVAL_DEFINED) && !defined(_WIN32)
#define _TIMEVAL_DEFINED
struct timeval
{
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* and microseconds */
};
#endif

#if !(defined(__GNUC__) && !defined(__ARMCC_VERSION)/*GCC*/) && \
    !(defined(__ICCARM__) && (__VER__ >= 8010001)) && \
    !defined(_WIN32)
struct timespec
{
    time_t  tv_sec;     /* seconds */
    long    tv_nsec;    /* and nanoseconds */
};

/*
 * Structure defined by POSIX.1b to be like a itimerval, but with
 * timespecs. Used in the timer_*() system calls.
 */
struct itimerspec
{
    struct timespec  it_interval;
    struct timespec  it_value;
};
#endif

int stime(const time_t *t);
time_t timegm(struct tm * const t);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);
#if defined(__ARMCC_VERSION) || defined (__ICCARM__)
struct tm *gmtime_r(const time_t *timep, struct tm *r);
struct tm* localtime_r(const time_t* t, struct tm* r);
char* asctime_r(const struct tm *t, char *buf);
char *ctime_r(const time_t * tim_p, char * result);
#endif

/* timezone */
void tz_set(int8_t tz);
int8_t tz_get(void);
int8_t tz_is_dst(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_TIME_H_ */
