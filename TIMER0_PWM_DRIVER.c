/*
 * TIMER0_PWM_DRIVER.c
 *
 *  Created on: Oct 8, 2024
 *      Author: nadam
 */
/***************************************************/

#include "TIMER0_PWM_DRIVER.h"
#include "GPIO.h"
#include "common_macros.h"
#include <avr/io.h>

/***************Functions Definitions**************/

void Timer0_PWM_init(void)
{
	TCNT0 = 0;             /* Initiate timer count from 0. */



	CLEAR_BIT (TCCR0, 7);  /* Make sure FOC0 Bit is clear to initialize PWM mode. */


	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<CS00) | (1<<CS02) | (1<<COM01); 	/*Adjust TCCR0 bits to :
	                                                             * Fast PWM mode.
	                                                             * Non-inverting mode
	                                                             * prescalar 1024 */

	OCR0 = 0;              /*Start with 0% Duty cycle */

	GPIO_setupPinDirection (PORTB_ID, PWM_PIN_ID, PIN_OUTPUT); /* Setup OC0 o/p */
}

void Timer0_PWM_start (uint8 duty_cycle)
{
	if (duty_cycle > 100)
	{
		duty_cycle = 100;      /* Adjust duty cycle to a maximum value of 100 */
	}

	OCR0 = (uint8)( ((uint16) duty_cycle * 255) /100 ); /*Convert duty cycle percentage to OCR value. */

}


