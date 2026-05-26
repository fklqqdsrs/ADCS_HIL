#include "app_modules_config.h"
#if CSP_ENABLE_ROUTE

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"csp_route_cli_client.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#include <csp/csp.h>
#include <csp_route_core.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#ifdef TSC_USING_FINSH
#include "finsh.h"

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ---------------------------------------------------------------------- //
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
/* --- route_load --- */
static void __csp_route_load(int argc, char **argv) {
    if (argc < 2) {
        tsc_kprintf("Usage: route_load <node>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    int result = csp_route_load(node, 1000, &msg);
    if (result == CSP_ERR_NONE) {
        tsc_kprintf("Route load successfully, valid entries=%i\r\n", msg.load);
    } else {
        tsc_kprintf("Route load failed (err=%d, code=%i)\r\n", result, msg.load);
    }
}

/* --- route_save --- */
static void __csp_route_save(int argc, char **argv) {
    if (argc < 2) {
        tsc_kprintf("Usage: route_save <node>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    int result = csp_route_save(node, 1000, &msg);
    if (result == CSP_ERR_NONE) {
        tsc_kprintf("Route save successfully result=%i\r\n", msg.save);
    } else {
        tsc_kprintf("Route save failed (err=%d, code=%i)\r\n", result, msg.save);
    }
}

/* --- route_check --- */
static void __csp_route_check(int argc, char **argv) {
    if (argc < 2) {
        tsc_kprintf("Usage: route_check <node>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    int result = csp_route_check(node, 1000, &msg);
    if (result == CSP_ERR_NONE) {
        tsc_kprintf("Route check successfully, valid entries=%i\r\n", msg.check);
    } else {
        tsc_kprintf("Route check failed (err=%d, code=%i)\r\n", result, msg.check);
    }
}

/* --- route_clear --- */
static void __csp_route_clear(int argc, char **argv) {
    if (argc < 2) {
        tsc_kprintf("Usage: route_clear <node>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    int result = csp_route_clear(node, 1000, &msg);
    if (result == CSP_ERR_NONE) {
        tsc_kprintf("Route clear successfully\r\n");
    } else {
        tsc_kprintf("Route clear failed (err=%d)\r\n", result);
    }
}

/* --- route_status --- */
static void __csp_route_status(int argc, char **argv) {
    if (argc < 2) {
        tsc_kprintf("Usage: route_status <node>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    int result = csp_route_status(node, 1000, &msg);
    if (result == CSP_ERR_NONE) {
        tsc_kprintf("Route table from node %d:\r\n%s\r\n", node, msg.status.data);
    } else {
        tsc_kprintf("route_status failed, err=%d\r\n", result);
    }
}

/* --- route_add --- */
static void __csp_route_add(int argc, char **argv) {
    if (argc < 6) {
        tsc_kprintf("Usage: route_add <node> <addr> <mask> <via> <iface>\r\n");
        return;
    }
    uint8_t node = (uint8_t) atoi(argv[1]);
    struct csp_route_message msg = {0};
    msg.add.address = (uint8_t) atoi(argv[2]);
    msg.add.netmask = (uint8_t) atoi(argv[3]);
    msg.add.via     = (uint8_t) atoi(argv[4]);
    strncpy(msg.add.iface_name, argv[5], sizeof(msg.add.iface_name) - 1);
    msg.add.iface_name[sizeof(msg.add.iface_name) - 1] = '\0';

    int result = csp_route_add(node, 1000, &msg);
    if (result == CSP_ERR_NONE && msg.add.error == 0) {
        tsc_kprintf("Route added successfully\r\n");
    } else {
        tsc_kprintf("route_add failed (err=%d, code=%d)\r\n", result, msg.add.error);
    }
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(__csp_route_load, csp_route_load, "Load route table from node");
TSC_CMD_EXPORT_ALIAS(__csp_route_save, csp_route_save, "Save route table on node");
TSC_CMD_EXPORT_ALIAS(__csp_route_check, csp_route_check, "Check route table on node");
TSC_CMD_EXPORT_ALIAS(__csp_route_clear, csp_route_clear, "Clear route table on node");
TSC_CMD_EXPORT_ALIAS(__csp_route_status, csp_route_status, "Show route table from node");
TSC_CMD_EXPORT_ALIAS(__csp_route_add, csp_route_add, "Add route entry on node");
#endif
// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_ROUTE */
