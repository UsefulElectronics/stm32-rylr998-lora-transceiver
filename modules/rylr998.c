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

	ret = HAL_UART_Transmit(&hUart, uartTxBuffer, packetSize, 10);

	return ret;
}
//AT+ADDRESS=<Address>
Rylr998_Status_t rylr998SetAddress(uint16_t address)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	const uint8_t 		packetSize 			= 15;
	uint8_t 			uartTxBuffer[15] 	= {0};
	uint8_t 			rylr998Address[2]	= {0};

	rylr998Address[0] = (address & 0xFF00) >> 8;
	rylr998Address[1] = address & 0x00FF;

	memcpy(uartTxBuffer, AT, AT_PRIFEX_SIZE);
	strcat(uartTxBuffer, ADDRESS);
	strcat(uartTxBuffer, SET_VALUE);

	strcat((char*) uartTxBuffer, (char*) rylr998Address);
	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&hUart, uartTxBuffer, packetSize, 10);

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

	ret = HAL_UART_Transmit(&hUart, uartTxBuffer, packetSize, 10);



	ret = HAL_UART_Receive_IT(&hUart, hRylr998->rylr998Receiver.rxBuffer, 12);

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

	ret = HAL_UART_Transmit(&hUart, uartTxBuffer, hRylr998->rylr998Transmitter.payloadLength, 10);

	return ret;
}

Rylr998_Status_t rylr998ReceivePacketParser(Rylr998Handler_t* hRylr998)
{
	Rylr998_Status_t 	ret 					= Rylr998_ERROR;
	uint8_t 			uartRxBuffer[250] 		= {0};

	return ret;
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
