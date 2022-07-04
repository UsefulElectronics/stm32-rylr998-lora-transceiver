
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  siren.c
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jul 3, 2022
 * @brief   		:
 *
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "siren.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
sirenHandler_t hSiren;
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/**
 * @brief 	Change the sound tone to sound like a siren depending sirenHandler
 *
 * @param 	sirenHandler		:	siren controlling parameters.
 *
 * @param 	sirenTone			:	Timer pre-scaler should be passed to this function
 *
 * @param 	currentTickValue	:	systick is passed
 */
void sirenHandler(sirenHandler_t* sirenHandler, uint32_t* sirenTone, uint32_t currentTickValue)
{
	if(sirenHandler->activisionStatus == true)
	{
		if(currentTickValue - sirenHandler->timer >= sirenHandler->playSpeed)
		{
			sirenHandler->timer = currentTickValue;
			if(sirenHandler->toneDirection)
			{
				*sirenTone += sirenHandler->oneStep;
				if(*sirenTone >= SIREN_LIMIT_TONE)
				{
					sirenHandler->toneDirection = false;
				}
			}
			else
			{
				*sirenTone -= sirenHandler->oneStep;
				if(*sirenTone <= SIREN_INITIAL_TONE)
				{
					sirenHandler->toneDirection = true;
				}
			}

		}
	}
}
/**
 * @brief
 *
 * @param sirenHandler
 * @param newSoundLevel
 * @param setValue
 */
void sirenSoundLevelSet(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel, uint32_t setValue)
{
	*newSoundLevel 			 	= setValue;
	sirenHandler->soundLevel 	= setValue;
}
/**
 * @brief
 *
 * @param sirenHandler
 * @param newSoundLevel
 * @param playSpeed
 * @param tone
 * @param oneStep
 */
void sirenInitialize(sirenHandler_t* sirenHandler,
					uint32_t newSoundLevel,
					uint32_t playSpeed,
					uint8_t  oneStep)
{
	sirenHandler->soundLevel 	= newSoundLevel;
	sirenHandler->tone 			= SIREN_INITIAL_TONE;
	sirenHandler->playSpeed		= playSpeed;
	sirenHandler->oneStep		= oneStep;
}

/**
 * @brief
 *
 * @param sirenHandler
 * @param newSoundLevel
 * @param currentTickValue
 */
void sirenStart(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel, uint32_t currentTickValue)
{
	sirenHandler->activisionStatus 	= true;
	sirenHandler->timer 			= currentTickValue;
	sirenHandler->toneDirection		= true;
	*newSoundLevel 					= sirenHandler->soundLevel;

}
/**
 * @brief
 *
 * @param sirenHandler
 * @param newSoundLevel
 */
void sirenStop(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel)
{
	sirenHandler->activisionStatus 	= false;
	sirenHandler->tone 				= SIREN_INITIAL_TONE;
	*newSoundLevel					= 0;

}
/**************************  Useful Electronics  ****************END OF FILE***/
