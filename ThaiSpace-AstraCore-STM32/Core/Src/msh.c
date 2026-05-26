/*
 * msh.c
 *
 *  Created on: 25 June 2025
 *      Author: Pongpot
 */

#include <cmsis_os.h>
#include "kservice.h"

/* Custom assert implementation for msh */
#ifdef assert
#undef assert
#endif

#ifdef TSC_DEBUG
#define assert(ex) if (!(ex)) tsc_assert_handler(#ex, __FUNCTION__, __LINE__)
#else
#define assert(ex)
#endif

#ifdef TSC_USING_FINSH

#ifndef FINSH_ARG_MAX
#define FINSH_ARG_MAX    8
#endif /* FINSH_ARG_MAX */

#include "msh.h"

typedef int (*cmd_function_t)(int argc, char **argv);

int msh_help(int argc, char **argv)
{
    tsc_kprintf("TSC-1 shell commands:\n");
    {
        struct finsh_syscall *index;

        for (index = _syscall_table_begin;
                index < _syscall_table_end;
                FINSH_NEXT_SYSCALL(index))
        {
#if defined(FINSH_USING_DESCRIPTION) && defined(FINSH_USING_SYMTAB)
            tsc_kprintf("%-16s - %s\n", index->name, index->desc);
#else
            tsc_kprintf("%s ", index->name);
#endif
        }
    }
    tsc_kprintf("\n");

    return 0;
}
TSC_CMD_EXPORT(msh_help, help, Shell help.);

#ifdef MSH_USING_BUILT_IN_COMMANDS
int cmd_ps(int argc, char **argv)
{
    extern long list_thread(void);
    extern int list_module(void);

    list_thread();

    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_ps, ps, List threads in the system.);

#ifdef TSC_USING_HEAP
int cmd_free(int argc, char **argv)
{
    size_t total = 0, used = 0, max_used = 0;

    tsc_memory_info(&total, &used, &max_used);
    tsc_kprintf("total   : %d\n", total);
    tsc_kprintf("used    : %d\n", used);
    tsc_kprintf("maximum : %d\n", max_used);
    return 0;
}
MSH_CMD_EXPORT_ALIAS(cmd_free, free, Show the memory usage in the system.);
#endif /* TSC_USING_HEAP */
#endif /* MSH_USING_BUILT_IN_COMMANDS */

static int msh_split(char *cmd, size_t length, char *argv[FINSH_ARG_MAX])
{
    char *ptr;
    size_t position;
    size_t argc;
    size_t i;

    ptr = cmd;
    position = 0;
    argc = 0;

    while (position < length)
    {
        /* strip bank and tab */
        while ((*ptr == ' ' || *ptr == '\t') && position < length)
        {
            *ptr = '\0';
            ptr ++;
            position ++;
        }

        if (argc >= FINSH_ARG_MAX)
        {
            tsc_kprintf("Too many args ! We only Use:\n");
            for (i = 0; i < argc; i++)
            {
                tsc_kprintf("%s ", argv[i]);
            }
            tsc_kprintf("\n");
            break;
        }

        if (position >= length) break;

        /* handle string */
        if (*ptr == '"')
        {
            ptr ++;
            position ++;
            argv[argc] = ptr;
            argc ++;

            /* skip this string */
            while (*ptr != '"' && position < length)
            {
                if (*ptr == '\\')
                {
                    if (*(ptr + 1) == '"')
                    {
                        ptr ++;
                        position ++;
                    }
                }
                ptr ++;
                position ++;
            }
            if (position >= length) break;

            /* skip '"' */
            *ptr = '\0';
            ptr ++;
            position ++;
        }
        else
        {
            argv[argc] = ptr;
            argc ++;
            while ((*ptr != ' ' && *ptr != '\t') && position < length)
            {
                ptr ++;
                position ++;
            }
            if (position >= length) break;
        }
    }

    return argc;
}

static cmd_function_t msh_get_cmd(char *cmd, int size)
{
    struct finsh_syscall *index;
    cmd_function_t cmd_func = NULL;

    for (index = _syscall_table_begin;
            index < _syscall_table_end;
            FINSH_NEXT_SYSCALL(index))
    {
        if (strncmp(index->name, cmd, size) == 0 &&
                index->name[size] == '\0')
        {
            cmd_func = (cmd_function_t)index->func;
            break;
        }
    }

    return cmd_func;
}

static int _msh_exec_cmd(char *cmd, size_t length, int *retp)
{
    int argc;
    size_t cmd0_size = 0;
    cmd_function_t cmd_func;
    char *argv[FINSH_ARG_MAX];

    assert(cmd);
    assert(retp);

    /* find the size of first command */
    while ((cmd[cmd0_size] != ' ' && cmd[cmd0_size] != '\t') && cmd0_size < length)
        cmd0_size ++;
    if (cmd0_size == 0)
        return -TSC_ERROR;

    cmd_func = msh_get_cmd(cmd, cmd0_size);
    if (cmd_func == NULL)
        return -TSC_ERROR;

    /* split arguments */
    memset(argv, 0x00, sizeof(argv));
    argc = msh_split(cmd, length, argv);
    if (argc == 0)
        return -TSC_ERROR;

    /* exec this command */
    *retp = cmd_func(argc, argv);
    return 0;
}

int msh_exec(char *cmd, size_t length)
{
    int cmd_ret;

    /* strim the beginning of command */
    while ((length > 0) && (*cmd  == ' ' || *cmd == '\t'))
    {
        cmd++;
        length--;
    }

    if (length == 0)
        return 0;

    /* Exec sequence:
     * 1. built-in command
     * 2. module(if enabled)
     */
    if (_msh_exec_cmd(cmd, length, &cmd_ret) == 0)
    {
        return cmd_ret;
    }

    /* truncate the cmd at the first space. */
    {
        char *tcmd;
        tcmd = cmd;
        while (*tcmd != ' ' && *tcmd != '\0')
        {
            tcmd++;
        }
        *tcmd = '\0';
    }
    tsc_kprintf("%s: command not found.\n", cmd);
    return -1;
}

static int str_common(const char *str1, const char *str2)
{
    const char *str = str1;

    while ((*str != 0) && (*str2 != 0) && (*str == *str2))
    {
        str ++;
        str2 ++;
    }

    return (str - str1);
}

void msh_auto_complete(char *prefix)
{
    int length, min_length;
    const char *name_ptr, *cmd_name;
    struct finsh_syscall *index;

    min_length = 0;
    name_ptr = NULL;

    if (*prefix == '\0')
    {
        msh_help(0, NULL);
        return;
    }

    /* checks in internal command */
    {
        for (index = _syscall_table_begin; index < _syscall_table_end; FINSH_NEXT_SYSCALL(index))
        {
            /* skip finsh shell function */
            cmd_name = (const char *) index->name;
            if (strncmp(prefix, cmd_name, strlen(prefix)) == 0)
            {
                if (min_length == 0)
                {
                    /* set name_ptr */
                    name_ptr = cmd_name;
                    /* set initial length */
                    min_length = strlen(name_ptr);
                }

                length = str_common(name_ptr, cmd_name);
                if (length < min_length)
                    min_length = length;

                tsc_kprintf("%s\n", cmd_name);
            }
        }
    }

    /* auto complete string */
    if (name_ptr != NULL)
    {
        strncpy(prefix, name_ptr, min_length);
    }

    return ;
}
#endif /* TSC_USING_FINSH */
