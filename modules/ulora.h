/*
 * ulora.h
 *
 *  Created on: May 17, 2022
 *      Author: Ward
 */

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
	ULORA_NAK = 0x00U,
	ULORA_ACK,
	ULORA_CONN_COUNT,

} UloraCommand_e;

typedef struct
{
	bool 					uloraActivate;
	uint8_t					uloraPacketSize;
	uint8_t					uloraDevicesCount;


}ULoraHandler_t;


uint8_t 		uloraPacketChecksum		(uint8_t* buffer, uint8_t packetSize);
Ulora_Status_t 	uloraCommunicationTest	(uint8_t* hModuleTransmitter);

#endif /* ULORA_H_ */
