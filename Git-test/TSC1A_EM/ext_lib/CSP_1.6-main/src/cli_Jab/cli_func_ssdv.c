#include "cli_csp.h"

// int set_ssdv_Parameter(uint8_t res , uint8_t quality , uint8_t camEnable ,uint32_t interval)
void csp_set_ssdv_command(const char *args) {
    uint8_t res;
    uint8_t quality;
    uint32_t camEnable;
    uint32_t interval;

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) res = (uint8_t)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) quality = (uint8_t)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) camEnable = (uint32_t)strtoul(token, NULL, 10);
    token = strtok(NULL, " ");
    if (token != NULL) interval = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        int8_t status = set_ssdv_Parameter(res, quality, camEnable, interval);
        printf("Running set_ssdv: res=%d, quality=%d, camEnable=%lu, interval=%lu : status=%d\n",
               res, quality, (unsigned long)camEnable, (unsigned long)interval, status);
    } else {
        printf("Usage: set_ssdv <res> <quality> <camEnable> <interval>\n");
    }

    free(args_copy);
}

void csp_reset_ssdv_command(const char *args)
{
  int8_t status = reset_ssdv_Parameter() ; 
  printf("reset_ssdv_command status:%d\n" ,status);
}
void csp_check_ssdv_command(const char *args)
{
  int8_t status = check_ssdv_Connection();
  printf("check ssdv respond time:%dms\n",status);
}
void csp_shutdown_ssdv_command(const char *args)
{
  printf("shutdown_ssdv_command\n");
   csp_shutdown(ADDRESS_SSDV);
}
