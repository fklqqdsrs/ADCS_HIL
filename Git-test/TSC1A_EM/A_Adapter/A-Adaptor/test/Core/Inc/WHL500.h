/*
 * WHL500.h
 *
 *  Created on: Mar 8, 2024
 *      Author: Chinn
 */

#ifndef INC_WHL500_H_
#define INC_WHL500_H_

#include "General.h"
#include "newPortCAN.h"
#include "WHL500_REG.h"

// length
#define RX_BUFFER_LENGTH  50
#define TX_BUFFER_LENGTH  50

#define CRC_ENABLE 1

#define AUTO 1024
// Priority
#define PRIOR_CRITICAL 0
#define PRIOR_HIGH     1
#define PRIOR_NORM     2
#define PRIOR_LOW      3

// Source and destination port
#define CSP_BUILT_IN_SERVICE_1  0
#define PING                    1
#define CSP_BUILT_IN_SERVICE_2  2
#define REBOOT					4
#define CSP_BUILT_IN_SERVICE_3  5
#define PARAMETER_SERVICE		10
#define PARAMETER_LIST_SERVICE  12
#define VMEM_SERVICE			14
#define EPHEMERAL_SOURCE_PORT	32 /*32-63*/

// Param Flag
#define PARAM_FLAG_DEFAULT  00
#define PARAM_FLAG_END (1 << 7)

//Command ID
#define	PARAM_PULL_REQUEST 	 0
#define	PARAM_PULL_RESPONSE  1
#define	PARAM_PUSH_REQUEST 	 2
#define	PARAM_PUSH_RESPONSE  3        // Push response is an ack, and is same for v1 and v2.
#define	PARAM_PULL_ALL_REQUEST  4     // Pull all request is same for v1 and v2.
#define	PARAM_PULL_REQUEST_V2   5
#define PARAM_PULL_RESPONSE_V2  6
#define PARAM_PUSH_REQUEST_V2   7
#define PARAM_PULL_ALL_REQUEST_V2  8

//PARAM TYPE
#define PARAM_TYPE_INT8    0xD0
#define PARAM_TYPE_INT16   0xD1
#define PARAM_TYPE_INT32   0xD2
#define PARAM_TYPE_INT64   0xD3
#define PARAM_TYPE_UINT8   0x00
#define PARAM_TYPE_UINT16  0xCD
#define PARAM_TYPE_UINT32  0xCE
#define PARAM_TYPE_UINT64  0XCF
#define PARAM_TYPE_FLOAT32 0xCA
#define PARAM_TYPE_FLOAT64 0xCB
#define PARAM_TYPE_XINT16  0xC8
#define PARAM_GET          0xFF


#define GET PARAM_PULL_REQUEST_V2
#define SET PARAM_PUSH_REQUEST_V2



typedef struct
{
	uint8_t source_id;
	uint8_t destination_id;
	uint8_t start;
	uint8_t remaining;
	uint16_t CFP_id;
}CANExtId_Typedef;

typedef struct
{
    uint8_t prior;
    uint8_t source_id;
    uint8_t destination_id;
    uint8_t destination_port;
    uint8_t source_port;
    uint8_t reserved;
    uint8_t CRC_enable;
}Message_HeaderTypeDef;


typedef struct
{
	uint8_t  Command_Id;
	uint8_t  flag;
	uint16_t Param_Id;
	uint8_t Array_flag;
	uint8_t Node_flag;
	uint8_t TimeStamp_flag;
	uint8_t Extended_flag;
	uint8_t Reserved;
	uint32_t Node;

}Payload;


class WHL500 {

private:

	uint8_t RxBuffer[RX_BUFFER_LENGTH];
	uint8_t TxBuffer[TX_BUFFER_LENGTH];
	uint8_t prior,src_id,dest_id,src_port,dest_port,vartype,WHLID;
	uint16_t CFP,Param  ;
	uint32_t speed;
	uint32_t VAL;
	float   fVAL;
	//void *value;
	newPortCAN *WHLCAN;
	CAN_TxHeaderTypeDef *TxHeaderWhl;
	CAN_RxHeaderTypeDef *RxHeaderWhl;
	Message_HeaderTypeDef *Msg_header;
	CANExtId_Typedef *rx_id,*tx_id;
	Payload *PL;
	WHLREG *reg;


public:



	WHL500(newPortCAN *can, WHLREG *whlReg, uint8_t _source_id)
	{
		WHLCAN = can;
		src_id = _source_id;
		reg = whlReg;

	}

	void SetDeviceIDfilter(uint8_t whlid)
	{
		WHLID = whlid;
	}

	void RegSetup()
	{
		reg->whl500_reg_setup();
	}

	void SetNewID(uint8_t _source_id)
	{
		src_id = _source_id;
	}

	CAN_TxHeaderTypeDef *getCANTxHeader()
	{
		return TxHeaderWhl;
	}

	CAN_RxHeaderTypeDef *getCANRxHeader()
	{
		return RxHeaderWhl;
	}

	void VEOWARETxMessage(uint8_t data[], uint8_t size, uint16_t new_cfp)
	{

		uint32_t TxMailBox;
		tx_id->source_id = src_id;
		tx_id->destination_id = dest_id;
		tx_id->CFP_id = CFP;

	    TxHeaderWhl->RTR = CAN_RTR_DATA;
	    TxHeaderWhl->IDE = CAN_ID_EXT;
		TxHeaderWhl->DLC = 8;

		uint16_t numFrames = (size + 8 - 1) / 8;

		for (uint16_t frameIndex = 0; frameIndex < numFrames; frameIndex++)
		{


		    if (frameIndex == (numFrames - 1) )
		    {


			    tx_id->remaining = 0;
			    tx_id->start = 1;

		      TxHeaderWhl->DLC = (size % 8);
		      if (TxHeaderWhl->DLC == 0)
		      {
		        TxHeaderWhl->DLC = 8;
		      }
			    TxHeaderWhl->ExtId = CANIdEncode(tx_id);
			    WHLCAN->writeBytes(TxHeaderWhl, (data + (frameIndex * 8)), &TxMailBox);
			    LoopDelay(700);

			    if(src_port >= 14 && src_port < 63)
			    {
			    	src_port += 1;
			    }
			    else
			    {
			    	src_port = 14;
			    }
			    if(new_cfp == AUTO)
			    {
			    	CFP = (CFP < 1023)?(CFP+1): 0;
			    	tx_id->CFP_id = CFP;
			    }
			    else
			    {
				   CFP = new_cfp;
				   tx_id->CFP_id = CFP;
			    }

		    }
		    else
		    {
		    	tx_id->remaining = (numFrames-1)- frameIndex;
		    	if (frameIndex == 0)
		    	{
		    		tx_id->start = 0;
		    	}
		    	else
		    	{
		    		tx_id->start = 1;
		    	}
			    TxHeaderWhl->ExtId = CANIdEncode(tx_id);
			    WHLCAN->writeBytes(TxHeaderWhl, (data + (frameIndex * 8)), &TxMailBox);
			    LoopDelay(700);

		    }


		  }
	}



	void SetMessageParam(uint8_t _prior,uint8_t _des_id,uint8_t _dst_port, uint8_t _src_port,uint16_t _cfp)
	{
		prior = _prior;
		dest_id = _des_id;
		dest_port = _dst_port;
		src_port = _src_port;
	    CFP = _cfp;


	}

	Payload PayloadConfig(uint8_t com_id, uint8_t Flag,uint32_t param_id, uint32_t array_flag, uint32_t node_flag,
						uint32_t time_flag, uint32_t extended, uint32_t node)
	{
		Payload pl;
		pl.Command_Id = com_id;
		pl.flag = Flag;
		pl.Param_Id = param_id;
		pl.Array_flag = array_flag;
		pl.Node_flag = node_flag;
		pl.TimeStamp_flag = time_flag;
		pl.Extended_flag = extended;
		pl.Reserved = 0;
		pl.Node = node;
		return pl;
	}

	void SetPayload(uint8_t com_id, uint8_t Flag,uint32_t param_id, uint32_t array_flag, uint32_t node_flag,
						uint32_t time_flag, uint32_t extended, uint32_t node)
	{

		PL->Command_Id = com_id;
		PL->flag = Flag;
		PL->Param_Id = param_id;
		PL->Array_flag = array_flag;
		PL->Node_flag = node_flag;
		PL->TimeStamp_flag = time_flag;
		PL->Extended_flag = extended;
		PL->Reserved = 0;
		PL->Node = node;

	}

	void ShortSetPayload(uint8_t com_id,uint16_t param_id)
	{

		PL->Command_Id = com_id;
		PL->flag = PARAM_FLAG_DEFAULT;
		PL->Param_Id = param_id;
		PL->Array_flag = 0;
		PL->Node_flag = 1;
		PL->TimeStamp_flag = 0;
		PL->Extended_flag = 0;
		PL->Reserved = 0;
		PL->Node = dest_id;

	}

	uint32_t CANIdEncode(uint8_t src_id,uint8_t dest_id,uint8_t start,uint8_t remain,uint8_t cfp)
	{
		uint32_t ident;
	    ident &= ~(0xFFFFFFFF);
	    ident |= (src_id & 0x1F)  << 24;
	    ident |= (dest_id & 0x1F) << 19;
	    ident |= (start & 0x01)  << 18;
	    ident |= (remain & 0xFF) << 10;
	    ident |= cfp & 0x3FF;
	    return ident;
	}

	uint32_t CANIdEncode(CANExtId_Typedef *ExtId)
	{
		uint32_t ident;
	    ident &= ~(0xFFFFFFFF);
	    ident |= (ExtId->source_id & 0x1F)  << 24;
	    ident |= (ExtId->destination_id & 0x1F) << 19;
	    ident |= (ExtId->start & 0x01)  << 18;
	    ident |= (ExtId->remaining & 0xFF) << 10;
	    ident |=  ExtId->CFP_id & 0x3FF;
	    return ident;
	}

	void CANRxIdDecode(uint32_t extid )
	{
		rx_id->source_id = (extid >> 24) & 0x1F;
		rx_id->destination_id = (extid >> 19) & 0x1F;
		rx_id->start = (extid >> 18) & 0x01;
		rx_id->remaining = (extid >> 10) & 0xFF;
		rx_id->CFP_id =  extid & 0x3FF;
	}


	uint32_t MessageHeaderEncode(uint8_t prior,uint8_t src_id,uint8_t dest_id,uint8_t dest_port,
								 uint8_t src_port,uint8_t crc_en)
	{

	    uint32_t Header;
	    Header &= ~(0xFFFFFFFF);
	    Header |= (prior  & 0x03)  << 30;
	    Header |= (src_id  & 0x1F)  << 25;
	    Header |= (dest_id  & 0x1F)  << 20;
	    Header |= (dest_port  & 0x3F)  << 14;
	    Header |= (src_port  & 0x3F)  << 8;
//	    Header |= (Msg_header->reserved  & 0x00)  << 4;
	    Header |= crc_en & 0x01 ;
	    return Header;
	}

	uint32_t MessageHeaderEncode(Message_HeaderTypeDef *header)
	{

	    uint32_t Header;
	    Header &= ~(0xFFFFFFFF);
	    Header |= (header->prior  & 0x03)  << 30;
	    Header |= (header->source_id  & 0x1F)  << 25;
	    Header |= (header->destination_id  & 0x1F)  << 20;
	    Header |= (header->destination_port  & 0x3F)  << 14;
	    Header |= (header->source_port  & 0x3F)  << 8;
//	    Header |= (Msg_header->reserved  & 0x00)  << 4;
	    Header |= header->CRC_enable & 0x01 ;

	    return Header;
	}


	void MessageHeaderDecode(uint32_t header )
	{
		Msg_header->prior = (header >> 30) & 0x03;
		Msg_header->source_id =  (header >> 25) & 0x1F;
		Msg_header->destination_id  = (header >> 20) & 0x1F;
		Msg_header->destination_port  = (header >> 14) & 0x3F;
		Msg_header->source_port  = (header >> 8) & 0x3F;
		Msg_header->reserved  = (header >> 4) & 0x00;
		Msg_header->CRC_enable = header & 0x01;
		BigEndian4bytesCpy(TxBuffer, MessageHeaderEncode(Msg_header));

	}

	void RxMessage(){
		uint8_t RxMsg[8];
		static uint8_t RxIndex = 0;

		WHLCAN->readBytes(WHLCAN->get_rx_head(), RxMsg);

		/*
		 * incoming new message
		 * clear buffer
		 */

		if (deviceCheck() == true)
		{
			if (newMessageCheck() == true)
			{

				ClearRxBuffer();
				RxIndex = 0;

			}


				memcpy(RxBuffer+RxIndex,RxMsg,8);
				RxIndex += 8;

				getValue();


		}

	}



//	void SendCommand(uint8_t com_id, uint16_t param_id, uint8_t VarType, void* value)
	void SendCommand(uint8_t com_id, uint16_t param_id, uint8_t VarType, float val)
	{
		void* value;
		if( VarType == PARAM_TYPE_FLOAT32 )
		{
			fVAL = val;
			value = (void*)& fVAL;

		}
		else
		{
			VAL = static_cast<uint32_t>(val);
			value = (void*)& VAL;
		}
//		void* value = (void*)& fVAL;
		MessageHeaderDecode(MessageHeaderEncode(prior, src_id, dest_id, PARAMETER_SERVICE ,src_port, CRC_ENABLE));
		ShortSetPayload(com_id, param_id);
		TxBuffer[6] = PL->Command_Id;
		TxBuffer[7] = PL->flag;
		TxBuffer[8] = PARAM_TYPE_UINT16;

		uint32_t Payload_ID;
		Payload_ID &= ~(0xFFFFFFFF);

	    Payload_ID |= PL->Param_Id  << 24;
	    Payload_ID |= (PL->Param_Id & 0x300) << 8;
	    Payload_ID |= PL->Array_flag << 23;
	    Payload_ID |= PL->Node_flag<< 22;
	    Payload_ID |= PL->TimeStamp_flag << 21;
	    Payload_ID |= PL->Extended_flag  << 20;
	    Payload_ID |= PL->Reserved  << 18;
	    Payload_ID |= PL->Node & 0xFFFF ;
	    if (TxBuffer[6] == GET)
	    {
	    	TxBuffer[5] =  11;
	    	BigEndian4bytesCpy(TxBuffer+9, Payload_ID);
        	BigEndian4bytesCpy(TxBuffer+13, Castagnoli_CRC32c(TxBuffer+6, 7));
        	VEOWARETxMessage(TxBuffer, 17, AUTO);
        	ClearTxBuffer();

	    }


	    BigEndian4bytesCpy(TxBuffer+9, Payload_ID);
	    TxBuffer[13] = VarType;

	    switch(VarType) {
	        case PARAM_TYPE_INT8:
	        	TxBuffer[5] =  13;
	        	TxBuffer[14] = *((int8_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+15, Castagnoli_CRC32c(TxBuffer+6, 9));
	        	VEOWARETxMessage(TxBuffer, 19, AUTO);
	        	ClearTxBuffer();
	            break;
	        case PARAM_TYPE_INT16:
	        	TxBuffer[5] = 14;
	        	BigEndian2bytesCpy(TxBuffer+14,*(uint16_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+16, Castagnoli_CRC32c(TxBuffer+6,10));
	        	VEOWARETxMessage(TxBuffer, 20, AUTO);
	        	ClearTxBuffer();
	            break;
	        case PARAM_TYPE_INT32:
	        	TxBuffer[5] = 16;
	        	BigEndian4bytesCpy(TxBuffer+14,*(uint32_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+18, Castagnoli_CRC32c(TxBuffer+6, 12));
	        	VEOWARETxMessage(TxBuffer, 22, AUTO);
	        	ClearTxBuffer();
	            break;
	        case PARAM_TYPE_INT64:
	        	TxBuffer[5] = 20;
	        	BigEndian8bytesCpy(TxBuffer+14,*(uint64_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+22, Castagnoli_CRC32c(TxBuffer+6, 16));
	        	VEOWARETxMessage(TxBuffer, 26, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_UINT8:
	        	TxBuffer[5] = 12;
	        	TxBuffer[13] = *((uint8_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+14, Castagnoli_CRC32c(TxBuffer+6, 8));
	        	VEOWARETxMessage(TxBuffer, 18, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_UINT16:
	        	TxBuffer[5] = 14;
	        	BigEndian2bytesCpy(TxBuffer+14,*(uint16_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+16, Castagnoli_CRC32c(TxBuffer+6, 10));
	        	VEOWARETxMessage(TxBuffer, 20, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_UINT32:
	        	TxBuffer[5] = 16;
	        	BigEndian4bytesCpy(TxBuffer+14, *(uint32_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+18, Castagnoli_CRC32c(TxBuffer+6, 12));
	        	VEOWARETxMessage(TxBuffer, 22, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_UINT64:
	        	TxBuffer[5] = 20;
	        	BigEndian8bytesCpy(TxBuffer+14,*(uint64_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+22, Castagnoli_CRC32c(TxBuffer+6, 16));
	        	VEOWARETxMessage(TxBuffer, 26, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_FLOAT32:
	        	TxBuffer[5] = 16;
	        	BigEndian4bytesCpy(TxBuffer+14,*(uint32_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+18, Castagnoli_CRC32c(TxBuffer+6, 12));
	        	VEOWARETxMessage(TxBuffer, 22, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_FLOAT64:
	        	TxBuffer[5] = 20;
	        	BigEndian8bytesCpy(TxBuffer+14,*(uint64_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+22, Castagnoli_CRC32c(TxBuffer+6, 16));
	        	VEOWARETxMessage(TxBuffer, 26, AUTO);
	        	ClearTxBuffer();
	        	break;
	        case PARAM_TYPE_XINT16:
	        	TxBuffer[5] = 14;
	        	BigEndian2bytesCpy(TxBuffer+14,*(uint16_t*)value);
	        	BigEndian4bytesCpy(TxBuffer+16, Castagnoli_CRC32c(TxBuffer+6, 10));
	        	VEOWARETxMessage(TxBuffer, 20, AUTO);
	        	ClearTxBuffer();
	        	break;
	    }
	}
	void SendCommand(uint8_t com_id, uint16_t param_id)
	{


		MessageHeaderDecode(MessageHeaderEncode(prior, src_id, dest_id, PARAMETER_SERVICE ,src_port, CRC_ENABLE));
		ShortSetPayload(com_id, param_id);
		TxBuffer[6] = PL->Command_Id;
		TxBuffer[7] = PL->flag;
		TxBuffer[8] = PARAM_TYPE_UINT16;

		uint32_t Payload_ID;
		Payload_ID &= ~(0xFFFFFFFF);

	    Payload_ID |= PL->Param_Id  << 24;
	    Payload_ID |= (PL->Param_Id & 0x300) << 8;
	    Payload_ID |= PL->Array_flag << 23;
	    Payload_ID |= PL->Node_flag<< 22;
	    Payload_ID |= PL->TimeStamp_flag << 21;
	    Payload_ID |= PL->Extended_flag  << 20;
	    Payload_ID |= PL->Reserved  << 18;
	    Payload_ID |= PL->Node & 0xFFFF ;
	    if (TxBuffer[6] == GET)
	    {
	    	TxBuffer[5] =  11;
	    	BigEndian4bytesCpy(TxBuffer+9, Payload_ID);
        	BigEndian4bytesCpy(TxBuffer+13, Castagnoli_CRC32c(TxBuffer+6, 7));
        	VEOWARETxMessage(TxBuffer, 17, AUTO);
        	ClearTxBuffer();

	    }

	}

	void Ping (uint8_t node)
	{
		TxBuffer[5] = 4;
		MessageHeaderDecode(MessageHeaderEncode(PRIOR_NORM, src_id, node, PING ,src_port, CRC_ENABLE));
		BigEndian4bytesCpy(TxBuffer, MessageHeaderEncode(Msg_header));
		VEOWARETxMessage(TxBuffer, 10, AUTO);
		ClearTxBuffer();

	}

	void Reboot (uint8_t node)
	{
		uint32_t reboot_payload = 0x80078007;
		TxBuffer[5] = 8;
		MessageHeaderDecode(MessageHeaderEncode(PRIOR_NORM, src_id, node, REBOOT ,src_port, CRC_ENABLE));
		BigEndian4bytesCpy(TxBuffer, MessageHeaderEncode(Msg_header));
		BigEndian4bytesCpy(TxBuffer+6, reboot_payload);
		BigEndian4bytesCpy(TxBuffer+10, Castagnoli_CRC32c(TxBuffer+6, 4));
		VEOWARETxMessage(TxBuffer, 14, AUTO);
		ClearTxBuffer();

	}

	void GET_ALL()
	{
		uint64_t getall = 0xFFFFFFFF00000012;
		TxBuffer[4] = 8;
		MessageHeaderDecode(MessageHeaderEncode(PRIOR_HIGH, src_id, dest_id, PARAMETER_SERVICE ,src_port, CRC_ENABLE));
		BigEndian4bytesCpy(TxBuffer, MessageHeaderEncode(Msg_header));
		BigEndian8bytesCpy(TxBuffer+8, getall);
		BigEndian4bytesCpy(TxBuffer+16, Castagnoli_CRC32c(TxBuffer+4, 12));
		VEOWARETxMessage(TxBuffer, 20, AUTO);
		ClearTxBuffer();
	}


	void ClearTxBuffer()
	{
		memset(TxBuffer,0,sizeof(TxBuffer));
	}

	void ClearRxBuffer()
	{
		memset(RxBuffer,0,sizeof(RxBuffer));
	}
	void getValue()
	{
	  if(RxBuffer[6] == 0x06)
	  {
		Param = RxBuffer[9] ;
		Param |= (RxBuffer[10] & 0x3) << 8;
		vartype = RxBuffer[13];
	    switch(vartype) {
	        case PARAM_TYPE_INT8:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 1,BigEndian);
	            break;
	        case PARAM_TYPE_INT16:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 2,BigEndian);
	            break;
	        case PARAM_TYPE_INT32:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 4,BigEndian);
	            break;
	        case PARAM_TYPE_INT64:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 8,BigEndian);
	        	break;
	        case PARAM_TYPE_UINT16:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 2,BigEndian);
	        	break;
	        case PARAM_TYPE_UINT32:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 4,BigEndian);
	        	break;
	        case PARAM_TYPE_UINT64:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 8,BigEndian);
	        	break;
	        case PARAM_TYPE_FLOAT32:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 4,BigEndian);
	        	break;
	        case PARAM_TYPE_FLOAT64:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+14, 8,BigEndian);
	        	break;
	        default:
	        	reg->whl500_reg[Param].set_bytes(RxBuffer+13, 1,BigEndian);
	        	break;
	        Param = 0;
	    }
	  }
	}


	uint32_t Castagnoli_CRC32c(uint8_t *data, uint8_t length)
	{
	  uint32_t crc = 0xFFFFFFFF;
	  for (uint8_t i = 0; i < length; ++i) {
	    crc ^= data[i];
	    for (int j = 0; j < 8; ++j) {
	      if (crc & 1) {
	        crc = (crc >> 1) ^ 0x82F63B78;
	      } else {
	        crc >>= 1;
	      }
	    }
	  }
	  return crc ^ 0xFFFFFFFF;
	}

	uint8_t newMessageCheck()
	{
		static uint16_t old_cfp = 1024;
		static uint16_t new_cfp;
		CANRxIdDecode(RxHeaderWhl->ExtId); /*decode 29 bit id*/
		new_cfp = rx_id->CFP_id;
		if ((new_cfp != old_cfp) )
		{
			old_cfp = new_cfp;
			return 1;
		}

		return 0;

	}

	uint8_t deviceCheck()
	{
		RxHeaderWhl = WHLCAN->get_rx_head();
		CANRxIdDecode(RxHeaderWhl->ExtId); /*decode 29 bit id*/

		if ((rx_id->destination_id != src_id) || (rx_id->source_id != WHLID) )
		{

			return 0;
		}

		return 1;
	}

	void getData(uint8_t ShiftBytes, uint8_t Size, void* Data)
	{
			memcpy(Data,RxBuffer+ShiftBytes,Size);
	}



};

#endif /* INC_WHL500_H_ */
