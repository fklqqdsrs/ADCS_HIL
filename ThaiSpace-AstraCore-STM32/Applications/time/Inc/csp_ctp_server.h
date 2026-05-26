//! @file csp_ctp_task.h

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP time task.
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

#ifndef _CSP_CTP_TASK_H
#define _CSP_CTP_TASK_H

#if defined __cplusplus
extern "C" {
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
// N/A

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
int csp_ctp_server_init(uint8_t server_port, uint32_t broadcast_period);
int csp_ctp_client_init(uint8_t node, uint32_t update_period);

// ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A

// ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //
// N/A

// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif  // #ifndef _CSP_CTP_TASK_H
