//! @file time_app.h

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : Device time application.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

#ifndef _TIME_MANAGER_H
#define _TIME_MANAGER_H

#if defined __cplusplus
extern "C" {
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "sys/time.h"

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
#define TIME_MANAGER_NOT_SYNC	0
#define TIME_MANAGER_SYNC		1

// ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
// N/A

// ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
// N/A

// ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A

// ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A

// ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A

// ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A

// ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //
int time_manager_get_rtc_time(time_t *now);
int time_manager_get_pps_time(time_t *now);
int time_manager_set_rtc_time(time_t now);
int time_manager_set_pps_time(time_t now);
uint8_t time_manager_is_rtc_syn(void);
uint8_t time_manager_is_pps_syn(void);
int time_manager_init(void);
int time_manager_set_date(uint32_t year, uint32_t month, uint32_t day);
int time_manager_set_time(uint32_t hour, uint32_t minute, uint32_t second);

// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif  // #ifndef _TIME_MANAGER_H
