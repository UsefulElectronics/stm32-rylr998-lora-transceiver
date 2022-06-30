
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  ulora.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jun 1, 2022
 * @brief   		:
 *
 ******************************************************************************/

#include "ulora.h"

#include "rylr998.h"

ULoraHandler_t hUloraProtocol;

UloraCommand_e 		uloraPacketDetermine(uint8_t idChar);
Rylr998_Status_t 	uloraChecksumValidate(uint8_t* rxBuffer);

Ulora_Status_t uloraCommunicationTest(uint8_t* payloadBuffer)
{
	ULoraHandler_t uloraHandler;
	Ulora_Status_t ret = Ulora_ERROR;
	uloraHandler.uloraPacketSize  = ULORA_CONN_COUNT_SIZE;
	uloraHandler.uloraDevicesCount = 1;

	payloadBuffer[0] = ULORA_CONN_COUNT + '0';
	payloadBuffer[1] = uloraHandler.uloraPacketSize + '0' ;
	payloadBuffer[2] = uloraHandler.uloraDevicesCount + '0';

	payloadBuffer[3] = '7';

	ret = Ulora_OK;
	return ret;
}
Ulora_Status_t uloraPirStatusLoad(uint8_t* payloadBuffer)
{
	Ulora_Status_t ret = Ulora_ERROR;
	payloadBuffer[0] = ULORA_PIR_SENS;
	payloadBuffer[1] = hUloraProtocol.uloraPirDetection;
	payloadBuffer[2] = uloraPacketChecksum(payloadBuffer, 3);
	ret = Ulora_OK;
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
uint8_t uloraPacketStore(uint8_t* buffer)
{
	UloraCommand_e packeId;
	packeId = uloraPacketDetermine(buffer[0]);
	switch (packeId)
	{
		case ULORA_CONN_COUNT:
			if(Rylr998_OK == uloraChecksumValidate(buffer))
			{
				hUloraProtocol.uloraDevicesCount = buffer[3];
			}
		case ULORA_PIR_SENS:
			//TODO Modify code segment below
			if(Rylr998_OK == uloraChecksumValidate(buffer))
			{
				hUloraProtocol.uloraDevicesCount = buffer[3];
			}
			break;
		default:
			break;
	}
	return packeId;
}
UloraCommand_e uloraPacketDetermine(uint8_t idChar)
{
	UloraCommand_e idScanner = ULORA_UNKNOWN;
	rylr998Ascii2Int(&idChar);
	for(idScanner = ULORA_UNKNOWN; idScanner < ULORA_MAX_ID; ++idScanner)
	{
		if(idScanner == idChar)
		{
			break;
		}
	}
	return idScanner;
}

Rylr998_Status_t uloraChecksumValidate(uint8_t* rxBuffer)
{
	Rylr998_Status_t ret = Rylr998_ERROR;
	uint8_t checksum = 0;
	uint8_t packetSize = rxBuffer[1] - 1 - '0';
	for(uint8_t i = 0; i < packetSize; ++i)
	{
		checksum += rxBuffer[i] - '0';
	}
	if(checksum == rxBuffer[3] - '0')
	{
		ret = Rylr998_OK;
	}
	return ret;
}
