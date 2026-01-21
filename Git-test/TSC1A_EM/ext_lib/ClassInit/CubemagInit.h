/*
 * CubemagInit.h
 *
 *  Created on: Jan 23, 2025
 *      Author: Chinnapop
 */

#ifndef INC_CUBEMAGINIT_H_
#define INC_CUBEMAGINIT_H_

#include <cubeObc/cubeObc.h>
#include <cubeObc/cubeObc_tctlmCommsMasterSvc.h>
#include "tctlmCubeMagControlProgramCommon1.h"
#include "tctlmCubeMagControlProgramDeploy1.h"
#include "tctlmCommonFramework1.h"
#include "LIB/CubeMag_REG.h"
TypeDef_TctlmEndpoint magEndPoint;



void CubemagInit()
{

	magEndPoint.nodeType = TCTLM_COMMON_FRAMEWORK_ENUMS__NODE_TYPE_CUBE_MAG_DEPLOY;
	magEndPoint.type = TYPEDEF__COMMS_ENDPOINT_CAN;
	magEndPoint.proto = TYPEDEF__COMMS_PROTOCOL_CUBESPACE;
	magEndPoint.addr = 52;
	    	//  endpoint.addrPass = 52;
	magEndPoint.timeout  = 10000;

	//ErrorCode error  =
	cubeObc_tctlmCommsMasterSvc_init(0x10);

		__SEV();


}


#endif /* INC_CUBEMAGINIT_H_ */
