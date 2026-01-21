#ifndef INC_TASK_TSC1_H_
#define INC_TASK_TSC1_H_
#include "main.h"
#include "usart.h"
extern UART_HandleTypeDef huart5; // please comment if not used
#define UART_RX_BUFFER_SIZE 36
 // Buffer for 36 bytes


//#define debug_uart huart5 // please comment if not used
#include "TASK_mode_obc_mtq1.h"
#include "TASK_mode_obc_mtq2.h"
#include "TASK_mode_obc_mtq3.h"
#include "TASK_mode_obc_rwa1.h"
#include "TASK_mode_obc_rwa2.h"
#include "TASK_mode_obc_rwa3.h"
#include "TASK_mode_obc_rwa4.h"
#include "TASK_mode_cube_acs.h"
#include "TASK_mode_cube_ads.h"
#include "TASK_mode_mem.h"
#include "TASK_mode_rwa.h"
#include "TASK_mode_mtq.h"
#include "TASK_mode_can.h"
#include "TASK_mode_uart.h"
#include "TASK_mode_i2c.h"
#include "TASK_mode_slave.h"
#include "TASK_mode_acs.h"
#include "TASK_mode_ads.h"
#include "TASK_mode_run.h"
#include "TASK_mode_imu.h"
#include "TASK_mode_gps.h"
#include "TASK_mode_mtm.h"
#include "TASK_mode_fss.h"
#include "TASK_mode_stt.h"

#define TASK_mode_obc_mtq1_SIZE 1280
#define TASK_mode_obc_mtq2_SIZE 1280
#define TASK_mode_obc_mtq3_SIZE 1280
#define TASK_mode_obc_rwa1_SIZE 1280
#define TASK_mode_obc_rwa2_SIZE 1280
#define TASK_mode_obc_rwa3_SIZE 1280
#define TASK_mode_obc_rwa4_SIZE 1280
#define TASK_mode_cube_acs_SIZE 1280
#define TASK_mode_cube_ads_SIZE 1280
#define TASK_mode_mem_SIZE 1280
#define TASK_mode_rwa_SIZE 1280
#define TASK_mode_mtq_SIZE 500
#define TASK_mode_can_SIZE 2500
#define TASK_mode_uart_SIZE 1280
#define TASK_mode_i2c_SIZE 1280
#define TASK_mode_slave_SIZE 1280
#define TASK_mode_acs_SIZE 1280
#define TASK_mode_ads_SIZE 1280
#define TASK_mode_run_SIZE 1280
#define TASK_mode_imu_SIZE 1280
#define TASK_mode_gps_SIZE 1280
#define TASK_mode_mtm_SIZE 1280
#define TASK_mode_fss_SIZE 1280
#define TASK_mode_stt_SIZE 1280



osThreadId_t TASK_mode_obc_mtq1Handle;
uint32_t TASK_mode_obc_mtq1Buffer[TASK_mode_obc_mtq1_SIZE];
StaticTask_t TASK_mode_obc_mtq1ControlBlock;
const osThreadAttr_t TASK_mode_obc_mtq1_attributes = {
	.name = "TASK_mode_obc_mtq1",
	.cb_mem = &TASK_mode_obc_mtq1ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_mtq1ControlBlock),
	.stack_mem = &TASK_mode_obc_mtq1Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_mtq1Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_mtq2Handle;
uint32_t TASK_mode_obc_mtq2Buffer[TASK_mode_obc_mtq2_SIZE];
StaticTask_t TASK_mode_obc_mtq2ControlBlock;
const osThreadAttr_t TASK_mode_obc_mtq2_attributes = {
	.name = "TASK_mode_obc_mtq2",
	.cb_mem = &TASK_mode_obc_mtq2ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_mtq2ControlBlock),
	.stack_mem = &TASK_mode_obc_mtq2Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_mtq2Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_mtq3Handle;
uint32_t TASK_mode_obc_mtq3Buffer[TASK_mode_obc_mtq3_SIZE];
StaticTask_t TASK_mode_obc_mtq3ControlBlock;
const osThreadAttr_t TASK_mode_obc_mtq3_attributes = {
	.name = "TASK_mode_obc_mtq3",
	.cb_mem = &TASK_mode_obc_mtq3ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_mtq3ControlBlock),
	.stack_mem = &TASK_mode_obc_mtq3Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_mtq3Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_rwa1Handle;
uint32_t TASK_mode_obc_rwa1Buffer[TASK_mode_obc_rwa1_SIZE];
StaticTask_t TASK_mode_obc_rwa1ControlBlock;
const osThreadAttr_t TASK_mode_obc_rwa1_attributes = {
	.name = "TASK_mode_obc_rwa1",
	.cb_mem = &TASK_mode_obc_rwa1ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_rwa1ControlBlock),
	.stack_mem = &TASK_mode_obc_rwa1Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_rwa1Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_rwa2Handle;
uint32_t TASK_mode_obc_rwa2Buffer[TASK_mode_obc_rwa2_SIZE];
StaticTask_t TASK_mode_obc_rwa2ControlBlock;
const osThreadAttr_t TASK_mode_obc_rwa2_attributes = {
	.name = "TASK_mode_obc_rwa2",
	.cb_mem = &TASK_mode_obc_rwa2ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_rwa2ControlBlock),
	.stack_mem = &TASK_mode_obc_rwa2Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_rwa2Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_rwa3Handle;
uint32_t TASK_mode_obc_rwa3Buffer[TASK_mode_obc_rwa3_SIZE];
StaticTask_t TASK_mode_obc_rwa3ControlBlock;
const osThreadAttr_t TASK_mode_obc_rwa3_attributes = {
	.name = "TASK_mode_obc_rwa3",
	.cb_mem = &TASK_mode_obc_rwa3ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_rwa3ControlBlock),
	.stack_mem = &TASK_mode_obc_rwa3Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_rwa3Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_obc_rwa4Handle;
uint32_t TASK_mode_obc_rwa4Buffer[TASK_mode_obc_rwa4_SIZE];
StaticTask_t TASK_mode_obc_rwa4ControlBlock;
const osThreadAttr_t TASK_mode_obc_rwa4_attributes = {
	.name = "TASK_mode_obc_rwa4",
	.cb_mem = &TASK_mode_obc_rwa4ControlBlock,
	.cb_size = sizeof(TASK_mode_obc_rwa4ControlBlock),
	.stack_mem = &TASK_mode_obc_rwa4Buffer[0],
	.stack_size = sizeof(TASK_mode_obc_rwa4Buffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_cube_acsHandle;
uint32_t TASK_mode_cube_acsBuffer[TASK_mode_cube_acs_SIZE];
StaticTask_t TASK_mode_cube_acsControlBlock;
const osThreadAttr_t TASK_mode_cube_acs_attributes = {
	.name = "TASK_mode_cube_acs",
	.cb_mem = &TASK_mode_cube_acsControlBlock,
	.cb_size = sizeof(TASK_mode_cube_acsControlBlock),
	.stack_mem = &TASK_mode_cube_acsBuffer[0],
	.stack_size = sizeof(TASK_mode_cube_acsBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_cube_adsHandle;
uint32_t TASK_mode_cube_adsBuffer[TASK_mode_cube_ads_SIZE];
StaticTask_t TASK_mode_cube_adsControlBlock;
const osThreadAttr_t TASK_mode_cube_ads_attributes = {
	.name = "TASK_mode_cube_ads",
	.cb_mem = &TASK_mode_cube_adsControlBlock,
	.cb_size = sizeof(TASK_mode_cube_adsControlBlock),
	.stack_mem = &TASK_mode_cube_adsBuffer[0],
	.stack_size = sizeof(TASK_mode_cube_adsBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_memHandle;
uint32_t TASK_mode_memBuffer[TASK_mode_mem_SIZE];
StaticTask_t TASK_mode_memControlBlock;
const osThreadAttr_t TASK_mode_mem_attributes = {
	.name = "TASK_mode_mem",
	.cb_mem = &TASK_mode_memControlBlock,
	.cb_size = sizeof(TASK_mode_memControlBlock),
	.stack_mem = &TASK_mode_memBuffer[0],
	.stack_size = sizeof(TASK_mode_memBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_rwaHandle;
uint32_t TASK_mode_rwaBuffer[TASK_mode_rwa_SIZE];
StaticTask_t TASK_mode_rwaControlBlock;
const osThreadAttr_t TASK_mode_rwa_attributes = {
	.name = "TASK_mode_rwa",
	.cb_mem = &TASK_mode_rwaControlBlock,
	.cb_size = sizeof(TASK_mode_rwaControlBlock),
	.stack_mem = &TASK_mode_rwaBuffer[0],
	.stack_size = sizeof(TASK_mode_rwaBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_mtqHandle;
uint32_t TASK_mode_mtqBuffer[TASK_mode_mtq_SIZE];
StaticTask_t TASK_mode_mtqControlBlock;
const osThreadAttr_t TASK_mode_mtq_attributes = {
	.name = "TASK_mode_mtq",
	.cb_mem = &TASK_mode_mtqControlBlock,
	.cb_size = sizeof(TASK_mode_mtqControlBlock),
	.stack_mem = &TASK_mode_mtqBuffer[0],
	.stack_size = sizeof(TASK_mode_mtqBuffer),
	.priority = (osPriority_t)osPriorityLow,
};
osThreadId_t TASK_mode_canHandle;
uint32_t TASK_mode_canBuffer[TASK_mode_can_SIZE];
StaticTask_t TASK_mode_canControlBlock;
const osThreadAttr_t TASK_mode_can_attributes = {
	.name = "TASK_mode_can",
	.cb_mem = &TASK_mode_canControlBlock,
	.cb_size = sizeof(TASK_mode_canControlBlock),
	.stack_mem = &TASK_mode_canBuffer[0],
	.stack_size = sizeof(TASK_mode_canBuffer),
	.priority = (osPriority_t)osPriorityHigh,
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
osThreadId_t TASK_mode_i2cHandle;
uint32_t TASK_mode_i2cBuffer[TASK_mode_i2c_SIZE];
StaticTask_t TASK_mode_i2cControlBlock;
const osThreadAttr_t TASK_mode_i2c_attributes = {
	.name = "TASK_mode_i2c",
	.cb_mem = &TASK_mode_i2cControlBlock,
	.cb_size = sizeof(TASK_mode_i2cControlBlock),
	.stack_mem = &TASK_mode_i2cBuffer[0],
	.stack_size = sizeof(TASK_mode_i2cBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_slaveHandle;
uint32_t TASK_mode_slaveBuffer[TASK_mode_slave_SIZE];
StaticTask_t TASK_mode_slaveControlBlock;
const osThreadAttr_t TASK_mode_slave_attributes = {
	.name = "TASK_mode_slave",
	.cb_mem = &TASK_mode_slaveControlBlock,
	.cb_size = sizeof(TASK_mode_slaveControlBlock),
	.stack_mem = &TASK_mode_slaveBuffer[0],
	.stack_size = sizeof(TASK_mode_slaveBuffer),
	.priority = (osPriority_t)osPriorityNormal,
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
	.priority = (osPriority_t)osPriorityNormal,
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
	.priority = (osPriority_t)osPriorityNormal,
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
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_imuHandle;
uint32_t TASK_mode_imuBuffer[TASK_mode_imu_SIZE];
StaticTask_t TASK_mode_imuControlBlock;
const osThreadAttr_t TASK_mode_imu_attributes = {
	.name = "TASK_mode_imu",
	.cb_mem = &TASK_mode_imuControlBlock,
	.cb_size = sizeof(TASK_mode_imuControlBlock),
	.stack_mem = &TASK_mode_imuBuffer[0],
	.stack_size = sizeof(TASK_mode_imuBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_gpsHandle;
uint32_t TASK_mode_gpsBuffer[TASK_mode_gps_SIZE];
StaticTask_t TASK_mode_gpsControlBlock;
const osThreadAttr_t TASK_mode_gps_attributes = {
	.name = "TASK_mode_gps",
	.cb_mem = &TASK_mode_gpsControlBlock,
	.cb_size = sizeof(TASK_mode_gpsControlBlock),
	.stack_mem = &TASK_mode_gpsBuffer[0],
	.stack_size = sizeof(TASK_mode_gpsBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_mtmHandle;
uint32_t TASK_mode_mtmBuffer[TASK_mode_mtm_SIZE];
StaticTask_t TASK_mode_mtmControlBlock;
const osThreadAttr_t TASK_mode_mtm_attributes = {
	.name = "TASK_mode_mtm",
	.cb_mem = &TASK_mode_mtmControlBlock,
	.cb_size = sizeof(TASK_mode_mtmControlBlock),
	.stack_mem = &TASK_mode_mtmBuffer[0],
	.stack_size = sizeof(TASK_mode_mtmBuffer),
	.priority = (osPriority_t)osPriorityHigh,
};

osThreadId_t TASK_mode_sttHandle;
uint32_t TASK_mode_sttBuffer[TASK_mode_stt_SIZE];
StaticTask_t TASK_mode_sttControlBlock;
const osThreadAttr_t TASK_mode_stt_attributes = {
	.name = "TASK_mode_stt",
	.cb_mem = &TASK_mode_sttControlBlock,
	.cb_size = sizeof(TASK_mode_sttControlBlock),
	.stack_mem = &TASK_mode_sttBuffer[0],
	.stack_size = sizeof(TASK_mode_sttBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};
osThreadId_t TASK_mode_fssHandle;
uint32_t TASK_mode_fssBuffer[TASK_mode_fss_SIZE];
StaticTask_t TASK_mode_fssControlBlock;
const osThreadAttr_t TASK_mode_fss_attributes = {
	.name = "TASK_mode_fss",
	.cb_mem = &TASK_mode_fssControlBlock,
	.cb_size = sizeof(TASK_mode_fssControlBlock),
	.stack_mem = &TASK_mode_fssBuffer[0],
	.stack_size = sizeof(TASK_mode_fssBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};



void TASK_mode_obc_mtq1(void* argument);
void TASK_mode_obc_mtq2(void* argument);
void TASK_mode_obc_mtq3(void* argument);
void TASK_mode_obc_rwa1(void* argument);
void TASK_mode_obc_rwa2(void* argument);
void TASK_mode_obc_rwa3(void* argument);
void TASK_mode_obc_rwa4(void* argument);
void TASK_mode_cube_acs(void* argument);
void TASK_mode_cube_ads(void* argument);
void TASK_mode_mem(void* argument);
void TASK_mode_rwa(void* argument);
void TASK_mode_mtq(void* argument);
void TASK_mode_can(void* argument);
void TASK_mode_uart(void* argument);
void TASK_mode_i2c(void* argument);
void TASK_mode_slave(void* argument);
void TASK_mode_acs(void* argument);
void TASK_mode_ads(void* argument);
void TASK_mode_run(void* argument);
void TASK_mode_imu(void* argument);
void TASK_mode_gps(void* argument);
void TASK_mode_mtm(void* argument);
void TASK_mode_stt(void* argument);
void TASK_mode_fss(void* argument);

void TASK_TSC1_init_task(){
	TASK_mode_mtmHandle = osThreadNew(TASK_mode_mtm, NULL,&TASK_mode_mtm_attributes);
	//TASK_mode_mtqHandle = osThreadNew(TASK_mode_mtq, NULL,&TASK_mode_mtq_attributes);
//	TASK_mode_obc_mtq1Handle = osThreadNew(TASK_mode_obc_mtq1, NULL,&TASK_mode_obc_mtq1_attributes);
//	TASK_mode_obc_mtq2Handle = osThreadNew(TASK_mode_obc_mtq2, NULL,&TASK_mode_obc_mtq2_attributes);
//	TASK_mode_obc_mtq3Handle = osThreadNew(TASK_mode_obc_mtq3, NULL,&TASK_mode_obc_mtq3_attributes);
//	TASK_mode_obc_rwa1Handle = osThreadNew(TASK_mode_obc_rwa1, NULL,&TASK_mode_obc_rwa1_attributes);
//	TASK_mode_obc_rwa2Handle = osThreadNew(TASK_mode_obc_rwa2, NULL,&TASK_mode_obc_rwa2_attributes);
//	TASK_mode_obc_rwa3Handle = osThreadNew(TASK_mode_obc_rwa3, NULL,&TASK_mode_obc_rwa3_attributes);
	//TASK_mode_obc_rwa4Handle = osThreadNew(TASK_mode_obc_rwa4, NULL,&TASK_mode_obc_rwa4_attributes);

//	TASK_mode_memHandle = osThreadNew(TASK_mode_mem, NULL,&TASK_mode_mem_attributes);


//	TASK_mode_canHandle = osThreadNew(TASK_mode_can, NULL,&TASK_mode_can_attributes);
//	TASK_mode_uartHandle = osThreadNew(TASK_mode_uart, NULL,&TASK_mode_uart_attributes);
//	TASK_mode_i2cHandle = osThreadNew(TASK_mode_i2c, NULL,&TASK_mode_i2c_attributes);
//	TASK_mode_slaveHandle = osThreadNew(TASK_mode_slave, NULL,&TASK_mode_slave_attributes);
//	TASK_mode_acsHandle = osThreadNew(TASK_mode_acs, NULL,&TASK_mode_acs_attributes);
//	TASK_mode_adsHandle = osThreadNew(TASK_mode_ads, NULL,&TASK_mode_ads_attributes);
//	TASK_mode_runHandle = osThreadNew(TASK_mode_run, NULL,&TASK_mode_run_attributes);
//	TASK_mode_imuHandle = osThreadNew(TASK_mode_imu, NULL,&TASK_mode_imu_attributes);
//	TASK_mode_gpsHandle = osThreadNew(TASK_mode_gps, NULL,&TASK_mode_gps_attributes);
//	TASK_mode_sttHandle = osThreadNew(TASK_mode_stt, NULL,&TASK_mode_stt_attributes);
//	TASK_mode_fssHandle = osThreadNew(TASK_mode_fss, NULL,&TASK_mode_fss_attributes);


} //end task_init


#endif //end TASK_TSC1 file
