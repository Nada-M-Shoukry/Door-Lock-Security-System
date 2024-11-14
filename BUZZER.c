/*
 * BUZZER.c
 *
 *  Created on: Oct 9, 2024
 *      Author: nadam
 */
/********************************************/

#include "BUZZER.h"
#include "GPIO.h"

/**************Functions Definition**********/

 void Buzzer_init(void)
 {

	 GPIO_setupPinDirection (BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);

	 GPIO_writePin (BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);  /* Turn OFF buzzer initially */
 }

 void Buzzer_on(void)
 {
	 GPIO_writePin (BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);  /* Turn ON buzzer */
 }

 void Buzzer_off(void)
 {
	 GPIO_writePin (BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);  /* Turn OFF buzzer */
 }

