/*
 * OEM719.h
 *
 *  Created on: Apr 25, 2024
 *      Author: Chinn
 */

#ifndef INC_OEM719_H_
#define INC_OEM719_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "time.h"
#include "OEM719_REG.h"
#include "NEWIO/UART/newuart.h"
#include "NEWIO/CAN/newPortCAN.h"
#include "General.h"



#define CRC32_POLYNOMIAL     0xEDB88320L
#define BUFF_LEN             150


// Header Type
#define SHORT_HEADER         0
#define FULL_HEADER          1

// Header Length
#define SHORT_HEADER_LENGTH  12
#define HEADER_LENGTH        28

//Message ID bytes address
#define MESSAGE_ID_BYTES1    4
#define MESSAGE_ID_BYTES2    5

// GPS Time Parameter
#define GPS_EPOCH_YEAR       1980
#define GPS_EPOCH_MONTH      1
#define GPS_EPOCH_DAY        6
#define SECONDS_IN_WEEK      604800
#define GPS_UNIX_EPOCH_DIFF  315964800
#define LEAP_SECONDS 19

//Port
#define CC0M2                0x1FA0

//Size
#define BESTPOS_SIZE        104
#define BESTXYZ_SIZE        144

enum MESSAGE_ID{
	IMUratePVAS = 0x519,
	INSPVAS = 0x1FC,
	RawIMUs = 0x145,
	bestPos = 0x2A,
	bestXYZ = 0xF1,
	hwmonitor = 0x3C3,
};

enum TRIGGER{
    ONNEW = 0,
    ONCHANGED = 1,
    ONTIME = 2,
    ONNEXT = 3,
    ONCE = 4,
    ONMARK = 5

};

//uint8_t	RawIMUs_len = 52;
//uint8_t	IMUratePVAS_Len = 100;
//uint8_t	INSPVAS_len = 100;
//uint8_t	bestPos_len = 104;
//uint8_t bestXYZ_len = 130;
//uint8_t hwmonitor_len = 90;


uint8_t Header[3]  = {0xAA,0x44,0x12};



typedef struct
{

	uint8_t src_addr;
	uint8_t dest_addr;
	uint8_t reserved;
	uint8_t data_page;
	uint8_t pdu_form;
	uint8_t prior;
	uint8_t header[8];
	uint8_t len;
} CANFrame;


class OEM719 {
private:

	NEWUART  *OEMUART;
	newPortCAN  *OEMCAN;


	CAN_TxHeaderTypeDef *TxHeader;
	struct tm *utc;

// Rx_Buffer
	uint8_t Buffer[BUFF_LEN];
	uint8_t command[100];
	uint8_t log[64] = {0xAA, 0x44, 0x12, 0x1C, 0x01, 0x00, 0x00, 0xC0,
					   0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0xA0, 0x1F, 0x00, 0x00,
					   0x2A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	CANFrame _rx_buffer,_tx_buffer;
	uint8_t _self_addr,_pdu_num;
	uint16_t id,log_id,command_id;
	uint16_t Week;
	uint32_t Msec;
	uint32_t ReceiverStatus;
	uint32_t unix;
	uint32_t port;
	uint32_t trigger;
	unsigned long crc;
	double period;



public:
	OEM719(newPortCAN *can,uint8_t self_id,uint8_t pdu_no)
	{
		OEMCAN = can;
		_self_addr = self_id;
		_pdu_num = pdu_no;

	}
	uint32_t encode_id(uint8_t prior, uint8_t pdu_form, uint8_t src_addr,
					   uint8_t dest_addr)
	{
		uint32_t can_id = 0;
		can_id |= (prior & 0x07) << 26; // First 3 bits
		can_id |= (pdu_form & 0xFF) << 16;   // Next 8 bits
		can_id |= (dest_addr & 0xFF) << 8;  // Next 8 bits
		can_id |= src_addr & 0xFF;		   // Last 8 bits
		return can_id;
	}

	void binary_command(uint32_t Port,uint16_t comm_id)
	{


	}

	void binary_log(uint32_t Port,uint16_t txid,uint32_t trig,double Period)
	{
		port = Port;
		log_id = txid;
		trigger = trig;
		period = Period;

		memcpy(log+HEADER_LENGTH,&port,4);
		memcpy(log+HEADER_LENGTH+4,&log_id,2);
		memcpy(log+HEADER_LENGTH+8,&trigger,4);
		memcpy(log+HEADER_LENGTH+12,&period,8);

		crc = CalculateBlockCRC32(60, log, 0);
		memcpy(log+HEADER_LENGTH+32,&crc,4);

		TxMessage(log, sizeof(log));

	}
	void decode_id(uint32_t can_id)
	{
		_rx_buffer.prior = (can_id >> 26) & 0x07; // First 3 bits
		_rx_buffer.reserved = (can_id >> 25) & 0x00;
		_rx_buffer.data_page = (can_id >> 24) & 0x00;
		_rx_buffer.pdu_form = (can_id >> 16) & 0xFF;			  // Next 8 bits
		_rx_buffer.dest_addr = (can_id >> 8) & 0xFF;			  // Next 8 bits
		_rx_buffer.src_addr = can_id & 0xFF;				  // Last 8 bits
	}


	void Boost(GPIO_TypeDef *gpio, uint16_t Pin )
	{
		HAL_GPIO_WritePin(gpio, Pin, GPIO_PIN_RESET);
		HAL_Delay(300);
		HAL_GPIO_WritePin(gpio, Pin, GPIO_PIN_SET);
	}

	void TxMessage(uint8_t data[], uint8_t size)
	{

		uint32_t TxMailBox;
		TxHeader->ExtId = 0x4061C11;
		TxHeader->RTR = CAN_RTR_DATA;
		TxHeader->IDE = CAN_ID_EXT;

		TxHeader->DLC = 8;
		uint16_t numFrames = (size + 8 - 1) / 8;

		for (uint16_t frameIndex = 0; frameIndex < numFrames; frameIndex++)
		{


		    if (frameIndex == (numFrames - 1))
		    {
		      TxHeader->DLC = ((size % 8));
		      if (TxHeader->DLC == 0)
		      {
		        TxHeader->DLC = 8;
		      }
		    }

//		    if (HAL_CAN_AddTxMessage(OEMCAN, TxHeader, (data + (frameIndex * 8)), &TxMailBox) != HAL_OK)
//		    {
//		      Error_Handler();
//		    }
		    OEMCAN->writeBytes(TxHeader, (data + (frameIndex * 8)), &TxMailBox);

		    LoopDelay(700);

		  }
	}


	uint8_t RxMessage(){
		uint8_t RxMsg[8];
		uint8_t size;
		static uint8_t RxIndex = 0;


		OEMCAN->readBytes(OEMCAN->get_rx_head(), RxMsg);

//		if(HAL_CAN_GetRxMessage(CAN, Rxfifo, RxHeader, RxMsg) != HAL_OK)
//		{
//			__disable_irq();
//			while (1){}
//		}
		/*
		 * incoming new message
		 * clear buffer
		 */

	if (deviceCheck(OEMCAN->get_rx_head()) == true)
	{
			if (newMessageCheck(RxMsg) == true)
			{
				//ClearBuffer();
				RxIndex = 0;

			}


				memcpy(Buffer+RxIndex,RxMsg,OEMCAN->get_rx_head()->DLC);
				RxIndex += OEMCAN->get_rx_head()->DLC;
				size = sizeCheck();
				if(RxIndex == size)
				{
					GetData();
				}
				return 1;

	}
	else
	{
		return 0;
	}
		// copy data to buffer
//			memcpy(Buffer+RxIndex,RxMsg,8);
//			RxIndex += 8;
	}

	void RxMessage(uint8_t *RxMsg,uint8_t len){
//				uint8_t size;
				static uint8_t RxIndex = 0;

			if (newMessageCheck(RxMsg) == true)
			{
				GetData();
			//	ClearBuffer();
				RxIndex = 0;

			}


				memcpy(Buffer+RxIndex,RxMsg,len);
				RxIndex += len;
//				size = sizeCheck();
//				if(RxIndex == size)
//				{
//					GetData();
//				}



	}


	void getData(uint8_t ShiftBytes, uint8_t Size, void* Data)
	{
			memcpy(Data,Buffer+ShiftBytes,Size);
	}

	void getBestPos()
	{

			Time(FULL_HEADER);
			Receiver_Status();
//		if (Buffer[bestPos_len-1] != 0)
//		{
			oem719_reg[bestPosId].set_bytes(Buffer+36 , 24,LittleEndian);
			//BESTPOS->set_bytes(Buffer+36 , 24);
//		}

	}

	void getBestXYZ()
	{

			Time(FULL_HEADER);
			Receiver_Status();
			LittleEndianCpy((uint8_t *)OEM_BestXYZ_D+24, 24, Buffer+80);
			//oem719_reg[bestXYZId].set_bytes_EXT(Buffer+80,24,24);
			oem719_reg[bestXYZId].set_bytes(Buffer+36,24,LittleEndian);
			//memcpy(OEM_BestXYZ_D+24,Buffer+80,24);
			//oem719_reg[bestXYZId].set_bytes_EXT(Buffer+80,24,24);


	}

	void getTemp()
	{

			Time(FULL_HEADER);
			Receiver_Status();
			//getData(HEADER_LENGTH+4, 4, temp);
			oem719_reg[hwmonitorId].set_bytes(Buffer+32,4,LittleEndian);

	}

	void getRawIMUs()
	{

//			getData(SHORT_HEADER_LENGTH+12, 4,  rawimu->IMU_Status);
//			getData(SHORT_HEADER_LENGTH+16, 12, rawimu->Accel);
//			getData(SHORT_HEADER_LENGTH+28, 12, rawimu->Angular_Velcity);
			oem719_reg[RawIMUsId].set_bytes(Buffer+24,4,LittleEndian);
			oem719_reg[RawIMUsId].set_bytes_EXT(Buffer+28,12,12);
			oem719_reg[RawIMUsId].set_bytes_EXT(Buffer+40,12,24);



	}

	void getGyro()
	{

			Time(SHORT_HEADER);
			oem719_reg[INSPVASId].set_bytes(Buffer+72,24,LittleEndian);
			//getData(SHORT_HEADER_LENGTH+60, 24, Gyro);

	}



//	time_t GPSTime_to_UTCTime(uint16_t gps_week, uint32_t gps_sec)
//	{
//	    struct tm gps_epoch = {0};
//	    gps_epoch.tm_year = GPS_EPOCH_YEAR - 1900;
//	    gps_epoch.tm_mon = GPS_EPOCH_MONTH-1;
//	    gps_epoch.tm_mday = GPS_EPOCH_DAY;
//	    time_t gps_epoch_time = mktime(&gps_epoch);
//
//	    time_t time_difference = (gps_week-1024) * SECONDS_IN_WEEK + (time_t)gps_sec;
//
//	    return (gps_epoch_time + time_difference);
//	}
//
//	void Time(bool HeaderType)
//	{
//
//		if(HeaderType == FULL_HEADER)
//		{
//			getData(14, 2, &Week);
//			getData(16, 4, &Msec);
//
//		}
//		else
//		{
//			getData(6,2, &Week);
//			getData(8,4, &Msec);
//		}
//		unix = GPSTime_to_UTCTime(Week, ((Msec/1000)-19));
//		utc = gmtime(&unix_time);
//		utc->tm_year = utc->tm_year +1900;
//		utc->tm_mon = utc->tm_mon+1;
//		utc->tm_hour  = utc->tm_hour + 7 ;
//
//		oem719_reg[Unix].set_bytes((uint8_t*)&unix,8);
//
//	}

	uint32_t GPStoUnix(uint16_t WEEK,uint32_t SEC)
	{
		uint32_t time = (WEEK*SECONDS_IN_WEEK)+SEC+GPS_UNIX_EPOCH_DIFF-LEAP_SECONDS;
		return time;
	}

	void Time(bool HeaderType)
	{

		if(HeaderType == FULL_HEADER)
		{
			getData(14, 2, &Week);
			getData(16, 4, &Msec);

		}
		else
		{
			getData(6,2, &Week);
			getData(8,4, &Msec);
		}
		Week = Week-1024;
		unix = GPStoUnix(Week, (Msec/1000));
		oem719_reg[Unix].set_bytes((uint8_t*)&unix,4,LittleEndian);


	}
//	void PrintTime(struct tm *utc_tm)
//	{
//	    printf("UTC Time: %d-%02d-%02d %02d:%02d:%02d\n",
//	           utc_tm->tm_year + 1900,
//	           utc_tm->tm_mon + 1,
//	           utc_tm->tm_mday,
//	           utc_tm->tm_hour,
//	           utc_tm->tm_min,
//	           utc_tm->tm_sec);
//	}

	void Receiver_Status()
	{
		oem719_reg[receiverstat].set_bytes(Buffer+20, 4,LittleEndian);
		//getData(20, 4, &ReceiverStatus);
	}

	void ClearBuffer()
	{
		memset(Buffer,0,sizeof(Buffer));
	}

	uint8_t sizeCheck()
	{
		uint16_t Id = getID();
		 switch(Id){
		 	 case IMUratePVAS:
			 	 return 0;

		 	 case INSPVAS:
		 		 getGyro();
			 	 return 1;

		 	 case RawIMUs:
		 		 getRawIMUs();
			 	 return 2;

		 	 case bestPos:
		 		 getBestPos();
			 	 return BESTPOS_SIZE;

		 	 case bestXYZ:
		 		 getBestXYZ();
			 	 return BESTXYZ_SIZE;

		 	 case hwmonitor:
		 		 getTemp();
			 	 return 108;


		 	 default:
		 		 return 8;

		 }
		return 10;

	}


	uint8_t GetData()
	{

		uint16_t Id = getID();
		 switch(Id){
		 	 case IMUratePVAS:
			 	 return 0;

		 	 case INSPVAS:
		 		 getGyro();
			 	 return 1;

		 	 case RawIMUs:
		 		 getRawIMUs();
			 	 return 2;

		 	 case bestPos:
		 		 getBestPos();
			 	 return 3;

		 	 case bestXYZ:
		 		 getBestXYZ();
			 	 return 4;

		 	 case hwmonitor:
		 		 getTemp();
			 	 return 5;


		 	 default:
		 		 return 8;

		 }
		return 10;
	}

	uint16_t getID()
	{

		 memcpy(&id,Buffer+MESSAGE_ID_BYTES1,2);
		 return id;
	}

	uint8_t newMessageCheck(uint8_t Msg[])
	{


		if ((Msg[0] == Header[0] ) && (Msg[1] == Header[1]) && (Msg[2]  == Header[2]))
		{

			return 1;
		}
		return 0;

	}

	uint8_t deviceCheck(CAN_RxHeaderTypeDef *header)
	{
		decode_id(header->ExtId); /*decode 29 bit id*/

		if ((_rx_buffer.dest_addr != _self_addr) || (_rx_buffer.pdu_form != _pdu_num))
		{

			return 0;
		}

		return 1;
	}

	uint8_t deviceCheck(uint32_t ext_id)
	{
		decode_id(ext_id); /*decode 29 bit id*/

		if ((_rx_buffer.dest_addr != _self_addr) || (_rx_buffer.pdu_form != _pdu_num))
		{

			return 0;
		}

		return 1;
	}

	 unsigned long CRC32Value(int i)
	 {
		 int j;
		 unsigned long ulCRC;
	     ulCRC = i;
	     for ( j = 8 ; j > 0; j-- ) {
	      if ( ulCRC & 1 )
	               ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
	      else
	               ulCRC >>= 1;
	     }
	     return ulCRC;
	}

	 unsigned long CalculateBlockCRC32( unsigned long ulCount, unsigned char*ucBuffer,bool enDianSwap )
	 {
		 unsigned long ulTemp1;
		 unsigned long ulTemp2;
		 unsigned long ulCRC = 0;
		 while ( ulCount-- != 0 )
		 {
	           ulTemp1 = ( ulCRC >> 8 ) & 0x00FFFFFFL;
	           ulTemp2 = CRC32Value( ((int) ulCRC ^ *ucBuffer++ ) & 0xFF );
	           ulCRC = ulTemp1 ^ ulTemp2;
		 }
		 if(enDianSwap == 1)
		 {
			return (ulCRC << 24) | ((ulCRC << 8) & 0x00FF0000) | ((ulCRC >> 8) & 0x0000FF00) | (ulCRC >> 24);
		 }
		 return( ulCRC );
	  }


};



#endif /* INC_OEM719_H_ */
