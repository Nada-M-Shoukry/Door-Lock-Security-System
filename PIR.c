/*
 * PIR.c
 *
 *  Created on: Nov 3, 2024
 *      Author: nadam
 */
/******************************************************/
#include "PIR.h"
#include "GPIO.h"
#include "common_macros.h"

/***************Functions Definitions ****************/


void PIR_init (void)
{
	GPIO_setupPinDirection (PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

uint8 PIR_getState (void)
{
	return GPIO_readPin (PIR_PORT_ID, PIR_PIN_ID);

}
