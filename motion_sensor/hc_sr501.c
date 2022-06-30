
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  hc_sr501.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jun 27, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "hc_sr501.h"
/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
hc_sr501_t hPirSensor;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief
 *
 * @param hMotionSens
 */
void hc_sr501Init(hc_sr501_t* hMotionSens, uint32_t onTime)
{
	memset(hMotionSens, 0, sizeof(hc_sr501_t));

	hMotionSens->sensorTimeout = onTime;

	hMotionSens->sesorActivation = true;


}
/**
 * @brief
 *
 * @param hMotionSens
 * @return
 */
bool hc_sr501ReadStatus(hc_sr501_t* hMotionSens)
{
	return hMotionSens->sesorStatus;
}
/**
 * @brief
 *
 * @param hMotionSens
 * @param sensorOutputStatus
 */
void hc_sr501UpdateStatus(hc_sr501_t* hMotionSens, uint8_t sensorOutputStatus)
{
	hMotionSens->sesorStatus = sensorOutputStatus;
}
/**
 * @brief
 *
 * @param hMotionSens
 */
void hc_sr501ResetTimer(hc_sr501_t* hMotionSens)
{
	hMotionSens->sensorTimer = 0;
}
/**
 * @brief
 *
 * @param hMotionSens
 * @return
 */
uint32_t hc_sr501ReadTimer(hc_sr501_t* hMotionSens)
{
	return hMotionSens->sensorTimer;
}
/**
 * @brief
 *
 * @param hMotionSens
 * @param currentTickValue
 */
void hc_sr501UpdateTimer(hc_sr501_t* hMotionSens,uint32_t currentTickValue)
{
	hMotionSens->sensorTimer = currentTickValue;
}
/**
 * @brief
 *
 * @param hMotionSens
 * @param timeout
 */
bool hc_sr501CheckTimer(hc_sr501_t* hMotionSens, uint32_t currentTickValue)
{
	bool ret = false;
	ret = currentTickValue - hMotionSens->sensorTimer >= hMotionSens->sensorTimeout ? true : false;
	if(ret)
	{
		hc_sr501UpdateTimer(hMotionSens, currentTickValue);
	}
	return ret;
}
/**
 * @brief
 *
 * @param hMotionSens
 * @param outputStatus
 * @return
 */
bool hc_sr501Handle(hc_sr501_t* hMotionSens,bool outputStatus, uint32_t currentTickValue)
{	//Check if the the PIR sensor init is executed.
	if(hMotionSens->sesorActivation)
	{
		if(hc_sr501CheckTimer(hMotionSens, currentTickValue))
		{
			hc_sr501UpdateStatus(hMotionSens, outputStatus);
			if(outputStatus)
			{
				//If motion is detected, block detection for around 9 seconds
				hMotionSens->sensorTimeout = HC_SR501_DETECTION_TIME_HIGH + HC_SR501_BLOCK_TIME_LOW;
			}
			else
			{
				hMotionSens->sensorTimeout = 500;
			}
		}

	}
	return hc_sr501ReadStatus(hMotionSens);
}
/**************************  Useful Electronics  ****************END OF FILE***/
