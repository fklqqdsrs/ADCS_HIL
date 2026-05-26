/*
 * shell.h
 *
 *  Created on: 27 June 2025
 *      Author: Pongpot
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include <cmsis_os.h>
#include "finsh.h"

#ifndef FINSH_THREAD_PRIORITY
    #define FINSH_THREAD_PRIORITY osPriorityNormal
#endif
#ifndef FINSH_THREAD_STACK_SIZE
    #define FINSH_THREAD_STACK_SIZE 1024*5
#endif
#ifndef FINSH_CMD_SIZE
    #define FINSH_CMD_SIZE      80
#endif

#define FINSH_OPTION_ECHO   0x01

#define FINSH_PROMPT        finsh_get_prompt()
const char *finsh_get_prompt(void);
int finsh_set_prompt(const char *prompt);

#ifdef FINSH_USING_HISTORY
    #ifndef FINSH_HISTORY_LINES
        #define FINSH_HISTORY_LINES 5
    #endif
#endif

#ifdef FINSH_USING_AUTH
    #ifndef FINSH_PASSWORD_MAX
        #define FINSH_PASSWORD_MAX 32
    #endif
    #ifndef FINSH_PASSWORD_MIN
        #define FINSH_PASSWORD_MIN 6
    #endif
    #ifndef FINSH_DEFAULT_PASSWORD
        #define FINSH_DEFAULT_PASSWORD "password"
    #endif
#endif /* FINSH_USING_AUTH */

#ifndef FINSH_THREAD_NAME
    #define FINSH_THREAD_NAME   "tshell"
#endif

enum input_stat
{
    WAIT_NORMAL,
    WAIT_SPEC_KEY,
    WAIT_FUNC_KEY,
};
struct finsh_shell
{
    osSemaphoreAttr_t rx_sem;

    enum input_stat stat;

    uint8_t echo_mode: 1;
    uint8_t prompt_mode: 1;

#ifdef FINSH_USING_HISTORY
    uint16_t current_history;
    uint16_t history_count;

    char cmd_history[FINSH_HISTORY_LINES][FINSH_CMD_SIZE];
#endif

    char line[FINSH_CMD_SIZE + 1];
    uint16_t line_position;
    uint16_t line_curpos;

#ifdef FINSH_USING_AUTH
    char password[FINSH_PASSWORD_MAX];
#endif
};

void finsh_set_echo(uint32_t echo);
uint32_t finsh_get_echo(void);

int finsh_system_init(void);
const char *finsh_get_device(void);
int finsh_getchar(void);

uint32_t finsh_get_prompt_mode(void);
void finsh_set_prompt_mode(uint32_t prompt_mode);

#ifdef FINSH_USING_AUTH
    int finsh_set_password(const char *password);
    const char *finsh_get_password(void);
#endif

#ifdef TSC_USING_HOOK
void finsh_thread_entry_sethook(void (*hook)(void));
#endif /* TSC_USING_HOOK */

#endif
