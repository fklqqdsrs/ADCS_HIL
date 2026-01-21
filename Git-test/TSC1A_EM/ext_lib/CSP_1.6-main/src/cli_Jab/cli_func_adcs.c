#include "cli_csp.h"

void csp_set_rpm_view_command(const char *args) {
    uint8_t node_view = 0;
    uint32_t rpm_view = 0;
    uint32_t timeout = 0;

    char *args_copy = strdup(args);
    char *token;

    token = strtok(args_copy, " ");
    if (token != NULL) {
        node_view = (uint8_t)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        rpm_view = (uint32_t)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
    }

    if (node_view > 0 && rpm_view > 0 && timeout > 0) {
        int8_t status = set_ms_rpm_react_view(node_view, rpm_view, timeout);
        printf("Running set_view: view=%d, rpm_view=%lu, timeout=%lu, status=%d\n", node_view, rpm_view, timeout, status);
    } else {
        printf("Usage: set_view <node_view> <rpm_view> <timeout>\n");
    }

    free(args_copy);
}

void csp_get_rpm_view_command(const char *args) {
    uint8_t node_view = 0;
    uint32_t timeout = 0;
    char *args_copy = strdup(args);
    char *token;
    token = strtok(args_copy, " ");
    if (token != NULL) {
        node_view = (uint8_t)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
    }

    if (node_view > 0 && timeout > 0) {
        uint32_t value_react = get_ms_rpm_react_view(node_view, timeout);
        if (value_react != 0xFFFFFFFF) {
            printf("Running csp_get_rpm_view: view=%d, RPM view=%lu\n", node_view, value_react);
        }
    } else {
        printf("Usage: get_view <node_view> <timeout>\n");
    }
    free(args_copy); 
}

void csp_get_imu_command(const char *args) {
    uint32_t timeout = 0;
    char *args_copy = strdup(args);
    char *token;

    token = strtok(args_copy, " ");
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
    }

    if (timeout > 0) {
        printf("Running csp_get_imu: timeout=%lu\n", timeout);
        axial_imu imu_data_test;
        get_imu(&imu_data_test, timeout);
    } else {
        printf("Usage: get_imu <timeout>\n");
    }

    free(args_copy);
}

void csp_get_gps_command(const char *args) {
    uint32_t timeout = 0;
    char *args_copy = strdup(args);
    char *token;

    token = strtok(args_copy, " ");
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
    }

    if (timeout > 0) {
        printf("Running get_gps: timeout=%lu\n", timeout);
        gps_data_t gps_test;
        get_gps(&gps_test, timeout);
    } else {
        printf("Usage: get_gps <timeout>\n");
    }

    free(args_copy);
}

void csp_get_mtm_command(const char *args) {
    uint32_t timeout = 0;
    char *args_copy = strdup(args);
    char *token;

    token = strtok(args_copy, " ");
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
    }

    if (timeout > 0) {
        axial_mtm mtm_test;
        get_mtm(&mtm_test, timeout);
        printf("Running get_mtm: timeout=%lu\n", timeout);
    } else {
        printf("Usage: get_mtm <timeout>\n");
    }

    free(args_copy);
}
