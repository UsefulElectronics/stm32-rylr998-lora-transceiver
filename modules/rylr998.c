/*
 * rylr998.c
 *
 *  Created on: May 4, 2022
 *      Author: Ward
 */
#include <rylr998.h>

UART_HandleTypeDef hUart;

Rylr998Handler_t   hLoRaModule;


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
	uint8_t 			uartTxBuffer[14] 	= {0};
	uint8_t 			rylr998Address[2]	= {0};

	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat(uartTxBuffer, ADDRESS);
	strcat(uartTxBuffer, SET_VALUE);

	strcat((char*) uartTxBuffer,  address);
	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, packetSize, 10);

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
Rylr998_Status_t rylr998Send(Rylr998Handler_t* hRylr998, uint16_t address)
//Rylr998_Status_t rylr998Send(uint16_t address, uint8_t payloadLength, uint8_t* txBuffer)
{
	Rylr998_Status_t 	ret 					= Rylr998_ERROR;
	uint8_t 			uartTxBuffer[15] 		= {0};
	if(address <= 255)
	{
		hRylr998->rylr998Transmitter.address[0] 	= (uint8_t) address;
		hRylr998->rylr998Transmitter.payloadLength += AT_OVERHEAD_SIZE + AT_ADDRESS_SIZE - 1;
	}
	else
	{
		hRylr998->rylr998Transmitter.address[0] 	= (address & 0xFF00) >> 8;
		hRylr998->rylr998Transmitter.address[1] 	= address & 0x00FF;
		hRylr998->rylr998Transmitter.payloadLength += AT_OVERHEAD_SIZE + AT_ADDRESS_SIZE;
	}


	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat((char*) uartTxBuffer, SEND);
	strcat((char*) uartTxBuffer, SET_VALUE);

	strcat((char*) uartTxBuffer, (char*) hRylr998->rylr998Transmitter.address);
	strcat((char*) uartTxBuffer, (char) &hRylr998->rylr998Transmitter.payloadLength);
	strcat((char*) uartTxBuffer, (char*) hRylr998->rylr998Transmitter.TxBuffer);

	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&huart1, uartTxBuffer, hRylr998->rylr998Transmitter.payloadLength, 10);

	return ret;
}

Rylr998_Status_t rylr998ReceivePacketParser(Rylr998Handler_t* hRylr998)
{
	Rylr998_Status_t 	ret 					= Rylr998_ERROR;
	Rylr998RxCommand_e  command;
	uint8_t 			uartRxBuffer[250] 		= {0};

//	if(hRylr998->rylr998Receiver.rxBuffer[0] == RX_PACKET_START)
	if(!memcmp(hRylr998->rylr998Receiver.rxBuffer, RX_PACKET_START, 1))
	{
		command = rylr998ResponseFind	(hRylr998->rylr998Receiver.rxBuffer + RESPONSE_OFFSET);
		switch (command)
		{
			case Rylr998R_OK:

				break;
			case Rylr998R_ADDRESS:
				rylr998Ascii2Int(&hRylr998->rylr998Receiver.rxBuffer[ADDRESS_OFFSET]);
				if(hRylr998->rylr998Receiver.rxBuffer[ADDRESS_OFFSET] == 0x00)
				{
					hRylr998->rylr998Receiver.address[0] = RYLR998_ADDRESS;
					rylr998Int2Ascii(hRylr998->rylr998Receiver.address);
					rylr998SetAddress(hRylr998->rylr998Receiver.address);
				}
				break;
			case Rylr998R_RCV:

				break;
			default:
				break;
		}
	}

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

void rylr998_disable(void)
{
	HAL_GPIO_WritePin(RYLR998_RST_GPIO_Port, RYLR998_RST_Pin, GPIO_PIN_RESET);
}

Rylr998_Status_t rylr998_transmit()
{

}
