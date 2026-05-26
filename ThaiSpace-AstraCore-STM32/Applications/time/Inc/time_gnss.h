//! @file time_gnss.h

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : GNSS time source.
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

#ifndef _TIME_GNSS_H
#define _TIME_GNSS_H

#if defined __cplusplus
extern "C" {
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
#include <hw_config.h>

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A

// ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
// N/A

// ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
// N/A

// ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A

// ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A

// ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //
void time_gnss_rx_finish(UART_HandleTypeDef *huart);
void time_gnss_rx_error(UART_HandleTypeDef *huart);
void time_gnss_init(void);

// ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A

// ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //
// N/A

// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif  // #ifndef _TIME_GNSS_H
