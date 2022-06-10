/*
 * rylr998.c
 *
 *  Created on: May 4, 2022
 *      Author: Ward
 */
#include <rylr998.h>


Rylr998Handler_t   hLoRaModule;

Rylr998_Status_t checksumValidate(uint8_t* rxBuffer);


Rylr998_Status_t rylr998Test(void)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	const uint8_t 		packetSize 			= 4;
	uint8_t 			uartTxBuffer[4] 	= {0};

	memcpy(uartTxBuffer, AT, packetSize);

	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, packetSize, 10);

	return ret;
}
//AT+ADDRESS=<Address>
Rylr998_Status_t rylr998SetAddress(uint8_t* address)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	const uint16_t 		packetSize 			= 14;
	char	 			uartTxBuffer[14] 	= {0};

	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat(uartTxBuffer, ADDRESS);
	strcat(uartTxBuffer, SET_VALUE);

	strcat((char*) uartTxBuffer, (char*)  address);
	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1,(uint8_t*) uartTxBuffer, packetSize, 10);

	return ret;
}
//AT+NETWORKID=<Network ID> <NetworkID>=3~15,18(default18)
Rylr998_Status_t rylr998SetNetworkId(NetworkId3_15or18_t networkId)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	uint16_t 			packetSize 		= 16;
	char 				uartTxBuffer[16] 	= {0};
	char 				tempNetworkId[2] 	= {0};

	if(((networkId >= 3) && (networkId <= 15)) || (networkId == 18))
	{
		tempNetworkId[0] = networkId + '0';				//'0' is used to convert to ASCII

		memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
		strcat(uartTxBuffer, NETWORKID);
		strcat(uartTxBuffer, SET_VALUE);

		strcat((char*) uartTxBuffer,  tempNetworkId);
		strcat((char*) uartTxBuffer, TERMINATOR);
		packetSize 		= 16;
		ret = HAL_UART_Transmit(&huart1,(uint8_t*) uartTxBuffer, packetSize, 10);
	}
	return ret;
}
//AT+CRFOP=<power> <power>0~22 dBm
Rylr998_Status_t rylr998SetOutputPower(Crfop0_22_t outpuPower)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	uint16_t 			packetSize 			= 0;
	char 				uartTxBuffer[16] 	= {0};
	char 				tempOutputPower[3] 	= {0};

	if(outpuPower <= 22)
	{
		tempOutputPower[1] = (outpuPower % 10);		//tens and units separations
		tempOutputPower[0] = (outpuPower / 10) % 10;

		tempOutputPower[1] += '0';
		tempOutputPower[0] += '0';					//'0' is used to convert to ASCII



		memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
		strcat(uartTxBuffer, CRFOP);
		strcat(uartTxBuffer, SET_VALUE);

		strcat((char*) uartTxBuffer,  tempOutputPower);
		strcat((char*) uartTxBuffer, TERMINATOR);
		packetSize 		= 13;
		ret = HAL_UART_Transmit(&huart1,(uint8_t*) uartTxBuffer, packetSize, 10);
	}
	return ret;
}
//AT+ADDRESS?
Rylr998_Status_t rylr998GetAddress(Rylr998Handler_t* hRylr998)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	const uint8_t 		packetSize 			= 13;
	uint8_t 			uartTxBuffer[13] 	= {0};

	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat((char*) uartTxBuffer,  ADDRESS);
	strcat((char*)uartTxBuffer, CHECK);
	strcat((char*)uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, packetSize, 10);


	memset(hRylr998->rylr998Receiver.rxBuffer, RESET, 20);
	ret = HAL_UART_Receive_IT(&huart1, hRylr998->rylr998Receiver.rxBuffer, 12);

	return ret;
}

//AT+SEND=<Address>,<Payload Length>,<Data>
Rylr998_Status_t rylr998Send(Rylr998Handler_t* hRylr998, UloraCommand_e uLoRaCommand)
//Rylr998_Status_t rylr998Send(uint16_t address, uint8_t payloadLength, uint8_t* txBuffer)
{
	Rylr998_Status_t 	ret 					= Rylr998_ERROR;
	uint8_t 			uartTxBuffer[256] 		= {0};
	uint16_t			packetSize				= 0;
	uint8_t				packetSizeAscii[2]		= {0};

	UloraCommand_e				Command			= uLoRaCommand;
	switch (Command)
	{
		case ULORA_NAK:

			break;
		case ULORA_ACK:
			hRylr998->rylr998Transmitter.TxBuffer[0] = ULORA_ACK;
			hRylr998->rylr998Transmitter.TxBuffer[1] = 0;
			break;
		case ULORA_CONN_COUNT:
			uloraCommunicationTest	(hRylr998->rylr998Transmitter.TxBuffer);
			break;
	}
	packetSize += AT_OVERHEAD_SIZE + sizeof(SEND) + 2 + 1 + 1;   //2 is the number of segment separators
																 //1 is for the receiver address
																 //1 is for the payload
	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat((char*) uartTxBuffer, SEND);
	strcat((char*) uartTxBuffer, SET_VALUE);

	strcat((char*) uartTxBuffer, (char*) hRylr998->rylr998Transmitter.address);
	strcat((char*) uartTxBuffer, SEGMENT_SEPARATOR);

	packetSize += hRylr998->rylr998Transmitter.TxBuffer[1] - 1 - '0' ;
	//-1 is used to omit null character from consideration
	packetSizeAscii[0] = hRylr998->rylr998Transmitter.TxBuffer[1];
//	rylr998Int2Ascii(packetSizeAscii);


	strcat((char*) uartTxBuffer, (char*)  packetSizeAscii);
	strcat((char*) uartTxBuffer, SEGMENT_SEPARATOR);
	strcat((char*) uartTxBuffer, (char*) hRylr998->rylr998Transmitter.TxBuffer);
	memcpy(uartTxBuffer + 16, TERMINATOR, 2);
//	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, packetSize, 10);

	return ret;
}

Rylr998_Status_t rylr998ReceivePacketParser(Rylr998Handler_t* hRylr998)
{
	Rylr998_Status_t 	ret 						= Rylr998_ERROR;
	Rylr998RxCommand_e  command						= Rylr998R_NOT_FOUND;
	uint8_t 			tempUartRxBuffer[250] 		= {0};
    //Real size should be used instead
	memcpy(tempUartRxBuffer, hRylr998->rylr998Receiver.rxBuffer, 30);

	if(!memcmp(tempUartRxBuffer, RX_PACKET_START, 1))
	{
		command = rylr998ResponseFind	(tempUartRxBuffer + RESPONSE_OFFSET);
		switch (command)
		{
			case Rylr998R_OK:

				break;
			case Rylr998R_ADDRESS:
				rylr998Ascii2Int(&hRylr998->rylr998Receiver.rxBuffer[ADDRESS_OFFSET]);
				if(hRylr998->rylr998Receiver.rxBuffer[ADDRESS_OFFSET] != RYLR998_ADDRESS)
				{
					hRylr998->rylr998Receiver.address[0] = RYLR998_ADDRESS;
					rylr998Int2Ascii(hRylr998->rylr998Receiver.address);
					rylr998SetAddress(hRylr998->rylr998Receiver.address);
				}
				break;
			case Rylr998R_RCV:

				uloraPacketStore(tempUartRxBuffer + 9);
				break;
			case Rylr998R_RDY:

				break;
			default:
				break;
		}
	}

	return ret;
}

Rylr998_Status_t rylr998GetSent(Rylr998Handler_t* hRylr998)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	uint8_t 			packetSize 			= 0;
	uint8_t 			uartTxBuffer[20] 	= {0};

	packetSize = AT_OVERHEAD_SIZE + sizeof(SEND) - 1;

	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat((char*) uartTxBuffer,  SEND);
	strcat((char*)uartTxBuffer, CHECK);
	strcat((char*)uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, packetSize, 10);

	return ret;
}



Rylr998_Status_t rylr998Get(Rylr998Handler_t* hRylr998, Rylr998Command_e command)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	uint8_t 			txPacketSize 		= 0;
	uint16_t 			rxPacketSize 		= 0;
	uint8_t 			uartTxBuffer[20] 	= {0};

	memcpy(uartTxBuffer, 		 AT, AT_PRIFEX_SIZE);

	switch (command)											//This part calculates the command length and
	{															//selects the wanted command string
		case Rylr998_BOUDRATE:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(IPR) - 1;
			strcat((char*) uartTxBuffer, IPR);

			break;
		case Rylr998_PARAMETER:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(PARAMETER) - 1;
			strcat((char*) uartTxBuffer, PARAMETER);
			rxPacketSize = 21;

			break;
		case Rylr998_BAND:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(BAND) - 1;
			strcat((char*) uartTxBuffer, BAND);

			break;
		case Rylr998_ADDRESS:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(ADDRESS) - 1;
			strcat((char*) uartTxBuffer, ADDRESS);
			rxPacketSize = 12;

			break;
		case Rylr998_NETWORKID:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(NETWORKID) - 1;
			strcat((char*) uartTxBuffer, NETWORKID);

			break;
		case Rylr998_CRFOP:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(CRFOP) - 1;
			strcat((char*) uartTxBuffer, CRFOP);

			break;
		case Rylr998_SEND:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(SEND) - 1;
			strcat((char*) uartTxBuffer, SEND);

			break;
		case Rylr998_UID:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(UID) - 1;
			strcat((char*) uartTxBuffer, UID);

			break;
		case Rylr998_VER:

			txPacketSize = AT_OVERHEAD_SIZE + sizeof(VER) - 1;
			strcat((char*) uartTxBuffer, VER);

			break;
	}


	strcat((char*)uartTxBuffer,  CHECK);
	strcat((char*)uartTxBuffer,  TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, txPacketSize, 10);

	memset(hRylr998->rylr998Receiver.rxBuffer, RESET, 20);

	ret = HAL_UART_Receive_IT(&huart1, hRylr998->rylr998Receiver.rxBuffer, rxPacketSize);

	return ret;
}

Rylr998RxCommand_e rylr998ResponseFind(uint8_t* rxBuffer)
{
	Rylr998RxCommand_e 	ret 					= Rylr998R_NOT_FOUND;
	if(!memcmp(rxBuffer, ADDRESS, 7))
	{
		return ret = Rylr998R_ADDRESS;
	}
	else if(!memcmp(rxBuffer, RCV, 3))
	{
		return ret = Rylr998R_RCV;
	}
	else if(!memcmp(rxBuffer, OK, 2))
	{
		return ret = Rylr998R_OK;
	}
	else if(!memcmp(rxBuffer, READY, 5))
	{
		return ret = Rylr998R_RDY;
	}
	return ret;
}

Rylr998_Status_t checksumValidate(uint8_t* rxBuffer)
{
	Rylr998_Status_t ret = DISABLE;
	uint8_t checksum = 0;
	uint8_t packetSize = rxBuffer[1] - 1 - '0';
	for(uint8_t i = 0; i < packetSize; ++i)
	{
		checksum += rxBuffer[i] - '0';
	}
	if(checksum == rxBuffer[3] - '0')
	{
		ret = ENABLE;
	}
	return ret;
}

void rylr998Int2Ascii(uint8_t* value)
{
	*value += 0x30;
}

void rylr998Ascii2Int(uint8_t* value)
{
	*value -= 0x30;
}

void rylr998_enable(void)
{
	HAL_GPIO_WritePin(RYLR998_RST_GPIO_Port, RYLR998_RST_Pin, GPIO_PIN_SET);
}





