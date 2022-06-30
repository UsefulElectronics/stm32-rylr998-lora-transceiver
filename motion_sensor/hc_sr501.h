
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  hc_sr501.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jun 27, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef HC_SR501_H_
#define HC_SR501_H_


/* INCLUDES ----------------------------------------- ------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* MACROS --------------------------------------------------------------------*/
#define HC_SR501_DETECTION_TIME_HIGH	2800		//ms
#define HC_SR501_BLOCK_TIME_LOW			6300		//ms
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef struct
{
	uint32_t 	sensorTimer;
	uint32_t 	sensorTimeout;
	bool		sesorStatus;
	bool		sesorActivation;
}hc_sr501_t;
/* VARIABLES -----------------------------------------------------------------*/
extern hc_sr501_t hPirSensor;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void 		hc_sr501Init			(hc_sr501_t* hMotionSens, uint32_t timeout);
bool 		hc_sr501ReadStatus		(hc_sr501_t* hMotionSens);
void 		hc_sr501UpdateStatus	(hc_sr501_t* hMotionSens, uint8_t sensorOutputStatus);
void 		hc_sr501ResetTimer		(hc_sr501_t* hMotionSens);
uint32_t	hc_sr501ReadTimer		(hc_sr501_t* hMotionSens);
void 		hc_sr501UpdateTimer		(hc_sr501_t* hMotionSens,uint32_t currentTickValue);
bool 		hc_sr501CheckTimer		(hc_sr501_t* hMotionSens,uint32_t currentTickValue);
bool 		hc_sr501Handle			(hc_sr501_t* hMotionSens, bool outputStatus, uint32_t currentTickValue);



#endif /* HC_SR501_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
