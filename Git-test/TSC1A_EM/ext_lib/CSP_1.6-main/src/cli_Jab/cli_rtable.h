#ifndef CLI_RTABLE_H
#define CLI_RTABLE_H

#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/csp/csp.h>
#ifdef __cplusplus
extern "C" {
#endif
//file cli_rtable.c
int set_address_csp(const char *address);
int set_routing_csp(const char *routing);
int read_address(uint8_t *address);
int read_routing_table(char **rtable);
int reset_address();
int reset_rtable();

void csp_set_address_command(const char *args);
void csp_get_address_command( const char *args);
void csp_reset_address_command(const char *args);

void csp_set_rtable_command(const char *args);
void csp_get_rtable_command( const char *args);
void csp_reset_rtable_command(const char *args);
#ifdef __cplusplus
}
#endif
#endif
