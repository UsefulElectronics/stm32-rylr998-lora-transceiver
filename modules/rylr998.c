/*
 * rylr998.c
 *
 *  Created on: May 4, 2022
 *      Author: Ward
 */
#include <rylr998.h>

UART_HandleTypeDef hUart;


Rylr998_Status_t rylr998_test(void)
{
	Rylr998_Status_t 	ret 				= Rylr998_ERROR;
	const uint8_t 		packetSize 			= 4;
	uint8_t 			uartTxBuffer[4] 	= {0};

	memcpy(uartTxBuffer, AT, packetSize);

	strcat((char*) uartTxBuffer, TERMINATOR);

	ret = HAL_UART_Transmit(&hUart, uartTxBuffer, packetSize, 10);

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
