#ifndef INC_TASK_mode_imu_H_
#define INC_TASK_mode_imu_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/IMUInit.h"
#include "usart.h"

void fcn_mode_imu_Nothing();
void fcn_mode_imu_on();
void fcn_mode_imu_off();
void fcn_mode_imu_config();

static bool on_flag = 0;
extern QueueHandle_t uartQueue;
extern uint8_t uartRxBuffer[36];
extern SemaphoreHandle_t  uartRxSemaphore;

enum class imu_enum_t: uint8_t
{
	Nothing = 0,
	on = 1,
	off = 2,
	config = 3,
};

__weak void TASK_mode_imu(void* argument){

	for(;;){

		osDelay(ram_data.mode_imu_delay_ms);
		switch(ram_data.mode_imu){
			case (uint8_t)imu_enum_t::Nothing :
				fcn_mode_imu_Nothing();
				ram_data.mode_imu_delay_ms = 1;
				break;
			case (uint8_t)imu_enum_t::on :
				fcn_mode_imu_on();
				ram_data.mode_imu_delay_ms = 50;
				break;
			case (uint8_t)imu_enum_t::off :
				fcn_mode_imu_off();
				ram_data.mode_imu_delay_ms = 10;
				break;
			case (uint8_t)imu_enum_t::config :
				fcn_mode_imu_config();
				ram_data.mode_imu_delay_ms = 1000;
				break;
			default:
#ifdef debug_uart
				ram_data.mode_imu = 0;
#endif
				break;
		} //switch end
	}//loop end

 }//function end
__weak void fcn_mode_imu_Nothing(){
	


	//add code below
	ram_data.mode_imu = (uint8_t)imu_enum_t::on ;
}

__weak void fcn_mode_imu_on(){
	


	//add code below

	if (on_flag == 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
		on_flag = 1;
	}

//	vTaskSuspend(NULL);

    if (xSemaphoreTake(uartRxSemaphore, pdMS_TO_TICKS(1000)) == pdTRUE) {
        // Process received 36-byte message
    	HAL_UART_Receive_DMA(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);

//    	reverse_bytes(uartRxBuffer+5, (uint8_t*)&imu_ram_data.gyro_dat, 12);
    	reverse_bytes(uartRxBuffer+5, (uint8_t*)&imu_ram_data.gyro_x, 4);
//    	reverse_bytes(uartRxBuffer+9, (uint8_t*)&imu_ram_data.gyro_y, 4);
//    	reverse_bytes(uartRxBuffer+13,(uint8_t*)&imu_ram_data.gyro_z, 4);

    	//reverse_bytes(uartRxBuffer+17, (uint8_t*)&imu_ram_data.accel_data, 12);
    	imu_ram_data.temp = uartRxBuffer[32];
    	imu_ram_data.stat = uartRxBuffer[29];
    	HAL_UART_Init(&huart2);

    	__SEV();
    }


}

__weak void fcn_mode_imu_off(){
	

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
	on_flag = 0;
	//add code below
}

__weak void fcn_mode_imu_config(){
	


	//add code below

}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//    if (huart->Instance == USART2) {
//        // Send data to a FreeRTOS queue or notify a task
//
//
//        // Restart UART reception
//        HAL_UART_Receive_IT(&huart2, imu_buffer, sizeof(imu_buffer));
//        reverse_bytes(imu_buffer+15, (uint8_t*)imu_ram_data.gyro, 12);
//
//    }
//}
#endif
