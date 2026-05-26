
#ifndef SCRIPT_SERVICE_INIT_H_
#define SCRIPT_SERVICE_INIT_H_

#include <csp/csp.h>

// Define the CSP port for the script service
#define SCRIPT_SERVICE_PORT 15

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the script service, including setting up the CSP listener.
 * 
 * @return int 0 on success, -1 on error.
 */
int script_service_init(void);

/**
 * @brief The main task for handling incoming CSP requests for the script service.
 * 
 * @param param Task parameters (not used).
 * @return void*
 */
void script_service_task(void *param);

#ifdef __cplusplus
}
#endif

#endif /* SCRIPT_SERVICE_INIT_H_ */
