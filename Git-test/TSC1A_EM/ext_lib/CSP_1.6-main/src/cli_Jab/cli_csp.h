#ifndef CLI_CSP_H
#define CLI_CSP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include <include/csp/csp.h>
#include <src/func/func_adcs/Func_ADCS.h>
#include <src/func/func_register/csp_register.h>
#include <src/func/func_register/csp_register_service.h>
#include <src/func/func_ssdv/func_SSDV.h>
#include <src/func/func_timesync/Func_Time_sync.h>
#include <src/func/func_chicken/Func_Chicken.h>
#include "cli_rtable.h"

#include "script_management.h"
#include "print_list.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined STM32F103xB // STM32F103C8T6 (used by Bluepill)

    #define CONFIG_STM32_HAL_HDR "stm32f1xx_hal.h"
    #define CONFIG_CONSOLE_UART huart2
    #define CONFIG_GPIO_TYPE 1

#elif defined STM32F401xE

    #define CONFIG_STM32_HAL_HDR "stm32f4xx_hal.h"
    #define CONFIG_CONSOLE_UART huart2

#elif defined STM32L452xx

    #define CONFIG_STM32_HAL_HDR "stm32l4xx_hal.h"
    #define CONFIG_CONSOLE_UART huart2

#elif defined STM32L496xx

#define CONFIG_STM32_HAL_HDR "stm32l4xx_hal.h"
#define CONFIG_CONSOLE_UART hlpuart1

#elif defined STM32U575xx

    #define CONFIG_STM32_HAL_HDR "stm32u5xx_hal.h"
    #define CONFIG_CONSOLE_UART huart1

#elif defined STM32F722xx

  #define CONFIG_STM32_HAL_HDR "stm32f7xx_hal.h"
    #define CONFIG_CONSOLE_UART huart3
#else
    #error Unknown processor
#endif

typedef enum
{
    FUNCTION_COMMAND,
    SUBCOMMAND_COMMAND
} CommandType;

typedef void (*CommandFunction)(const char *args);

typedef struct Command
{
    char *name;
    CommandType type;
    CommandFunction function;
    struct Command *subcommands;
    struct Command *next;
} Command;

extern Command *command_list;
extern Command *current_context;


uint8_t init_command_via_uart();

void *safe_malloc(size_t size);
void add_function_command(Command **list, const char *name, CommandFunction function);
void add_subcommand(Command *parent, const char *name, CommandFunction function);
Command *find_command(Command *list, const char *name);
void execute_subcommand(Command *parent);

void execute_command(const char *input);
void Command_Line_Interface_TSC1 ();
char *trim_whitespace(char *str);
void add_function_basic();

//add sub sytems functions
void add_csp_commands();
void add_adcs_commands();
void add_ssdv_commands();
void add_register_commands();
void add_timesync_commands();
void add_settings_commands();
void add_script_commands();

// functions_csp
void csp_ping_command(const char *args);
void csp_ping_all_command(const char *args);
void csp_ping_noreply_command(const char *args);
void csp_reboot_command(const char *args);
void csp_shutdown_command(const char *args);
void csp_ps_command(const char *args);
void csp_memfree_command(const char *args);
void csp_buf_free_command(const char *args);
void csp_uptime_command(const char *args);
void csp_print_connection_table_command(const char *args);
void csp_print_interface_command(const char *args);
void csp_print_rtable_command(const char *args);

// functions_adcs
void csp_set_rpm_view_command(const char *args);
void csp_get_rpm_view_command(const char *args);
void csp_get_imu_command(const char *args);
void csp_get_gps_command(const char *args);
void csp_get_mtm_command(const char *args);

// function_registers
void csp_add_register_command(const char *args);
void csp_set_register_command(const char *args);
void csp_get_register_command(const char *args);

// function ssdv
void csp_set_ssdv_command(const char *args);
void csp_reset_ssdv_command(const char *args);
void csp_check_ssdv_command(const char *args);
void csp_shutdown_ssdv_command(const char *args);

//function time sync
void csp_set_timesync_command(const char *args);
void csp_timesync_broadcast_command(const char *args);
void csp_get_timesync_command(const char *args);

//func chicken
void send_chicken_command(const char *args);
void request_chicken_command(const char *args);
void send_chicken_hex_command(const char *args);
void request_chicken_hex_command(const char *args);


#ifdef __cplusplus
}
#endif
#endif
