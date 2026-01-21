#ifndef INC_TASK_mode_can_H_
#define INC_TASK_mode_can_H_
#include "main.h"
#include "TASK_TSC1.h"
#include "REG_TSC1.h"
#include "ClassInit/GNSSInit.h"
void fcn_mode_can_Nothing();
void fcn_mode_can_normal();

#define RWA1 7
#define RWA2 4
#define RWA3 9
#define RWA4 10

extern SemaphoreHandle_t xCANRxSemaphore;

typedef struct {
    uint32_t id;
    uint8_t device_id;
    uint8_t dlc;
    uint8_t data[8];

} canRxHandle_t;

enum class can_enum_t: uint8_t
{
	Nothing = 0,
	normal = 1,

};

canRxHandle_t rxMessage;
extern QueueHandle_t canRxQueue;
__weak void TASK_mode_can(void* argument){

	for(;;){

	//	osDelay(ram_data.mode_can_delay_ms);
		switch(ram_data.mode_can){
			case (uint8_t)can_enum_t::Nothing :
				fcn_mode_can_Nothing();
				ram_data.mode_can_delay_ms = 1;
				break;
			case (uint8_t)can_enum_t::normal :
				fcn_mode_can_normal();
				ram_data.mode_can_delay_ms = 10;
				break;

			default:
#ifdef debug_uart
				ram_data.mode_can = 0;
#endif
				break;
		} //switch end
	}//loop end
 }//function end
__weak void fcn_mode_can_Nothing(){
	
	ram_data.mode_can = (uint8_t)can_enum_t::normal;

}



__weak void fcn_mode_can_normal(){


  while(1)
  {
        if (xQueueReceive(canRxQueue, &rxMessage, portMAX_DELAY) == pdPASS) {
            // Identify the device based on CAN ID
            switch (rxMessage.device_id) {
                case RWA1:
                	whl1.CANRxIdDecode(rxMessage.id);
                	whl1.RxMessage(rxMessage.data, rxMessage.dlc);
                    break;

                case RWA2:
                	whl2.CANRxIdDecode(rxMessage.id);
                	whl2.RxMessage(rxMessage.data, rxMessage.dlc);
                    break;

                case RWA3:
                	whl3.CANRxIdDecode(rxMessage.id);
                	whl3.RxMessage(rxMessage.data, rxMessage.dlc);
                    break;

                case RWA4:
                	whl4.CANRxIdDecode(rxMessage.id);
                	whl4.RxMessage(rxMessage.data, rxMessage.dlc);
                    break;

                default:
                	if(GNSS.deviceCheck(rxMessage.id))
                	{
                		GNSS.RxMessage(rxMessage.data, rxMessage.dlc);
                	}


                    break;
            }



	}

  }

}
#endif
