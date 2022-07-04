
/**
 ******************************************************************************
 * @Channel Link    :  https://www.youtube.com/user/wardzx1
 * @file    		:  siren.h
 * @author  		:  Ward Almasarani - Useful Electronics
 * @version 		:  v.1.0
 * @date    		:  Jul 3, 2022
 * @brief   		:
 *
 ******************************************************************************/

#ifndef SIREN_H_
#define SIREN_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

/* MACROS --------------------------------------------------------------------*/
#define SIREN_INITIAL_TONE			125
#define SIREN_LIMIT_TONE			250
#define SIREN_INITIAL_SOUND_LEVEL	500

#define SIREN_SILENT				0
/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/
typedef struct
{
	uint32_t 	timer;
	uint32_t	soundLevel;
	bool		activisionStatus;
	bool		toneDirection;
	uint32_t	tone;
	uint16_t	playSpeed;
	uint8_t		oneStep;

}sirenHandler_t;
/* VARIABLES -----------------------------------------------------------------*/
extern sirenHandler_t hSiren;
/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void sirenInitialize	(sirenHandler_t* sirenHandler,
						uint32_t newSoundLevel,
						uint32_t playSpeed,
						uint8_t  oneStep);
void sirenHandler		(sirenHandler_t* sirenHandler, uint32_t* sirenTone, uint32_t currentTickValue);
void sirenSoundLevelSet	(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel, uint32_t setValue);
void sirenStart			(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel, uint32_t currentTickValue);
void sirenStop			(sirenHandler_t* sirenHandler, uint32_t* newSoundLevel);

#endif /* SIREN_H_ */

/**************************  Useful Electronics  ****************END OF FILE***/
