/*
 * DC_MOTOR.c
 *
 *  Created on: Oct 9, 2024
 *      Author: nadam
 */
/**************************************************************/

#include "DC_MOTOR.h"
#include "GPIO.h"
#include "TIMER0_PWM_DRIVER.h"

/********************Functions Definition**********************/

void DcMotor_Init(void)
{
	GPIO_setupPinDirection (MOTOR_PORT_ID, MOTOR_IN1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection (MOTOR_PORT_ID, MOTOR_IN2_PIN, PIN_OUTPUT);


	Timer0_PWM_init();
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	switch (state)
	{
	case (STOP) :
			GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN1_PIN, LOGIC_LOW);
	        GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN2_PIN, LOGIC_LOW);

	        Timer0_PWM_start(speed);
	        break;

	case (CW) :
			GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN1_PIN, LOGIC_LOW);
	        GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN2_PIN, LOGIC_HIGH);

	        Timer0_PWM_start(speed);
	        break;

	case (ACW) :
			GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN1_PIN, LOGIC_HIGH);
	        GPIO_writePin (MOTOR_PORT_ID, MOTOR_IN2_PIN, LOGIC_LOW);

	        Timer0_PWM_start(speed);
	        break;

	}
}

