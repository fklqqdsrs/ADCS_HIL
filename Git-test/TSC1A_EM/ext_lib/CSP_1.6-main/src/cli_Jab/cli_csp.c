#include "cli_csp.h"
//#include <readline/readline.h>
//#include <readline/history.h>
#include <include/csp/arch/csp_thread.h>
#include CONFIG_STM32_HAL_HDR

extern UART_HandleTypeDef CONFIG_CONSOLE_UART;
static SemaphoreHandle_t xSemaphore = NULL;
static CSP_DEFINE_TASK(task_command);

#define UART_RX_BUFFER_SIZE 256
static char *uart3RxData = NULL;

Command *command_list = NULL;
Command *current_context = NULL;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (uart3RxData != NULL && Size < UART_RX_BUFFER_SIZE) {
		uart3RxData[Size] = '\0';
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		HAL_UARTEx_ReceiveToIdle_IT(&CONFIG_CONSOLE_UART,(uint8_t *) uart3RxData, UART_RX_BUFFER_SIZE);
	}
}

uint8_t init_command_via_uart() {
	uart3RxData = (char*) malloc(UART_RX_BUFFER_SIZE);
	if (uart3RxData == NULL) {
		printf("Memory allocation failed for uart3RxData\n");
	}
	HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_IT(&CONFIG_CONSOLE_UART, (uint8_t *)uart3RxData, sizeof(uart3RxData));
	uint8_t ret = csp_thread_create(task_command, "CLI_CSP", 3072, NULL, 0, NULL);
	xSemaphore = xSemaphoreCreateBinary();
	if (xSemaphore == NULL) {
		printf("Failed to create semaphore\r\n");
		return CSP_ERR_NOMEM;
	}
	if (status == HAL_OK && ret == CSP_ERR_NONE) {
		// printf("\nInitializing command via UART...\r\n");
		return CSP_ERR_NONE;
	} else {
		printf("Initialization error, please check\r\n");
		printf("Failed to start server task, error: %d\r\n", ret);
		return ret;
	}
}

void display_prompt() {
    if (current_context) {
        printf("\033[33mTSC1> %s>\033[0m ", current_context->name);
    } else {
        printf("\033[33mTSC1> \033[0m");
    }
    fflush(stdout);
}

CSP_DEFINE_TASK(task_command) {
	add_function_basic();

	while (1) {

		 display_prompt();

        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            if (strlen(uart3RxData) > 0) {
                uart3RxData[strcspn(uart3RxData, "\n")] = '\0';

                if (current_context) {
                    if (strcmp(uart3RxData, "exit") == 0 || strcmp(uart3RxData, "cd ..") == 0 || strcmp(uart3RxData, "cd..") == 0) {
                        current_context = NULL;
                    } else if (strcmp(uart3RxData, "pwd") == 0) {
                        printf("%s\n", current_context->name);
                    } else {
                        char *args = strchr(uart3RxData, ' ');
                        if (args)
                            *args++ = '\0';

                        Command *subcmd = find_command(current_context->subcommands, uart3RxData);
                        if (subcmd) {
                            subcmd->function(args);
                        } else {
                            printf("Unknown subcommand: %s\n", uart3RxData);
                        }
                    }
                } else {
                    char *args = strchr(uart3RxData, ' ');
                    if (args)
                        *args++ = '\0';

                    Command *cmd = find_command(command_list, uart3RxData);
                    if (cmd) {
                        if (cmd->type == FUNCTION_COMMAND) {
                            cmd->function(args);
                        } else if (cmd->type == SUBCOMMAND_COMMAND) {
                            current_context = cmd;
                        }
                    } else {
                        printf("Unknown command: %s\n", uart3RxData);
                    }
                }

                memset(uart3RxData, 0, UART_RX_BUFFER_SIZE);
            }
        }
		csp_sleep_ms(1);
	}
}

// Function to safely allocate memory
void* safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

// Function to add a function command
void add_function_command(Command **list, const char *name, CommandFunction function) {
	Command *cmd = (Command*) safe_malloc(sizeof(Command));
	cmd->name = strdup(name);
	cmd->type = FUNCTION_COMMAND;
	cmd->function = function;
	cmd->subcommands = NULL;
	cmd->next = *list;
	*list = cmd;
}

// Function to add a subcommand
void add_subcommand(Command *parent, const char *name, CommandFunction function) {
	if (parent->type != SUBCOMMAND_COMMAND) {
		parent->type = SUBCOMMAND_COMMAND;
	}
	add_function_command(&parent->subcommands, name, function);
}

// Function to find a command by name
Command* find_command(Command *list, const char *name) {
	Command *cmd = list;
	while (cmd != NULL) {
		if (strcmp(cmd->name, name) == 0) {
			return cmd;
		}
		cmd = cmd->next;
	}
	return NULL;
}

void list_subcommands(Command *parent) {
	if (parent == NULL || parent->subcommands == NULL) {
		printf("No subcommands available for subsystem: %s\n", parent ? parent->name : "Unknown");
		return;
	}

	Command *cmd = parent->subcommands;
	while (cmd != NULL) {
		printf(" - %s\n", cmd->name);
		cmd = cmd->next;
	}
}

void list_subcommands_command(const char *args) {
	if (args == NULL || strlen(args) == 0) {
		Command *cmd = command_list;
		printf("Available Subsystems:\n");
		while (cmd != NULL) {
			printf(" - %s\n", cmd->name);
			cmd = cmd->next;
		}
		return;
	}

	Command *subsystem = find_command(command_list, args);
	if (subsystem) {
		list_subcommands(subsystem);
	} else {
		printf("Unknown subsystem: %s\n", args);
	}
}

void list_subcommands_sub(const char *args) {
	if (current_context == NULL) {
		printf("Cannot select subsystem when in subsystem.\n");
		return;
	}
	list_subcommands(current_context);
}

void Command_Line_Interface_TSC1() {
	char prompt[256];

	while (1) {

		if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
			if (current_context) {
				snprintf(prompt, sizeof(prompt), "\033[33mTSC1> %s>\033[0m ", current_context->name);
			} else {
				snprintf(prompt, sizeof(prompt), "\033[33mTSC1> \033[0m");
			}
			printf("%s", prompt);
			if (strlen(uart3RxData) > 0) {
				uart3RxData[strcspn(uart3RxData, "\n")] = '\0';

				if (current_context) {
					if (strcmp(uart3RxData, "exit") == 0 || strcmp(uart3RxData, "cd ..") == 0 || strcmp(uart3RxData, "cd..") == 0) {
						current_context = NULL;
					} else if (strcmp(uart3RxData, "pwd") == 0) {
						printf("%s\n", current_context->name);
					} else {
						char *args = strchr(uart3RxData, ' ');
						if (args)
							*args++ = '\0';

						Command *subcmd = find_command(current_context->subcommands, uart3RxData);
						if (subcmd) {
							subcmd->function(args);
						} else {
							printf("Unknown subcommand: %s\n", uart3RxData);
						}
					}
				} else {
					char *args = strchr(uart3RxData, ' ');
					if (args)
						*args++ = '\0';

					Command *cmd = find_command(command_list, uart3RxData);
					if (cmd) {
						if (cmd->type == FUNCTION_COMMAND) {
							cmd->function(args);
						} else if (cmd->type == SUBCOMMAND_COMMAND) {
							current_context = cmd;
						}
					} else {
						printf("Unknown command: %s\n", uart3RxData);
					}
				}
				memset(uart3RxData, 0, UART_RX_BUFFER_SIZE);
			}
		}
	}
}

char* trim_whitespace(char *str) {
	char *end;
	while (isspace((unsigned char )*str))
		str++;
	if (*str == 0)
		return str;
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char )*end))
		end--;
	*(end + 1) = 0;
	return str;
}

void add_csp_commands() {
	Command *csp = (Command*) safe_malloc(sizeof(Command));
	csp->name = strdup("csp");
	csp->type = SUBCOMMAND_COMMAND;
	csp->subcommands = NULL;
	csp->next = command_list;
	command_list = csp;

	add_subcommand(csp, "help", help_command_csp);
	add_subcommand(csp, "list", list_subcommands_sub);
	add_subcommand(csp, "rtable", csp_print_rtable_command);
	add_subcommand(csp, "interface", csp_print_interface_command);
	add_subcommand(csp, "conn_table", csp_print_connection_table_command);
	add_subcommand(csp, "uptime", csp_uptime_command);
	add_subcommand(csp, "buf_free", csp_buf_free_command);
	add_subcommand(csp, "memfree", csp_memfree_command);
	add_subcommand(csp, "ps", csp_ps_command);
	add_subcommand(csp, "shutdown", csp_shutdown_command);
	add_subcommand(csp, "reboot", csp_reboot_command);
	add_subcommand(csp, "ping_noreply", csp_ping_noreply_command);
	add_subcommand(csp, "ping_all", csp_ping_all_command);
	add_subcommand(csp, "ping", csp_ping_command);
}

void add_adcs_commands() {
	Command *adcs = (Command*) safe_malloc(sizeof(Command));
	adcs->name = strdup("adcs");
	adcs->type = SUBCOMMAND_COMMAND;
	adcs->subcommands = NULL;
	adcs->next = command_list;
	command_list = adcs;

	add_subcommand(adcs, "help", help_command_adcs);
	add_subcommand(adcs, "list", list_subcommands_sub);
	add_subcommand(adcs, "get_mtm", csp_get_mtm_command);
	add_subcommand(adcs, "get_gps", csp_get_gps_command);
	add_subcommand(adcs, "get_imu", csp_get_imu_command);
	add_subcommand(adcs, "get_view", csp_get_rpm_view_command);
	add_subcommand(adcs, "set_view", csp_set_rpm_view_command);
}


// Adding CSP commands to the structure
void add_dhu_commands()
{
    Command *dhu_c = (Command *)safe_malloc(sizeof(Command));
    dhu_c->name = strdup("dhu");
    dhu_c->type = SUBCOMMAND_COMMAND;
    dhu_c->subcommands = NULL;
    dhu_c->next = command_list;
    command_list = dhu_c;

    add_subcommand(dhu_c, "help", help_command_dhu);
    add_subcommand(dhu_c, "list", list_subcommands_sub);
    add_subcommand(dhu_c, "get_image", list_subcommands_sub);
}

void add_chicken_commands()
{
    Command *chicken_c = (Command *)safe_malloc(sizeof(Command));
    chicken_c->name = strdup("chicken");
    chicken_c->type = SUBCOMMAND_COMMAND;
    chicken_c->subcommands = NULL;
    chicken_c->next = command_list;
    command_list = chicken_c;

    add_subcommand(chicken_c, "help", help_command_chicken);
    add_subcommand(chicken_c, "list", list_subcommands_sub);
    add_subcommand(chicken_c, "request_hex", request_chicken_hex_command);
    add_subcommand(chicken_c, "send_hex", send_chicken_hex_command);
    add_subcommand(chicken_c, "request", request_chicken_command);
    add_subcommand(chicken_c, "send", send_chicken_command);
}

void add_ssdv_commands() {
	Command *ssdv_c = (Command*) safe_malloc(sizeof(Command));
	ssdv_c->name = strdup("ssdv");
	ssdv_c->type = SUBCOMMAND_COMMAND;
	ssdv_c->subcommands = NULL;
	ssdv_c->next = command_list;
	command_list = ssdv_c;

	add_subcommand(ssdv_c, "help", help_command_ssdv);
	add_subcommand(ssdv_c, "list", list_subcommands_sub);
	add_subcommand(ssdv_c, "shutdown_ssdv", csp_shutdown_ssdv_command);
	add_subcommand(ssdv_c, "reset_ssdv", csp_reset_ssdv_command);
	add_subcommand(ssdv_c, "check_ssdv", csp_check_ssdv_command);
	add_subcommand(ssdv_c, "set_ssdv", csp_set_ssdv_command);
}

void add_timesync_commands() {
	Command *timesync_c = (Command*) safe_malloc(sizeof(Command));
	timesync_c->name = strdup("timesync");
	timesync_c->type = SUBCOMMAND_COMMAND;
	timesync_c->subcommands = NULL;
	timesync_c->next = command_list;
	command_list = timesync_c;

	add_subcommand(timesync_c, "help", help_command_time_sync);
	add_subcommand(timesync_c, "list", list_subcommands_sub);
	add_subcommand(timesync_c, "get_time_sync", csp_get_timesync_command);
	add_subcommand(timesync_c, "set_time_broadcast", csp_timesync_broadcast_command);
	add_subcommand(timesync_c, "set_time_sync", csp_set_timesync_command);
}

// Adding CSP commands to the structure
void add_register_commands() {
	Command *register_c = (Command*) safe_malloc(sizeof(Command));
	register_c->name = strdup("register");
	register_c->type = SUBCOMMAND_COMMAND;
	register_c->subcommands = NULL;
	register_c->next = command_list;
	command_list = register_c;

	add_subcommand(register_c, "help", help_command_register);
	add_subcommand(register_c, "list", list_subcommands_sub);
	add_subcommand(register_c, "get_reg", csp_get_register_command);
	add_subcommand(register_c, "set_reg", csp_set_register_command);
	add_subcommand(register_c, "add_reg", csp_add_register_command);
}

void add_settings_commands() {
	Command *settings_c = (Command*) safe_malloc(sizeof(Command));
	settings_c->name = strdup("settings");
	settings_c->type = SUBCOMMAND_COMMAND;
	settings_c->subcommands = NULL;
	settings_c->next = command_list;
	command_list = settings_c;

	add_subcommand(settings_c, "help", help_command_routing);
	add_subcommand(settings_c, "list", list_subcommands_sub);
	add_subcommand(settings_c, "reset_rtable", csp_reset_rtable_command);
	add_subcommand(settings_c, "get_rtable", csp_get_rtable_command);
	add_subcommand(settings_c, "set_rtable", csp_set_rtable_command);
	add_subcommand(settings_c, "reset_address", csp_reset_address_command);
	add_subcommand(settings_c, "get_address", csp_get_address_command);
	add_subcommand(settings_c, "set_address", csp_set_address_command);
}

void add_script_commands() {
	Command *script_c = (Command*) safe_malloc(sizeof(Command));
	script_c->name = strdup("automate");
	script_c->type = SUBCOMMAND_COMMAND;
	script_c->subcommands = NULL;
	script_c->next = command_list;
	command_list = script_c;

	initiate_command_script();

	add_subcommand(script_c, "help", help_command_script);
	add_subcommand(script_c, "list", list_subcommands_sub);
	add_subcommand(script_c, "list_command", list_commands_print);
	add_subcommand(script_c, "open_notepad", open_with_notepad);
	add_subcommand(script_c, "open_nano", open_with_nano);
	add_subcommand(script_c, "delete_script", delete_script);
	add_subcommand(script_c, "clean_script", clean_script);
	add_subcommand(script_c, "save_script", save_script_command);
	add_subcommand(script_c, "run_script_loop", load_script_loop);
	add_subcommand(script_c, "run_script", load_script);
}

void add_function_basic() {
    display_initialising_with_color("Welcome to the TSC-1 CLI.");
    printf("You can use the help command to see the details of the available systems or list to view other subsystems.\r\n");
    add_function_command(&command_list, "other", print_details);
    add_function_command(&command_list, "over_help", help_command);
    add_function_command(&command_list, "help", display_help);
    add_function_command(&command_list, "list", list_subcommands_command);
    add_settings_commands();
    add_chicken_commands();
    add_script_commands();
    add_register_commands();
    add_timesync_commands();
    add_ssdv_commands();
    add_adcs_commands();
    add_dhu_commands();
    add_csp_commands();
}
