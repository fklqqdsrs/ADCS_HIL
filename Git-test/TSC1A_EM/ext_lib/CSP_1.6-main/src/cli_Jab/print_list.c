#include "print_list.h"

void display_help(const char *args)
{
    printf("\n------------------- Help Menu -------------------\n");

    printf("\n-csp\n");
    printf("  %-25s: %s\n", "ping", "Ping a target node");
    printf("  %-25s: %s\n", "ping_noreply", "Ping a target node without reply");
    printf("  %-25s: %s\n", "reboot", "Reboot a target node");
    printf("  %-25s: %s\n", "shutdown", "Shutdown a target node");
    printf("  %-25s: %s\n", "ps", "Get process list of a node");
    printf("  %-25s: %s\n", "memfree", "Get free memory of a node");
    printf("  %-25s: %s\n", "buf_free", "Get buffer free status");
    printf("  %-25s: %s\n", "uptime", "Get uptime of a node");
    printf("  %-25s: %s\n", "rtable", "Print routing table");
    printf("  %-25s: %s\n", "interface", "Print interface status");
    printf("  %-25s: %s\n", "conn_table", "Print connection table");

    printf("\n-adcs\n");
    printf("  %-25s: %s\n", "set_view", "Set RPM for reaction wheels");
    printf("  %-25s: %s\n", "get_view", "Get RPM of reaction wheels");
    printf("  %-25s: %s\n", "get_imu", "Get IMU data");
    printf("  %-25s: %s\n", "get_gps", "Get GPS data");
    printf("  %-25s: %s\n", "get_mtm", "Get MTM data");

    printf("\n-ssdv\n");
    printf("  %-25s: %s\n", "set_ssdv", "Set SSDV parameters");
    printf("  %-25s: %s\n", "reset_ssdv", "Reset SSDV parameters");
    printf("  %-25s: %s\n", "check_ssdv", "Check SSDV connection status");
    printf("  %-25s: %s\n", "shutdown_ssdv", "Shutdown SSDV");

    printf("\n--time sync\n");
    printf("  %-25s: %s\n", "set_time_sync", "Set time sync on a node");
    printf("  %-25s: %s\n", "get_time_sync", "Get time sync from a node");
    printf("  %-25s: %s\n", "set_time_broadcast", "Broadcast time sync to nodes");

    printf("\n-register\n");
    printf("  %-25s: %s\n", "add_reg", "Add a register to CSP");
    printf("  %-25s: %s\n", "set_reg", "Set a value in a CSP register");
    printf("  %-25s: %s\n", "get_reg", "Get a value from a CSP register");

    printf("\n-script\n");
    printf("  %-25s: %s\n", "run_script", "Run a script from file");
    printf("  %-25s: %s\n", "run_script_loop", "Run a script from file loop");
    printf("  %-25s: %s\n", "save_script", "Save a command to a script");
    printf("  %-25s: %s\n", "clean_script", "Clean a script file");
    printf("  %-25s: %s\n", "delete_script", "Delete a script file");
    printf("  %-25s: %s\n", "open_nano", "Open script with Nano editor");
    printf("  %-25s: %s\n", "open_notepad", "Open script with Notepad");

    printf("\n-settings\n");
    printf("  %-25s: %s\n", "set_address", "Set the address of CSP");
    printf("  %-25s: %s\n", "get_address", "Get the address of CSP");
    printf("  %-25s: %s\n", "reset_address", "Reset the address of CSP");
    printf("  %-25s: %s\n", "set_rtable", "Set routing table of CSP");
    printf("  %-25s: %s\n", "get_rtable", "Get a value from the routing table of CSP");
    printf("  %-25s: %s\n", "reset_rtable", "Reset the routing table of CSP"); // printf("  %-25s: %s\n", "help_command_settings", "Display help for settings commands");

    printf("\n-list\n");
    printf("  %-25s: %s\n", "list", "List all available subsystems");

    printf("\n-help\n");
    printf("  %-25s: %s\n", "help", "Display this help menu");

    printf("\n-------------------------------------------------\n");
}

void help_command(const char *args)
{
    printf("Available Commands:\n\n");
    printf("/run\n");
    printf("  --> CLIcsp\n");
    printf("--------> General CSP Functions <---------\n\n");
    help_command_csp(NULL);
    help_command_adcs(NULL);
    help_command_dhu(NULL);
    help_command_chicken(NULL);
    help_command_ssdv(NULL);
    help_command_register(NULL);
    help_command_time_sync(NULL);
    help_command_routing(NULL);
    help_command_script(NULL);
}

void help_command_csp(const char *args)
{
    printf("------------ CSP Command help ------------\n");
    printf("---------> function general csp <---------\n\n");

    // csp_ping
    printf("ping <node> <timeout> <size> <opts>\n");
    printf("   - Send a ping/echo packet.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n");
    printf("     <timeout>: Wait time in ms\n");
    printf("     <size>: Payload size in bytes\n");
    printf("     <opts>: Connection options\n\n");

    // csp_ping_noreply
    printf("ping_noreply <node>\n");
    printf("   - Send a ping without waiting for a reply.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n\n");

    // csp_ps
    printf("ps <node> <timeout>\n");
    printf("   - Request a process list (only FreeRTOS).\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // csp_memfree
    printf("memfree <node> <timeout>\n");
    printf("   - Request available free memory.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // csp_buf_free
    printf("buf_free <node> <timeout>\n");
    printf("   - Request and print available free buffers.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // csp_reboot
    printf("reboot <node>\n");
    printf("   - Reboot the subsystem.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n\n");

    // csp_shutdown
    printf("shutdown <node>\n");
    printf("   - Shutdown the subsystem.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n\n");

    // csp_uptime
    printf("uptime <node> <timeout>\n");
    printf("   - Request and print subsystem uptime.\n");
    printf("   - Parameters: \n");
    printf("     <node>: Subsystem address (0-31)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // rtable
    printf("rtable\n");
    printf("   - Request and print routing table.\n\n");

    // interface
    printf("interface\n");
    printf("   - Request and print interface.\n\n");

    // connect table
    printf("conn_table\n");
    printf("   - Request and print connect table.\n\n");
}

void help_command_adcs(const char *args)
{
    // ADCS functions
    printf("---------- ADCS Command Help -------------\n");
    printf("--------> function adcs csp <-------------\n\n");

    printf("set_view <view> <rpm_view> <timeout>\n");
    printf("   - Set the view and RPM for reaction wheels.\n");
    printf("   - Parameters: \n");
    printf("     <view>: View number (1-4)\n");
    printf("     <rpm_view>: RPM value (0-3000000)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // get_ms_rpm_react_view
    printf("get_view <view> <timeout>\n");
    printf("   - Retrieve RPM data for a specific view.\n");
    printf("   - Parameters: \n");
    printf("     <view>: View number (1-4)\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // get_imu
    printf("get_imu <imu_data> <timeout>\n");
    printf("   - Retrieve IMU data.\n");
    printf("   - Parameters: \n");
    printf("     <imu_data>: Pointer to IMU data structure\n");
    printf("     <timeout>: Wait time in ms\n\n");

    // get_gps
    printf("get_gps <gps_data_s> <timeout>\n");
    printf("   - Retrieve GPS data.\n");
    printf("   - Parameters:\n");
    printf("     <gps_data_s>: Pointer to GPS data structure\n");
    printf("     <timeout>: Time in ms to wait for a response\n");
    printf("   - Return: Status of the operation\n\n");

    // get_mtm
    printf("get_mtm <axial_mtm_s> <timeout>\n");
    printf("   - Retrieve MTM (Magnetometer) data.\n");
    printf("   - Parameters:\n");
    printf("     <axial_mtm_s>: Pointer to axial MTM data structure\n");
    printf("     <timeout>: Time in ms to wait for a response\n");
    printf("   - Return: Status of the operation\n\n");
}

void help_command_dhu(const char *args)
{
    printf("-------------------- DHU Command Help --------------------\n");
    printf("-------------------> function dhu csp <-------------------\n\n");
}

void help_command_chicken(const char *args)
{
    // Chicken Command Help
    printf("-------------------- Chicken Command Help --------------------\n");
    printf("-------------------> function chicken csp <-------------------\n\n");

    // Help for send_chicken_command
    printf("send <node> <prio> <dport> <timeout> <opts> <data>\n");
    printf("   - Send data to a specified destination node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node ID of the destination (0-31)\n");
    printf("     <prio>: Priority of the CSP packet (0-3)\n");
    printf("     <dport>: Destination port (0-63)\n");
    printf("     <timeout>: Timeout in milliseconds\n");
    printf("     <opts>: CSP options\n");
    printf("     <data>: Data string to be sent (up to 255 characters)\n\n");

    // Help for request_chicken_command
    printf("request <node> <prio> <dport> <timeout> <opts> <send_data>\n");
    printf("   - Send a request and wait for a response from the specified node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node ID of the destination (0-31)\n");
    printf("     <prio>: Priority of the CSP packet (0-3)\n");
    printf("     <dport>: Destination port (0-63)\n");
    printf("     <timeout>: Timeout in milliseconds\n");
    printf("     <opts>: CSP options\n");
    printf("     <send_data>: Data string to be sent (up to 255 characters)\n\n");

    // Help for send_hex_command
    printf("send_hex <node> <prio> <dport> <timeout> <opts> <hex_data>\n");
    printf("   - Send data in hexadecimal format to a specified destination node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node ID of the destination (0-31)\n");
    printf("     <prio>: Priority of the CSP packet (0-3)\n");
    printf("     <dport>: Destination port (0-63)\n");
    printf("     <timeout>: Timeout in milliseconds\n");
    printf("     <opts>: CSP options\n");
    printf("     <hex_data>: Hexadecimal string of data to be sent (e.g., '1A2B3C')\n");
    printf("   - The hexadecimal data should be a series of byte values (up to 255 bytes).\n");
    printf("   - Each byte should be represented by two hexadecimal characters.\n\n");

    // Help for request_chicken_hex_command
    printf("request_hex <node> <prio> <dport> <timeout> <opts> <hex_send_data>\n");
    printf("   - Send a request in hexadecimal format and wait for a response from the specified node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node ID of the destination (0-31)\n");
    printf("     <prio>: Priority of the CSP packet (0-3)\n");
    printf("     <dport>: Destination port (0-63)\n");
    printf("     <timeout>: Timeout in milliseconds\n");
    printf("     <opts>: CSP options\n");
    printf("     <hex_send_data>: Hexadecimal string of data to be sent (e.g., '4D5E6F')\n");
    printf("   - The send_data should be a series of byte values (up to 255 bytes).\n");
    printf("   - Each byte should be represented by two hexadecimal characters.\n");
    printf("   - The function will wait for a response and print the received data in hexadecimal format.\n\n");
    printf("-------------------------------------------------------------\n");
}

void help_command_ssdv(const char *args)
{
    // SSDV functions
    printf("----------- SSDV Command Help ------------\n");
    printf("---------> function ssdv csp <-------------\n\n");

    // set_ssdv_Parameter
    printf("set_ssdv <res> <quality> <camEnable> <interval>\n");
    printf("   - Set parameters for SSDV camera.\n");
    printf("   - Parameters: \n");
    printf("     <res>: Resolution setting (0-32)\n");
    printf("     <quality>: Quality setting (0-32)\n");
    printf("     <camEnable>: Enable/disable camera (0 or 1)\n");
    printf("     <interval>: Interval between captures in SEC\n\n");

    // check_ssdv_Connection
    printf("check_ssdv\n");
    printf("   - Check connection status for SSDV.\n\n");

    // reset_ssdv_Parameter
    printf("reset_ssdv\n");
    printf("   - Reset SSDV parameters to default.\n\n");

    printf("shutdown_ssdv\n");
    printf("   - Shutdown SSDV parameters to default.\n\n");
}

void help_command_time_sync(const char *args)
{
    // Time Sync Functions
    printf("---------- Time Sync Command Help ----------\n");
    printf("--------> function time sync csp <-----------\n\n");

    // csp_set_time_sync
    printf("set_time_sync <node> <timesync> <timeout>\n");
    printf("   - Synchronize the time of a specific node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node to synchronize time for (0-32)\n");
    printf("     <timesync>: Time to set in milliseconds\n");
    printf("     <timeout>: Time in ms to wait for a response\n");
    printf("   - Return: Status of the operation\n\n");

    // csp_get_time_sync
    printf("get_time_sync <node> <timeout> <received_time>\n");
    printf("   - Retrieve time synchronization data from a node.\n");
    printf("   - Parameters:\n");
    printf("     <node>: Node to retrieve time sync from (0-32)\n");
    printf("     <timeout>: Time in ms to wait for a response\n");
    printf("     <received_time>: Pointer to store the received time\n");
    printf("   - Return: Status of the operation\n\n");

    // csp_time_sync_broadcast
    printf("time_sync_broadcast <timesync> <timeout>\n");
    printf("   - Broadcast time synchronization data to all nodes.\n");
    printf("   - Parameters:\n");
    printf("     <timesync>: Time to broadcast in milliseconds\n");
    printf("     <timeout>: Time in ms to wait for responses\n");
    printf("   - Return: Status of the operation\n\n");
}

void help_command_register(const char *args)
{
    // Register Functions
    printf("--------- Registers command Help ----------\n");
    printf("--------> function register csp <----------\n\n");

    // add_register_csp
    printf("add_reg <device_id> <register_id> <sub_register> <type_register> <value> <access_mode> <timeout>\n");
    printf("   - Add a register entry for a device.\n");
    printf("   - Parameters:\n");
    printf("     <device_id>: Device ID to add register for\n");
    printf("     <register_id>: Register ID to add\n");
    printf("     <sub_register>: Sub-register to target\n");
    printf("     <type_register>: Type of register data (e.g., uint8_t)\n");
    printf("     <value>: Pointer to data to be written\n");
    printf("     <access_mode>: Access mode for the register\n");
    printf("     <timeout>: Time in ms to wait for a response\n\n");

    // set_register_csp
    printf("set_reg <device_id> <register_id> <sub_register> <type_register> <value> <timeout>\n");
    printf("   - Set a value for a specific register.\n");
    printf("   - Parameters:\n");
    printf("     <device_id>: Device ID to set register for\n");
    printf("     <register_id>: Register ID to set\n");
    printf("     <sub_register>: Sub-register to target\n");
    printf("     <type_register>: Type of register data (e.g., uint8_t)\n");
    printf("     <value>: Pointer to data to be written\n");
    printf("     <timeout>: Time in ms to wait for a response\n\n");

    // get_register_csp
    printf("get_reg <device_id> <register_id> <sub_register> <type_register> <timeout>\n");
    printf("   - Retrieve the value of a specific register.\n");
    printf("   - Parameters:\n");
    printf("     <device_id>: Device ID to get register value from\n");
    printf("     <register_id>: Register ID to retrieve\n");
    printf("     <sub_register>: Sub-register to target\n");
    printf("     <type_register>: Type of register data (e.g., uint8_t)\n");
    //  printf("     <outValue>: Pointer to store the received value\n");
    printf("     <timeout>: Time in ms to wait for a response\n\n");
}

void help_command_routing(const char *args)
{
    printf("---------- Settings Command Help ----------\n");
    printf("---------> function settings csp <---------\n\n");

    // set_address_csp
    printf("set_address <node>\n");
    printf("   - Set address information for CSP.\n");
    printf("   - Parameters:\n");
    printf("     <node>: node information (0-31)\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // read_routing_table
    printf("get_address\n");
    printf("   - Read Address CSP from the default file.\n");
    printf("   - No parameters.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // reset_rtable
    printf("reset_address\n");
    printf("   - Reset or delete the current address CSP.\n");
    printf("   - No parameters.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // set_routing_csp
    printf("set_rtable <routing>\n");
    printf("   - Set routing information for CSP.\n");
    printf("   - Parameters:\n");
    printf("     <routing>: Routing information (e.g., 1/5CAN,0/0LOOP)\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // read_routing_table
    printf("get_rtable\n");
    printf("   - Read routing table from the default file.\n");
    printf("   - No parameters.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // reset_rtable
    printf("reset_rtable\n");
    printf("   - Reset or delete the current routing table.\n");
    printf("   - No parameters.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");
}

void help_command_script(const char *args)
{
    printf("----------- Script Command Help ----------------\n");
    printf("-----------> function script csp <---------------\n\n");

    // open_notepad
    printf("open_notepad <filename>\n");
    printf("   - Open the specified script file in Notepad.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to open.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // open_nano
    printf("open_nano <filename>\n");
    printf("   - Open the specified script file in Nano editor.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to open.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // delete_script
    printf("delete_script <filename>\n");
    printf("   - Delete the specified script file.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to delete.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // clean_script
    printf("clean_script <filename>\n");
    printf("   - Clean the content of the specified script file.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to clean.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // save_script
    printf("save_script <filename> <command> <parameters>\n");
    printf("   - Save a command and its parameters to the specified script file.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file.\n");
    printf("     <command>: The command to be saved.\n");
    printf("     <parameters>: Parameters of the command.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // run_script
    printf("run_script <filename>\n");
    printf("   - Run the commands from the specified script file.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to run.\n");
    printf("   - Return: Status of the operation (0 = success, -1 = failure)\n\n");

    // run_script_loop
    printf("run_script_loop <filename>\n");
    printf("   - Run the commands from the specified script file.\n");
    printf("   - Parameters:\n");
    printf("     <filename>: Name of the script file to run loop.\n");
    printf("   - Command [stop] to stop\n\n");
}

void print_details(const char *args)
{

    print_ascii_art(NULL);
     printf("\n------------------- Project Details -------------------\n");
    
    print_blue_typing_effect("Project Name: TSC-1 CSP Command Interface\n");
    print_blue_typing_effect("Version: 1.0.0\n");

    print_blue_typing_effect("Address: NARIT, 260 Moo 4, DonKaew Subdistrict, MaeRim District, Chiang Mai, Thailand\n");
    print_blue_typing_effect("Description: This project provides a command interface for controlling various nodes and subsystems.\n");
    
    print_blue_typing_effect("\n------------------- Code Details ---------------------\n");
    
    print_blue_typing_effect("Tested Platform:\n");
    print_blue_typing_effect("Software:\n");
    print_blue_typing_effect("  OS: Windows 11 Pro 23H2 (November 2024), Ubuntu 20.03 LTS\n");
    print_blue_typing_effect("  If Windows:\n");
    print_blue_typing_effect("    - Python: 3.8\n");
    print_blue_typing_effect("    - GCC (Rev2, Built by MSYS2 project) 14.2.0 Copyright (C) in MSYS2 UCRT64\n");
    print_blue_typing_effect("    - pkg-config 2.3.0 in MSYS2 UCRT64\n");
    print_blue_typing_effect("    - lib readline in MSYS2 UCRT64\n");
    print_blue_typing_effect("  If Ubuntu:\n");
    print_blue_typing_effect("    - Python: 3.10.15\n");
    print_blue_typing_effect("    - GCC (Ubuntu 13.2.0-23ubuntu4) 13.2.0 Copyright (C)\n");
    print_blue_typing_effect("    - pkg-config 1.8.1\n");
    print_blue_typing_effect("    - lib readline\n");
    
    print_blue_typing_effect("\nHardware:\n");
    print_blue_typing_effect("  CPU: 13th Gen Intel(R) Core(TM) I7-1360P 2.20 GHz\n");
    print_blue_typing_effect("  RAM: 32.0 GB\n");
    print_blue_typing_effect("  GPU: Intel(R) Iris(R) Xe Graphics\n");
    
    print_blue_typing_effect("\nGit Repository: https://git.narit.or.th/chaturong/tsc1-csp-v1.6.git\n");
    
    printf("\n------------------------------------------------------\n");

}

void print_ascii_art(const char *args)
{
    const char *art =
        "  _______ _____  _____     __    _____  _____ _____     _____                                          _   _____       _             __                \n"
        " |__   __/ ____|/ ____|   /_ |  / ____|/ ____|  __ \\   / ____|                                        | | |_   _|     | |           / _|               \n"
        "    | | | (___ | |   ______| | | |    | (___ | |__) | | |     ___  _ __ ___  _ __ ___   __ _ _ __   __| |   | |  _ __ | |_ ___ _ __| |_ __ _  ___ ___ \n"
        "    | |  \\___ \\| |  |______| | | |     \\___ \\|  ___/  | |    / _ \\| '_ ` _ \\| '_ ` _ \\ / _` | '_ \\ / _` |   | | | '_ \\| __/ _ \\ '__|  _/ _` |/ __/ _ \\ \n"
        "    | |  ____) | |____     | | | |____ ____) | |      | |___| (_) | | | | | | | | | | | (_| | | | | (_| |  _| |_| | | | ||  __/ |  | || (_| | (_|  __/ \n"
        "    |_| |_____/ \\_____|    |_|  \\_____|_____/|_|       \\_____\\___/|_| |_| |_|_| |_| |_|\\__,_|_| |_|\\__,_| |_____|_| |_|\\__\\___|_|  |_| \\__,_|\\___\\___| \n"
        "                                                                                                                                                       \n";

    print_blue_typing_effect(art);
}



void print_colored_char(char c, int color_code)
{
    printf("\033[1;%dm%c\033[0m", color_code, c);
    fflush(stdout);
}

void display_initialising_with_color(const char *text)
{
    int colors[] = {31, 32, 33, 34, 35, 36, 37};

    for (int i = 0; text[i] != '\0'; i++)
    {
        print_colored_char(text[i], colors[i % 7]);
    }

    printf("\n");
}

void print_colored_char1(char c, int color_code, int is_bold)
{
    if (is_bold)
        printf("\033[1;%dm%c\033[0m", color_code, c);
    else
        printf("\033[0;%dm%c\033[0m", color_code, c);

    fflush(stdout);
    usleep(50000);
}

void display_initialising_with_effect(const char *text)
{
    int colors[] = {31, 32, 33, 34, 35, 36, 37};
    int is_bold = 1;

    for (int i = 0; text[i] != '\0'; i++)
    {
        print_colored_char1(text[i], colors[i % 7], is_bold);

        if (i % 3 == 0)
            is_bold = !is_bold;
    }

    printf("\n");
}

void print_scrolling_text(const char *text)
{
    int len = strlen(text);
    int window_width = 40;
    char buffer[41] = {0};
    int colors[] = {31, 32, 33, 34, 35, 36, 37};

    for (int offset = 0; offset < len + window_width; offset++)
    {
        for (int i = 0; i < window_width; i++)
        {
            int index = offset + i - window_width;
            if (index >= 0 && index < len)
            {
                buffer[i] = text[index];
            }
            else
            {
                buffer[i] = ' ';
            }
        }

        printf("\033[1;%dm%s\033[0m\r", colors[offset % 7], buffer);
        fflush(stdout);
        usleep(40000);
    }
}

void print_blue_typing_effect(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        printf("\033[1;34m%c\033[0m", text[i]);
        fflush(stdout);
        //  usleep(100000);
    }
}