/*
 * ulora.c
 *
 *  Created on: May 17, 2022
 *      Author: Ward
 */


#include <ulora.h>

Ulora_Status_t uloraCommunicationTest(Rylr998Transmitter_t* hModuleTransmitter)
{
	ULoraHandler_t uloraHandler;
	Ulora_Status_t ret = Ulora_ERROR;
	uloraHandler.uloraPacketSize  = 3;
	uloraHandler.uloraDevicesCount = 1;


	hModuleTransmitter->payloadLength = uloraHandler.uloraPacketSize;
	hModuleTransmitter->TxBuffer[0] = ULORA_CONN_COUNT;
	hModuleTransmitter->TxBuffer[1] = uloraHandler.uloraDevicesCount;
	hModuleTransmitter->TxBuffer[2] = uloraPacketChecksum(hModuleTransmitter->TxBuffer,
															uloraHandler.uloraPacketSize);
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
