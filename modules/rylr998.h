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
#include <stdbool.h>

#include <main.h>
#include "ulora.h"
//AT Command Set
#define TEST					"AT"
#define AT						"AT+"

#define RESETART				"RESET"		//Software RESET
#define MODE					"MODE"		//Set the wireless work mode.
#define IPR						"IPR"		//Set the UART baud rate
#define PARAMETER				"PARAMETER"//Set the RF parameters
#define BAND					"BAND"		//Set RF Frequency
#define ADDRESS					"ADDRESS"	//Set the ADDRESS ID of module LoRa.
#define NETWORKID				"NETWORKID"	//Set the network ID.
#define CRFOP					"CRFOP"		//Set the RF output power.
#define SEND					"SEND"		//Send data to the appointed address by Command Mode.
#define RCV						"RCV"		//Show the received data actively
#define UID						"UID"		//To inquire module ID. 12BYTES
#define VER						"VER"
#define OK						"OK"
#define READY					"READY"

#define TERMINATOR				"\r\n"
#define CHECK					"?"
#define SET_VALUE				"="
#define RX_PACKET_START			"+"
#define SEGMENT_SEPARATOR		","

#define RYLR998_ADDRESS			0x02

#define AT_PRIFEX_SIZE			0x03
#define AT_TERMINATOR_SIZE		0x02
#define AT_SET_VALUE_SIZE		0x01
#define AT_OVERHEAD_SIZE		(AT_PRIFEX_SIZE + AT_TERMINATOR_SIZE + AT_SET_VALUE_SIZE)
#define AT_ADDRESS_SIZE			0x02

#define RESPONSE_OFFSET			0x01		//offset by 1 to ignore + character after checking it
#define ADDRESS_OFFSET			0x09		//offset to where the data is located in the address get command response





//RYLR998 pins
#define RYLR998_RST_Pin 		GPIO_PIN_14
#define RYLR998_RST_GPIO_Port 	GPIOB


#define RYLR998_ReadInterruptFlag()				hLoRaModule.rylr998Flag.bit.B0
#define RYLR998_WriteInterruptFlag(state)		hLoRaModule.rylr998Flag.bit.B0 = state

#define RYLR998_ReadSuccessfulRxFlag()			hLoRaModule.rylr998Flag.bit.B1
#define RYLR998_WriteSuccessfulRxFlag(state)	hLoRaModule.rylr998Flag.bit.B1 = state

#define RYLR998_ReadSuccessfulTxFlag()			hLoRaModule.rylr998Flag.bit.B2
#define RYLR998_WriteSuccessfulTxFlag(state)	hLoRaModule.rylr998Flag.bit.B2 = state


//<Programmed Preamble>4~24, (default 12)
typedef uint8_t Preamble4_24_t;
//<NetworkID>=3~15,18(default18)
typedef uint8_t NetworkId3_15or18_t;
//<RF Output Power>=0~22, (default22)
typedef uint8_t Crfop0_22_t;
typedef enum
{
	Rylr998_TRANSCEIVER = 0x00U,
	Rylr998_SLEEP,
	Rylr998_SMART_POWER_SAVING,
} Rylr998Mode_e;
//SF7to SF9 at 125kHz, SF7 to SF10 at 250kHz, and SF7 to SF11 at 500kHz
//<Spreading Factor>7~11 (default 9)
typedef enum
{
	SF7 = 0x07U,
	SF8,
	SF9,
	SF10,
	SF11
} Rylr998Sf_e;
//<Bandwidth>7~9, default is 125KHz
typedef enum
{
	B125KHz = 0x07U,
	B250KHz,
	B500KHz
} Rylr998Bandwidth_e;
//<Coding Rate>1~4, (default 1)
typedef enum
{
	CR1 = 0x01U,
	CR2,
	CR3,
	CR4
} Rylr998CodingRate_e;

typedef enum
{
	Rylr998R_OK = 0x00U,
	Rylr998R_ADDRESS,
	Rylr998R_RCV,
	Rylr998R_RDY,
	Rylr998R_NOT_FOUND

} Rylr998RxCommand_e;

typedef enum
{
	Rylr998_OK = 0x00U,
	Rylr998_ERROR,
	Rylr998_BUSY,
	Rylr998_TIMEOUT
} Rylr998_Status_t;

typedef union
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
	Rylr998Sf_e 		spredingFactor;
	Rylr998Bandwidth_e 	bandwidth;
	Rylr998CodingRate_e	CodingRate;
	Preamble4_24_t		programmedPreamble;

}Rylr998Parameter_t;

/*
 * AT+SEND=<Address>,<Payload Length>,<Data>
	<Address>0~65535, When the <Address> is 0,
	it will send data to all address (From 0 to
	65535.)
	<Payload Length> Maximum 240bytes
 */

typedef struct
{
	uint32_t			timer;
	uint8_t 			address[2];
	uint16_t			payloadLength;
	uint8_t				TxBuffer[240];

}Rylr998Transmitter_t;
/*
 * +RCV=<Address>,<Length>,<Data>,<RSSI>,<SNR>,
	<Address> Transmitter Address ID
	<Length> Data Length
	<Data> ASCll Format Data
	<RSSI> Received Signal Strength Indicator
	<SNR> Signal-to-noise ratio
 */

typedef struct
{
	uint8_t 			address[2];
	uint8_t				payloadLength;
	uint8_t				rxBuffer[240];
	int8_t				rssi;
	uint8_t				snr;

}Rylr998Receiver_t;

typedef enum
{
	Rylr998_BOUDRATE = 0x00U,
	Rylr998_PARAMETER,
	Rylr998_BAND,
	Rylr998_ADDRESS,
	Rylr998_NETWORKID,
	Rylr998_CRFOP,			//Output Power
	Rylr998_SEND, 			//Last sent data
	Rylr998_UID,
	Rylr998_VER,

}Rylr998Command_e;


/*
 * 	AT+FACTORY
	Manufacturer defaults:
	BAND：915MHz
	UART：115200
	Spreading Factor：9
	Bandwidth：125kHz
	Coding Rate：1
	Preamble Length：12
	Address：0
	Network ID：18
	CRFOP：22
 */
typedef struct
{
	bool 					rylr998Reset;
	uint32_t				rylr998Timer;
	Flag8_t					rylr998Flag;

	uint32_t				rylr998Band;
	uint16_t				rylr998BoaudRate;
	Rylr998Mode_e			rylr998Mode;
	Rylr998Transmitter_t	rylr998Transmitter;
	Rylr998Receiver_t		rylr998Receiver;
	NetworkId3_15or18_t 	rylr998NetworkId;
	Rylr998Parameter_t  	Rylr998Parameters;
	Crfop0_22_t				Rylr998RfPower;
	Rylr998RxCommand_e		Rylr998LastRXPacket;

}Rylr998Handler_t;

//Global variables
extern Rylr998Handler_t   hLoRaModule;


void 			 	rylr998_disable				(void);
void 			 	rylr998_enable				(void);
void 				rylr998ReceiverTask			(void);
void 				rylr998ModuleInitialize		(Rylr998Handler_t* hRylr998);
void 				rylr998Int2Ascii			(uint8_t* value);
void 				rylr998Ascii2Int			(uint8_t* value);
Rylr998_Status_t 	rylr998Test					(void);

void 				rylr998TargetAddressSet		(Rylr998Handler_t* hRylr998, uint8_t targetAddress);
Rylr998_Status_t 	rylr998SetNetworkId			(NetworkId3_15or18_t networkId);
Rylr998_Status_t 	rylr998SetOutputPower		(Crfop0_22_t outpuPower);
Rylr998_Status_t 	rylr998GetAddress			(Rylr998Handler_t* hRylr998);
Rylr998_Status_t 	rylr998SetAddress			(uint8_t* address);
Rylr998_Status_t 	rylr998Get					(Rylr998Handler_t* hRylr998, Rylr998Command_e command);
Rylr998_Status_t 	rylr998Send					(Rylr998Handler_t* hRylr998, UloraCommand_e uLoRaCommand);
Rylr998_Status_t 	rylr998GetSent				(Rylr998Handler_t* hRylr998);
Rylr998_Status_t 	rylr998GetParameter			(Rylr998Handler_t* hRylr998);
Rylr998_Status_t 	rylr998ReceivePacketParser	(Rylr998Handler_t* hRylr998);

Rylr998RxCommand_e 	rylr998ResponseFind			(uint8_t* rxBuffer);
Rylr998_Status_t 	rylr998ParameterLoad		(Rylr998Handler_t* hRylr998);


#endif /* RYLR998_H_ */
