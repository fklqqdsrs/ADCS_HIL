#ifndef INC_TASK_ABOARD_H_
#define INC_TASK_ABOARD_H_
#include "main.h"
extern UART_HandleTypeDef huart2; // please comment if not used
#define debug_uart huart2 // please comment if not used
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
#define TASK_mode_mtq1_SIZE 128
#define TASK_mode_rwa1_SIZE 128
#define TASK_mode_acs_SIZE 128
#define TASK_mode_ads_SIZE 128
#define TASK_mode_run_SIZE 128
#define TASK_mode_uart_SIZE 128
#define TASK_mode_acs_signal_id (1<<0)
#define TASK_mode_ads_signal_id (1<<1)
osThreadId_t TASK_mode_mtq1Handle;
uint32_t TASK_mode_mtq1Buffer[TASK_mode_mtq1_SIZE];
StaticTask_t TASK_mode_mtq1ControlBlock;
const osThreadAttr_t TASK_mode_mtq1_attributes = {
	.name = "TASK_mode_mtq1",
	.cb_mem = &TASK_mode_mtq1ControlBlock,
	.cb_size = sizeof(TASK_mode_mtq1ControlBlock),
	.stack_mem = &TASK_mode_mtq1Buffer[0],
	.stack_size = sizeof(TASK_mode_mtq1Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_rwa1Handle;
uint32_t TASK_mode_rwa1Buffer[TASK_mode_rwa1_SIZE];
StaticTask_t TASK_mode_rwa1ControlBlock;
const osThreadAttr_t TASK_mode_rwa1_attributes = {
	.name = "TASK_mode_rwa1",
	.cb_mem = &TASK_mode_rwa1ControlBlock,
	.cb_size = sizeof(TASK_mode_rwa1ControlBlock),
	.stack_mem = &TASK_mode_rwa1Buffer[0],
	.stack_size = sizeof(TASK_mode_rwa1Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osEventFlagsId_t TASK_mode_acs_evt;
osStaticEventGroupDef_t TASK_mode_acs_evt_ControlBlock;
const osEventFlagsAttr_t TASK_mode_acs_evt_attributes = {
	.name = "TASK_mode_acs_evt",
	.cb_mem = &TASK_mode_acs_evt_ControlBlock,
	.cb_size = sizeof(TASK_mode_acs_evt_ControlBlock),
};
osThreadId_t TASK_mode_acsHandle;
uint32_t TASK_mode_acsBuffer[TASK_mode_acs_SIZE];
StaticTask_t TASK_mode_acsControlBlock;
const osThreadAttr_t TASK_mode_acs_attributes = {
	.name = "TASK_mode_acs",
	.cb_mem = &TASK_mode_acsControlBlock,
	.cb_size = sizeof(TASK_mode_acsControlBlock),
	.stack_mem = &TASK_mode_acsBuffer[0],
	.stack_size = sizeof(TASK_mode_acsBuffer),
	.priority = (osPriority_t)osPriorityAboveNormal,
};
osEventFlagsId_t TASK_mode_ads_evt;
osStaticEventGroupDef_t TASK_mode_ads_evt_ControlBlock;
const osEventFlagsAttr_t TASK_mode_ads_evt_attributes = {
	.name = "TASK_mode_ads_evt",
	.cb_mem = &TASK_mode_ads_evt_ControlBlock,
	.cb_size = sizeof(TASK_mode_ads_evt_ControlBlock),
};
osThreadId_t TASK_mode_adsHandle;
uint32_t TASK_mode_adsBuffer[TASK_mode_ads_SIZE];
StaticTask_t TASK_mode_adsControlBlock;
const osThreadAttr_t TASK_mode_ads_attributes = {
	.name = "TASK_mode_ads",
	.cb_mem = &TASK_mode_adsControlBlock,
	.cb_size = sizeof(TASK_mode_adsControlBlock),
	.stack_mem = &TASK_mode_adsBuffer[0],
	.stack_size = sizeof(TASK_mode_adsBuffer),
	.priority = (osPriority_t)osPriorityHigh,
};
osThreadId_t TASK_mode_runHandle;
uint32_t TASK_mode_runBuffer[TASK_mode_run_SIZE];
StaticTask_t TASK_mode_runControlBlock;
const osThreadAttr_t TASK_mode_run_attributes = {
	.name = "TASK_mode_run",
	.cb_mem = &TASK_mode_runControlBlock,
	.cb_size = sizeof(TASK_mode_runControlBlock),
	.stack_mem = &TASK_mode_runBuffer[0],
	.stack_size = sizeof(TASK_mode_runBuffer),
	.priority = (osPriority_t)osPriorityRealtime,
};
osThreadId_t TASK_mode_uartHandle;
uint32_t TASK_mode_uartBuffer[TASK_mode_uart_SIZE];
StaticTask_t TASK_mode_uartControlBlock;
const osThreadAttr_t TASK_mode_uart_attributes = {
	.name = "TASK_mode_uart",
	.cb_mem = &TASK_mode_uartControlBlock,
	.cb_size = sizeof(TASK_mode_uartControlBlock),
	.stack_mem = &TASK_mode_uartBuffer[0],
	.stack_size = sizeof(TASK_mode_uartBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
void TASK_mode_mtq1(void* argument);
void TASK_mode_rwa1(void* argument);
void TASK_mode_acs(void* argument);
void TASK_mode_ads(void* argument);
void TASK_mode_run(void* argument);
void TASK_mode_uart(void* argument);
#include "TASK_mode_mtq1.h"
#include "TASK_mode_rwa1.h"
#include "TASK_mode_acs.h"
#include "TASK_mode_ads.h"
#include "TASK_mode_run.h"
#include "TASK_mode_uart.h"

void TASK_ABOARD_init_task(){
	TASK_mode_acs_evt = osEventFlagsNew(&TASK_mode_acs_evt_attributes);
	TASK_mode_ads_evt = osEventFlagsNew(&TASK_mode_ads_evt_attributes);
	TASK_mode_mtq1Handle = osThreadNew(TASK_mode_mtq1, NULL,&TASK_mode_mtq1_attributes);
	TASK_mode_rwa1Handle = osThreadNew(TASK_mode_rwa1, NULL,&TASK_mode_rwa1_attributes);
	TASK_mode_acsHandle = osThreadNew(TASK_mode_acs, NULL,&TASK_mode_acs_attributes);
	TASK_mode_adsHandle = osThreadNew(TASK_mode_ads, NULL,&TASK_mode_ads_attributes);
	TASK_mode_runHandle = osThreadNew(TASK_mode_run, NULL,&TASK_mode_run_attributes);
	TASK_mode_uartHandle = osThreadNew(TASK_mode_uart, NULL,&TASK_mode_uart_attributes);
} //end task_init


#endif //end TASK_ABOARD file