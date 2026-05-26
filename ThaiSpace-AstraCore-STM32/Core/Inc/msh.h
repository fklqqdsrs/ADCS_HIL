/*
 * msh.h
 *
 *  Created on: 25 June 2025
 *      Author: Pongpot
 */

#ifndef __M_SHELL__
#define __M_SHELL__

#include <stm32l4xx_hal.h>

int msh_exec(char *cmd, size_t length);
void msh_auto_complete(char *prefix);

int msh_exec_module(const char *cmd_line, int size);
int msh_exec_script(const char *cmd_line, int size);

#endif
