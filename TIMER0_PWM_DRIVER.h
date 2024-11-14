/*
 * TIMER0_PWM_DRIVER.h
 *
 *  Created on: Oct 8, 2024
 *      Author: nadam
 */
/***************************************************************/

#ifndef TIMER0_PWM_DRIVER_H_
#define TIMER0_PWM_DRIVER_H_

/***************************************************************/

#include "std_types.h"

/*********************Definitions*******************************/

#define PWM_PIN_ID PIN3

/*****************Functions Prototypes**********************************/

 void Timer0_PWM_init  (void);

 void Timer0_PWM_start (uint8 duty_cycle);


#endif /* TIMER0_PWM_DRIVER_H_ */
