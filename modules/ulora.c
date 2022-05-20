/*
 * ulora.c
 *
 *  Created on: May 17, 2022
 *      Author: Ward
 */


#include "ulora.h"

Ulora_Status_t uloraCommunicationTest(uint8_t* payloadBuffer)
{
	ULoraHandler_t uloraHandler;
	Ulora_Status_t ret = Ulora_ERROR;
	uloraHandler.uloraPacketSize  = ULORA_CONN_COUNT_SIZE;
	uloraHandler.uloraDevicesCount = 1;

	payloadBuffer[0] = ULORA_CONN_COUNT + '0';
	payloadBuffer[1] = uloraHandler.uloraPacketSize + '0';
	payloadBuffer[2] = uloraHandler.uloraDevicesCount + '0';
//	payloadBuffer[3] =  uloraPacketChecksum(payloadBuffer,
//											uloraHandler.uloraPacketSize) + '0';
	payloadBuffer[3] = '7';
	return ret;
}

uint8_t uloraPacketChecksum(uint8_t* buffer, uint8_t packetSize)
{
	uint8_t checksum = 0;
	packetSize 		-= 1;					//checksum should not be counted while calculating the packet checksum
	for(uint8_t i = 0; i < packetSize; ++i)
	{
		checksum += buffer[i];
	}
	return checksum;
}
