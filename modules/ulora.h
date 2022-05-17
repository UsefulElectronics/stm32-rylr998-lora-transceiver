/*
 * ulora.h
 *
 *  Created on: May 17, 2022
 *      Author: Ward
 */

#ifndef ULORA_H_
#define ULORA_H_

#include <rylr998.h>

#define ULORA_NAK				0x00
#define ULORA_ACK				0x01
#define ULORA_CONN_COUNT		0x02

typedef enum
{
	Ulora_OK = 0x00U,
	Ulora_ERROR,
	Ulora_BUSY,
	Ulora_TIMEOUT
} Ulora_Status_t;


typedef struct
{
	bool 					uloraActivate;
	uint32_t				uloraTimer;
	uint8_t					uloraPacketSize;
	uint8_t					uloraChecksum;
	uint8_t					uloraDevicesCount;

}ULoraHandler_t;


uint8_t 		uloraPacketChecksum		(uint8_t* buffer, uint8_t packetSize);
Ulora_Status_t 	uloraCommunicationTest	(Rylr998Transmitter_t* hModuleTransmitter);

#endif /* ULORA_H_ */
