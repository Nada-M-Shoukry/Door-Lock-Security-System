/*
 * DC_MOTOR.h
 *
 *  Created on: Oct 9, 2024
 *      Author: nadam
 */
/***********************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/******************Definitions*****************************/

#define MOTOR_PORT_ID             PORTD_ID
#define MOTOR_IN1_PIN             PIN6_ID
#define MOTOR_IN2_PIN             PIN7_ID

typedef enum
{
	STOP, CW, ACW

}DcMotor_State;

/***************Functions prototypes**********************/

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
