/**
 * @file    csp_route_server.h
 * @brief   CSP Routing Service Header
 * @author  Pongpot Chaiboonuang
 */

#ifndef CSP_ROUTE_SERVER_H
#define CSP_ROUTE_SERVER_H

#include "app_modules_config.h"

#if CSP_ENABLE_ROUTE

#ifdef __cplusplus
extern "C" {
#endif

int csp_task_init(void);
void csp_task_start(void);

#ifdef __cplusplus
}
#endif

#endif /* CSP_ENABLE_ROUTE */

#endif /* CSP_ROUTE_SERVER_H */
