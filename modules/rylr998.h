/*
 * rylr998.h
 *
 *  Created on: May 4, 2022
 *      Author: Ward
 */

#ifndef RYLR998_H_
#define RYLR998_H_

#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <string.h>


//AT Command Set
#define AT						"AT"
#define RESET					"+RESET"
#define TERMINATOR				"\r\n"


//RYLR998 pins
#define RYLR998_RST_Pin 		GPIO_PIN_14
#define RYLR998_RST_GPIO_Port 	GPIOB

//Global variables
extern UART_HandleTypeDef hUart;

typedef enum
{
	Rylr998_OK = 0x00U,
	Rylr998_ERROR,
	Rylr998_BUSY,
	Rylr998_TIMEOUT
} Rylr998_Status_t;

typedef union																	// module send to Lop
{	uint8_t data;
	struct
	{
		uint8_t B0				:1;
		uint8_t B1				:1;
		uint8_t B2				:1;
		uint8_t B3				:1;
		uint8_t B4				:1;
		uint8_t B5				:1;
		uint8_t B6				:1;
		uint8_t B7				:1;
	}bit;

}Flag8_t;

typedef struct
{
	bool 				rylr998Reset;
	bool 				rylr998RxBuffer;
	bool				rylr998TxBuffer;
	uint8_t				rylr998Timer;
	Flag8_t				rylr998Flag;

	uint16_t			indicatorAddress;
	int16_t				indicatorCounter;
	uint8_t 			indicatorFlasher;
}Rylr998Handler_t;



void rylr998_disable				(void);
void rylr998_enable					(void);
Rylr998_Status_t rylr998_test		(void);


#endif /* RYLR998_H_ */
