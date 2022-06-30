
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  ulora.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jun 1, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef ULORA_H_
#define ULORA_H_


#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ULORA_ACK_NAK_SIZE		0x01
#define ULORA_CONN_COUNT_SIZE	0x04

typedef enum
{
	Ulora_OK = 0x00U,
	Ulora_ERROR,
	Ulora_BUSY,
	Ulora_TIMEOUT
} Ulora_Status_t;


typedef enum
{
	ULORA_UNKNOWN = -1,
	ULORA_NAK ,
	ULORA_ACK,
	ULORA_CONN_COUNT,
	ULORA_PIR_SENS,
	ULORA_MAX_ID

} UloraCommand_e;

typedef struct
{
	bool 					uloraPirDetection;
	bool 					uloraActivate;
	uint8_t					uloraPacketSize;
	uint8_t					uloraDevicesCount;

}ULoraHandler_t;

extern ULoraHandler_t 	hUloraProtocol;

uint8_t 				uloraPacketChecksum		(uint8_t* buffer, uint8_t packetSize);
Ulora_Status_t 			uloraCommunicationTest	(uint8_t* hModuleTransmitter);
Ulora_Status_t 			uloraPirStatusLoad		(uint8_t* payloadBuffer);
uint8_t 				uloraPacketStore		(uint8_t* buffer);

#endif /* ULORA_H_ */
