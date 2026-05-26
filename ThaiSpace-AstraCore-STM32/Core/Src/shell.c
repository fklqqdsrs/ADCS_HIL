/*
 * shell.c
 *
 *  Created on: 27 June 2025
 *      Author: Pongpot
 */

#include <kservice.h>

#ifdef TSC_USING_FINSH

#include "shell.h"
#include "msh.h"

/* Definitions for rx_hw_console_sem */
osSemaphoreId_t rx_console_sem;
const osSemaphoreAttr_t rx_console_sem_attributes = {
  .name = "rx_console_sem"
};

osThreadId_t shellTaskHandle;

/* finsh thread */
#ifndef TSC_USING_HEAP
    static osThreadAttr_t finsh_thread;
    __ALIGNED(4)
    static char finsh_thread_stack[FINSH_THREAD_STACK_SIZE];
    struct finsh_shell _shell;
#endif

/* finsh symtab */
#ifdef FINSH_USING_SYMTAB
    struct finsh_syscall *_syscall_table_begin  = NULL;
    struct finsh_syscall *_syscall_table_end    = NULL;
#endif

struct finsh_shell *shell;
static char *finsh_prompt_custom = NULL;

#if defined(_MSC_VER) || (defined(__GNUC__) && defined(__x86_64__))
struct finsh_syscall *finsh_syscall_next(struct finsh_syscall *call)
{
    unsigned int *ptr;
    ptr = (unsigned int *)(call + 1);
    while ((*ptr == 0) && ((unsigned int *)ptr < (unsigned int *) _syscall_table_end))
        ptr ++;

    return (struct finsh_syscall *)ptr;
}

#endif /* defined(_MSC_VER) || (defined(__GNUC__) && defined(__x86_64__)) */

#ifdef TSC_USING_HEAP
int finsh_set_prompt(const char *prompt)
{
    if (finsh_prompt_custom)
    {
    	vPortFree(finsh_prompt_custom);
        finsh_prompt_custom = NULL;
    }

    /* strdup */
    if (prompt)
    {
        finsh_prompt_custom = (char *)pvPortMalloc(strlen(prompt) + 1);
        if (finsh_prompt_custom)
        {
            strcpy(finsh_prompt_custom, prompt);
        }
    }

    return 0;
}
#endif /* TSC_USING_HEAP */

#define _MSH_PROMPT "TSC-1 "

const char *finsh_get_prompt(void)
{
    static char finsh_prompt[TSC_CONSOLEBUF_SIZE + 1] = {0};

    /* check prompt mode */
    if (!shell->prompt_mode)
    {
        finsh_prompt[0] = '\0';
        return finsh_prompt;
    }

    if (finsh_prompt_custom)
    {
        strncpy(finsh_prompt, finsh_prompt_custom, sizeof(finsh_prompt) - 1);
    }
    else
    {
        strcpy(finsh_prompt, _MSH_PROMPT);
    }

#if defined(DFS_USING_POSIX) && defined(DFS_USING_WORKDIR)
    /* get current working directory */
    getcwd(&finsh_prompt[tsc_strlen(finsh_prompt)], TSC_CONSOLEBUF_SIZE - tsc_strlen(finsh_prompt));
#endif

    if (strlen(finsh_prompt) + 2 < TSC_CONSOLEBUF_SIZE)
    {
        finsh_prompt[strlen(finsh_prompt)] = '>';
        finsh_prompt[strlen(finsh_prompt) + 1] = '\0';
    }

    return finsh_prompt;
}

/**
 * @ingroup group_finsh
 *
 * This function get the prompt mode of finsh shell.
 *
 * @return prompt the prompt mode, 0 disable prompt mode, other values enable prompt mode.
 */
uint32_t finsh_get_prompt_mode(void)
{
	TSC_ASSERT(shell != NULL);
    return shell->prompt_mode;
}

/**
 * @ingroup group_finsh
 *
 * This function set the prompt mode of finsh shell.
 *
 * The parameter 0 disable prompt mode, other values enable prompt mode.
 *
 * @param prompt_mode the prompt mode
 */
void finsh_set_prompt_mode(uint32_t prompt_mode)
{
	TSC_ASSERT(shell != NULL);
    shell->prompt_mode = prompt_mode;
}

int finsh_getchar(void)
{
    extern char tsc_hw_console_getchar(void);
    return tsc_hw_console_getchar();
}

/**
 * @ingroup group_finsh
 *
 * This function set the echo mode of finsh shell.
 *
 * FINSH_OPTION_ECHO=0x01 is echo mode, other values are none-echo mode.
 *
 * @param echo the echo mode
 */
void finsh_set_echo(uint32_t echo)
{
	TSC_ASSERT(shell != NULL);
    shell->echo_mode = (uint8_t)echo;
}

/**
 * @ingroup group_finsh
 *
 * This function gets the echo mode of finsh shell.
 *
 * @return the echo mode
 */
uint32_t finsh_get_echo()
{
	TSC_ASSERT(shell != NULL);

    return shell->echo_mode;
}

#ifdef FINSH_USING_AUTH
/**
 * set a new password for finsh
 *
 * @param password new password
 *
 * @return result, TSC_EOK on OK, -TSC_ERROR on the new password length is less than
 *  FINSH_PASSWORD_MIN or greater than FINSH_PASSWORD_MAX
 */
int finsh_set_password(const char *password)
{
    size_t pw_len = tsc_strlen(password);

    if (pw_len < FINSH_PASSWORD_MIN || pw_len > FINSH_PASSWORD_MAX)
        return -TSC_ERROR;

    taskENTER_CRITICAL();
    tsc_strncpy(shell->password, password, FINSH_PASSWORD_MAX);
    taskEXIT_CRITICAL();

    return TSC_EOK;
}

/**
 * get the finsh password
 *
 * @return password
 */
const char *finsh_get_password(void)
{
    return shell->password;
}

static void finsh_wait_auth(void)
{
    int ch;
    bool input_finish = false;
    char password[FINSH_PASSWORD_MAX] = { 0 };
    size_t cur_pos = 0;
    /* password not set */
    if (tsc_strlen(finsh_get_password()) == 0) return;

    while (1)
    {
        tsc_kprintf("Password for login: ");
        while (!input_finish)
        {
            while (1)
            {
                /* read one character from device */
                ch = (int)finsh_getchar();
                if (ch < 0)
                {
                    continue;
                }

                if (ch >= ' ' && ch <= '~' && cur_pos < FINSH_PASSWORD_MAX)
                {
                    /* change the printable characters to '*' */
                    tsc_kprintf("*");
                    password[cur_pos++] = ch;
                }
                else if (ch == '\b' && cur_pos > 0)
                {
                    /* backspace */
                    cur_pos--;
                    password[cur_pos] = '\0';
                    tsc_kprintf("\b \b");
                }
                else if (ch == '\r' || ch == '\n')
                {
                    tsc_kprintf("\n");
                    input_finish = true;
                    break;
                }
            }
        }
        if (!tsc_strncmp(shell->password, password, FINSH_PASSWORD_MAX)) return;
        else
        {
            /* authentication failed, delay 2S for retry */
            osDelay(2000);
            tsc_kprintf("Sorry, try again.\n");
            cur_pos = 0;
            input_finish = false;
            tsc_memset(password, '\0', FINSH_PASSWORD_MAX);
        }
    }
}
#endif /* FINSH_USING_AUTH */

static void shell_auto_complete(char *prefix)
{
    tsc_kprintf("\n");
    msh_auto_complete(prefix);

#ifdef FINSH_USING_OPTION_COMPLETION
    msh_opt_auto_complete(prefix);
#endif

    tsc_kprintf("%s%s", FINSH_PROMPT, prefix);
}

#ifdef FINSH_USING_HISTORY
static bool shell_handle_history(struct finsh_shell *shell)
{
#if defined(_WIN32)
    int i;
    tsc_kprintf("\r");

    for (i = 0; i <= 60; i++)
        putchar(' ');
    tsc_kprintf("\r");

#else
    tsc_kprintf("\033[2K\r");
#endif
    tsc_kprintf("%s%s", FINSH_PROMPT, shell->line);
    return false;
}

static void shell_push_history(struct finsh_shell *shell)
{
    if (shell->line_position != 0)
    {
        /* push history */
        if (shell->history_count >= FINSH_HISTORY_LINES)
        {
            /* if current cmd is same as last cmd, don't push */
            if (memcmp(&shell->cmd_history[FINSH_HISTORY_LINES - 1], shell->line, FINSH_CMD_SIZE))
            {
                /* move history */
                int index;
                for (index = 0; index < FINSH_HISTORY_LINES - 1; index ++)
                {
                    memcpy(&shell->cmd_history[index][0],
                           &shell->cmd_history[index + 1][0], FINSH_CMD_SIZE);
                }
                memset(&shell->cmd_history[index][0], 0, FINSH_CMD_SIZE);
                memcpy(&shell->cmd_history[index][0], shell->line, shell->line_position);

                /* it's the maximum history */
                shell->history_count = FINSH_HISTORY_LINES;
            }
        }
        else
        {
            /* if current cmd is same as last cmd, don't push */
            if (shell->history_count == 0 || memcmp(&shell->cmd_history[shell->history_count - 1], shell->line, FINSH_CMD_SIZE))
            {
                shell->current_history = shell->history_count;
                memset(&shell->cmd_history[shell->history_count][0], 0, FINSH_CMD_SIZE);
                memcpy(&shell->cmd_history[shell->history_count][0], shell->line, shell->line_position);

                /* increase count and set current history position */
                shell->history_count ++;
            }
        }
    }
    shell->current_history = shell->history_count;
}
#endif

static void finsh_thread_entry(void *parameter)
{
    int ch;

    /* normal is echo mode */
#ifndef FINSH_ECHO_DISABLE_DEFAULT
    shell->echo_mode = 1;
#else
    shell->echo_mode = 0;
#endif

#ifdef FINSH_USING_AUTH
    /* set the default password when the password isn't setting */
    if (tsc_strlen(finsh_get_password()) == 0)
    {
        if (finsh_set_password(FINSH_DEFAULT_PASSWORD) != TSC_EOK)
        {
            tsc_kprintf("Finsh password set failed.\n");
        }
    }
    /* waiting authenticate success */
    finsh_wait_auth();
#endif

    tsc_kprintf(FINSH_PROMPT);

    while (1)
    {
        ch = (int)finsh_getchar();
        if (ch < 0)
        {
            continue;
        }

        /*
         * handle control key
         * up key  : 0x1b 0x5b 0x41
         * down key: 0x1b 0x5b 0x42
         * right key:0x1b 0x5b 0x43
         * left key: 0x1b 0x5b 0x44
         */
        if (ch == 0x1b)
        {
            shell->stat = WAIT_SPEC_KEY;
            continue;
        }
        else if (shell->stat == WAIT_SPEC_KEY)
        {
            if (ch == 0x5b)
            {
                shell->stat = WAIT_FUNC_KEY;
                continue;
            }

            shell->stat = WAIT_NORMAL;
        }
        else if (shell->stat == WAIT_FUNC_KEY)
        {
            shell->stat = WAIT_NORMAL;

            if (ch == 0x41) /* up key */
            {
#ifdef FINSH_USING_HISTORY
                /* prev history */
                if (shell->current_history > 0)
                    shell->current_history --;
                else
                {
                    shell->current_history = 0;
                    continue;
                }

                /* copy the history command */
                memcpy(shell->line, &shell->cmd_history[shell->current_history][0],
                       FINSH_CMD_SIZE);
                shell->line_curpos = shell->line_position = (uint16_t)strlen(shell->line);
                shell_handle_history(shell);
#endif
                continue;
            }
            else if (ch == 0x42) /* down key */
            {
#ifdef FINSH_USING_HISTORY
                /* next history */
                if (shell->current_history < shell->history_count - 1)
                    shell->current_history ++;
                else
                {
                    /* set to the end of history */
                    if (shell->history_count != 0)
                        shell->current_history = shell->history_count - 1;
                    else
                        continue;
                }

                memcpy(shell->line, &shell->cmd_history[shell->current_history][0],
                       FINSH_CMD_SIZE);
                shell->line_curpos = shell->line_position = (uint16_t)strlen(shell->line);
                shell_handle_history(shell);
#endif
                continue;
            }
            else if (ch == 0x44) /* left key */
            {
                if (shell->line_curpos)
                {
                    tsc_kprintf("\b");
                    shell->line_curpos --;
                }

                continue;
            }
            else if (ch == 0x43) /* right key */
            {
                if (shell->line_curpos < shell->line_position)
                {
                    tsc_kprintf("%c", shell->line[shell->line_curpos]);
                    shell->line_curpos ++;
                }

                continue;
            }
        }

        /* received null or error */
        if (ch == '\0' || ch == 0xFF) continue;
        /* handle tab key */
        else if (ch == '\t')
        {
            int i;
            /* move the cursor to the beginning of line */
            for (i = 0; i < shell->line_curpos; i++)
                tsc_kprintf("\b");

            /* auto complete */
            shell_auto_complete(&shell->line[0]);
            /* re-calculate position */
            shell->line_curpos = shell->line_position = (uint16_t)strlen(shell->line);

            continue;
        }
        /* handle backspace key */
        else if (ch == 0x7f || ch == 0x08)
        {
            /* note that shell->line_curpos >= 0 */
            if (shell->line_curpos == 0)
                continue;

            shell->line_position--;
            shell->line_curpos--;

            if (shell->line_position > shell->line_curpos)
            {
                int i;

                memmove(&shell->line[shell->line_curpos],
                           &shell->line[shell->line_curpos + 1],
                           shell->line_position - shell->line_curpos);
                shell->line[shell->line_position] = 0;

                tsc_kprintf("\b%s  \b", &shell->line[shell->line_curpos]);

                /* move the cursor to the origin position */
                for (i = shell->line_curpos; i <= shell->line_position; i++)
                    tsc_kprintf("\b");
            }
            else
            {
                tsc_kprintf("\b \b");
                shell->line[shell->line_position] = 0;
            }

            continue;
        }

        /* handle end of line, break */
        if (ch == '\r' || ch == '\n')
        {
#ifdef FINSH_USING_HISTORY
            shell_push_history(shell);
#endif
            if (shell->echo_mode)
                tsc_kprintf("\n");
            msh_exec(shell->line, shell->line_position);

            tsc_kprintf(FINSH_PROMPT);
            memset(shell->line, 0, sizeof(shell->line));
            shell->line_curpos = shell->line_position = 0;
            continue;
        }

        /* it's a large line, discard it */
        if (shell->line_position >= FINSH_CMD_SIZE)
            shell->line_position = 0;

        /* normal character */
        if (shell->line_curpos < shell->line_position)
        {
            int i;

            memmove(&shell->line[shell->line_curpos + 1],
                       &shell->line[shell->line_curpos],
                       shell->line_position - shell->line_curpos);
            shell->line[shell->line_curpos] = ch;
            if (shell->echo_mode)
                tsc_kprintf("%s", &shell->line[shell->line_curpos]);

            /* move the cursor to new position */
            for (i = shell->line_curpos; i < shell->line_position; i++)
                tsc_kprintf("\b");
        }
        else
        {
            shell->line[shell->line_position] = ch;
            if (shell->echo_mode)
                tsc_kprintf("%c", ch);
        }

        ch = 0;
        shell->line_position ++;
        shell->line_curpos++;
        if (shell->line_position >= FINSH_CMD_SIZE)
        {
            /* clear command line */
            shell->line_position = 0;
            shell->line_curpos = 0;
        }
    } /* end of device read */
}

static void finsh_system_function_init(const void *begin, const void *end)
{
    _syscall_table_begin = (struct finsh_syscall *) begin;
    _syscall_table_end = (struct finsh_syscall *) end;
}

#if defined(__ICCARM__) || defined(__ICCRX__)               /* for IAR compiler */
#ifdef FINSH_USING_SYMTAB
    #pragma section="FSymTab"
#endif
#elif defined(__ADSPBLACKFIN__) /* for VisaulDSP++ Compiler*/
#ifdef FINSH_USING_SYMTAB
    extern "asm" int __fsymtab_start;
    extern "asm" int __fsymtab_end;
#endif
#elif defined(_MSC_VER)
#pragma section("FSymTab$a", read)
const char __fsym_begin_name[] = "__start";
const char __fsym_begin_desc[] = "begin of finsh";
__declspec(allocate("FSymTab$a")) const struct finsh_syscall __fsym_begin =
{
    __fsym_begin_name,
    __fsym_begin_desc,
    NULL
};

#pragma section("FSymTab$z", read)
const char __fsym_end_name[] = "__end";
const char __fsym_end_desc[] = "end of finsh";
__declspec(allocate("FSymTab$z")) const struct finsh_syscall __fsym_end =
{
    __fsym_end_name,
    __fsym_end_desc,
    NULL
};
#endif

/*
 * @ingroup group_finsh
 *
 * This function will initialize finsh shell
 */
int finsh_system_init(void)
{
	uint8_t result = TSC_EOK;

#ifdef FINSH_USING_SYMTAB
#ifdef __ARMCC_VERSION  /* ARM C Compiler */
    extern const int FSymTab$$Base;
    extern const int FSymTab$$Limit;
    finsh_system_function_init(&FSymTab$$Base, &FSymTab$$Limit);
#elif defined (__ICCARM__) || defined(__ICCRX__)      /* for IAR Compiler */
    finsh_system_function_init(__section_begin("FSymTab"),
                               __section_end("FSymTab"));
#elif defined (__GNUC__) || defined(__TI_COMPILER_VERSION__) || defined(__TASKING__)
    /* GNU GCC Compiler and TI CCS */
    extern const int __fsymtab_start;
    extern const int __fsymtab_end;
    finsh_system_function_init(&__fsymtab_start, &__fsymtab_end);
#elif defined(__ADSPBLACKFIN__) /* for VisualDSP++ Compiler */
    finsh_system_function_init(&__fsymtab_start, &__fsymtab_end);
#elif defined(_MSC_VER)
    unsigned int *ptr_begin, *ptr_end;

    if (shell)
    {
        tsc_kprintf("finsh shell already init.\n");
        return TSC_EOK;
    }

    ptr_begin = (unsigned int *)&__fsym_begin;
    ptr_begin += (sizeof(struct finsh_syscall) / sizeof(unsigned int));
    while (*ptr_begin == 0) ptr_begin ++;

    ptr_end = (unsigned int *) &__fsym_end;
    ptr_end --;
    while (*ptr_end == 0) ptr_end --;

    finsh_system_function_init(ptr_begin, ptr_end);
#endif
#endif

#ifdef TSC_USING_HEAP
    /* create or set shell structure */
    shell = (struct finsh_shell *)calloc(1, sizeof(struct finsh_shell));
    if (shell == NULL)
    {
        tsc_kprintf("no memory for shell\n");
        return -1;
    }
    const osThreadAttr_t shellTask_attributes = { .name = "shellTask",
    		.stack_size = FINSH_THREAD_STACK_SIZE, .priority = (osPriority_t) FINSH_THREAD_PRIORITY, };
#endif /* TSC_USING_HEAP */

    rx_console_sem = osSemaphoreNew(1, 1, &rx_console_sem_attributes);
    finsh_set_prompt_mode(1);

    if (result == TSC_EOK)
    	shellTaskHandle = osThreadNew(finsh_thread_entry, NULL, &shellTask_attributes);

    return 0;
}

//INIT_APP_EXPORT(finsh_system_init);

#endif /* TSC_USING_FINSH */
