/*
 * CONTROL_ECU.c
 *
 *  Created on: Nov 4, 2024
 *      Author: nadam
 */
/*******************************************************/

#include "CONTROL_ECU.h"

/*******************************************************/

#define SENDING_FIRST_PASSWORD     0XC0
#define FIRST_PASSWORD             0xA0
#define SYSTEM_BEGIN               0xDD
#define READY_TO_CHANGE_PASS       0X10
#define READY_TO_OPEN_DOOR         0X20
#define PASS_CONFIRMATION          0XF0
#define PASS_CORRECT               0XB0
#define PASS_INCORRECT             0XE0
#define EEPROM_START_ADDRESS       0x0005
#define PEOPLE_DETECTED            1
#define PEOPLE_UNDETECTED          0
#define DOOR_UNLOCKING             0X01
#define DOOR_LOCKING               0X02
#define ERROR_CONFIRMATION         0XCC
#define NO_ERROR                   0XAA
#define DISPLAY_ERROR              0X0F
#define CONTINUE                   0XFF

/*******************************************************/

uint8 user_choice = 0;
uint8 consecutives = 0;
uint8 counter = 0;
uint8 first_password[5];
uint8 changed_password[5];
uint8 current_password[5];
uint8 password[5];
uint8 re_password[5];
uint8 read_password[5];
uint8 i = 0;
uint8 password_matched = 1;
uint8 pir_state = PEOPLE_UNDETECTED;
uint8 get_state = 0;


void Timer1_callBack(void)
{
	counter++;
}

int main ()
{
	UART_ConfigType   UART_config  = {BitDataType_8bit, parityType_OFF, one_stopBit, 9600};
	Timer_ConfigType  TIMER_config = {0, 7812, TIMER1_ID, PRESCALER_1024, COMPARE_MODE};
	I2C_ConfigType    I2C_config   = {0x01, 2, prescaler_1};


	Timer_init(&TIMER_config);
	Timer_setCallBack(Timer1_callBack, TIMER1_ID);


	UART_init(&UART_config);

	I2C_init(&I2C_config);

	sei();
	PIR_init();
	Buzzer_init();
	DcMotor_Init();
	Timer0_PWM_init();

	UART_sendByte(0xff);


	get_state = UART_receiveByte ();
	if ( get_state == SENDING_FIRST_PASSWORD)
	{
		for(i = 0; i < 5; i++ )
		{
			first_password[i] = UART_receiveByte();
			_delay_ms(10);

		}
		for(i = 0; i < 5; i++ )
		{
			EEPROM_writeByte (EEPROM_START_ADDRESS + i , first_password[i] );
			_delay_ms(10);

		}
	}


	while(1)
	{
		while ( UART_receiveByte() != SYSTEM_BEGIN );

		_delay_ms(20);
		user_choice = UART_receiveByte ();

		switch (user_choice)
		{

		case READY_TO_CHANGE_PASS :

			for(i = 0; i < 5; i++)
			{
				password[i] = UART_receiveByte();
				_delay_ms(10);
			}

			_delay_ms(500);

			for(i = 0; i < 5; i++)
			{
				re_password[i] = UART_receiveByte();
				_delay_ms(10);
			}

			UART_sendByte (PASS_CONFIRMATION);
			_delay_ms(10);
			password_matched = 1;                                               /* Assume password matched */
			for(i = 0; i < 5; i++)
			{
				if (password[i] != re_password[i])
				{
					password_matched = 0;
				}
			}

			if (password_matched)
			{
				UART_sendByte (PASS_CORRECT);
				for(i = 0; i < 5 ; i++ )
				{
					EEPROM_writeByte (EEPROM_START_ADDRESS + i, password[i]);
					_delay_ms(10);
				}

			}

			else
			{
				UART_sendByte (PASS_INCORRECT);
				consecutives++;

			}

			UART_sendByte(ERROR_CONFIRMATION);

			_delay_ms(10);
			if ( consecutives >= 3)
			{
				UART_sendByte(DISPLAY_ERROR);
				Buzzer_on();
				counter = 0;
				while (counter < 6);
				Buzzer_off();
				UART_sendByte (CONTINUE);

				consecutives = 0;         // Reset consecutive attempts counter after error handling
			}

			else
			{
				UART_sendByte (NO_ERROR);
			}

			break;

		case  READY_TO_OPEN_DOOR :


			for( i = 0; i < 5; i++)
			{
				current_password[i] = UART_receiveByte();

			}

			password_matched = 1;                                                       // Reset before comparison
			for( i = 0; i < 5; i++)
			{
				EEPROM_readByte ( EEPROM_START_ADDRESS + i, &read_password[i] );
				_delay_ms(10);
				if ( current_password[i] != read_password[i] )
				{
					password_matched = 0;
					break;
				}
			}
			UART_sendByte(PASS_CONFIRMATION);
			_delay_ms(10);
			if (password_matched)
			{
				UART_sendByte(PASS_CORRECT);

				_delay_ms(30);

				while (pir_state == PEOPLE_UNDETECTED)
				{
					pir_state = PIR_getState();           // Update the PIR state continuously
				}

				if (PIR_getState() == PEOPLE_DETECTED)
				{
					UART_sendByte (DOOR_UNLOCKING);

					DcMotor_Rotate(CW,100);               /* Activate motor to open the Door */
					counter = 0; 					      /* rotate motor for 15 seconds */
					while( counter < 6);

					DcMotor_Rotate ( STOP,0);             /* Hold the door open for people to enter */
					counter = 0;
					while( counter < 2);


					UART_sendByte (DOOR_LOCKING);
					DcMotor_Rotate (ACW, 100);
					counter = 0;
					while (counter < 6);
					DcMotor_Rotate ( STOP,0);
				}
			}

			else if (!password_matched)
			{
				UART_sendByte (PASS_INCORRECT);

				consecutives++;
			}

			UART_sendByte(ERROR_CONFIRMATION);
			_delay_ms(10);
			if ( consecutives >= 3)
			{
                UART_sendByte(DISPLAY_ERROR);
				Buzzer_on();
				counter = 0;
				while (counter < 6);
				Buzzer_off();
				UART_sendByte (CONTINUE);

				consecutives = 0;         // Reset consecutive attempts counter after error handling
			}

			else
			{
				UART_sendByte (NO_ERROR);
			}
		}
	}
}

