/*
 * HMI_ECU.c
 *
 *  Created on: Nov 4, 2024
 *      Author: nadam
 */
/*************************************************/

#include "HMI_ECU.h"

/*************************************************/

#define SENDING_FIRST_PASSWORD     0XC0
#define FIRST_PASSWORD             0xA0
#define SYSTEM_BEGIN               0xDD
#define READY_TO_CHANGE_PASS       0X10
#define READY_TO_OPEN_DOOR         0X20
#define OPEN_DOOR                  '+'
#define CHANGE_PASS                '-'
#define PASS_END                   '%'
#define PASS_CONFIRMATION          0XF0
#define PASS_CORRECT               0XB0
#define PASS_INCORRECT             0XE0
#define PEOPLE_DETECTED            1
#define PEOPLE_UNDETECTED          0
#define DOOR_UNLOCKING             0X01
#define DOOR_LOCKING               0X02
#define ERROR_CONFIRMATION         0XCC
#define NO_ERROR                   0XAA
#define DISPLAY_ERROR              0X0F
#define CONTINUE                   0XFF


/*********************************************/

uint8 counter = 0;
uint8 current_password[5];
uint8 required = 0;
uint8 first_password[5];
uint8 password[5];
uint8 re_password[5];
uint8 i = 0;
uint8 get_state = 0;
uint8 byte_received = 0;
uint8 security_error = 0;


void Timer1_callBack(void)
{
	counter++;
}


int main (void)
{
	Timer_ConfigType  TIMER_config = {0, 7812, TIMER1_ID, PRESCALER_1024, COMPARE_MODE};
	UART_ConfigType   UART_config  = {BitDataType_8bit, parityType_OFF, one_stopBit, 9600};


	Timer_init(&TIMER_config);
	Timer_setCallBack( Timer1_callBack, TIMER1_ID);
	UART_init(&UART_config);
	LCD_init();
	sei();



	while(UART_receiveByte() != (0xff));                                            // wait for the two ECUs to synchronize

	LCD_displayStringRowColumn ( 0,0, "SET PASSWORD");

	for ( i = 0; i < 5; i++)
	{
		_delay_ms(10);
		first_password[i] = KEYPAD_getPressedKey();
		LCD_moveCursor(1,i);
		LCD_displayCharacter('*');
		_delay_ms(150);
	}

	while(KEYPAD_getPressedKey() != PASS_END);                                       /* wait until the user presses enter '%' */

	UART_sendByte (SENDING_FIRST_PASSWORD);
	for ( i = 0; i < 5; i++)
	{
		UART_sendByte(first_password[i]);
		_delay_ms(10);
	}
	_delay_ms(100);

	while(1)
	{
		LCD_displayStringRowColumn( 0,0, " + : Open Door"   );
		LCD_displayStringRowColumn( 1,0, " - : Change Pass" );

		required = KEYPAD_getPressedKey();
		_delay_ms(150);

		UART_sendByte(SYSTEM_BEGIN);
		_delay_ms(20);

		switch ( required )
		{

		case CHANGE_PASS :

			LCD_clearScreen();
			LCD_moveCursor (0,1);                                                    /* Take Door Password from user */
			LCD_displayString("New Password");
			_delay_ms(200);

			for (i = 0 ; i < 5 ; i++)
			{
				_delay_ms(10);
				password[i] = KEYPAD_getPressedKey();
				LCD_moveCursor(1,i);
				LCD_displayCharacter('*');
				_delay_ms(100);

			}

			while(KEYPAD_getPressedKey() != PASS_END);


			UART_sendByte (READY_TO_CHANGE_PASS);

			for (i = 0 ; i < 5 ; i++)
			{
				UART_sendByte (password[i]);
				_delay_ms(10);
			}

			LCD_clearScreen();
			LCD_moveCursor (0,0);                                                     /* Password confirmation process */
			LCD_displayString("ReEnter Password");
			_delay_ms(200);

			for (i = 0 ; i < 5 ; i++)
			{
				_delay_ms(10);
				re_password[i] = KEYPAD_getPressedKey();
				LCD_moveCursor(1,i);
				LCD_displayCharacter('*');
				_delay_ms(100);
			}

			while(KEYPAD_getPressedKey() != PASS_END);

			for (i = 0 ; i < 5 ; i++)
			{
				UART_sendByte (re_password[i]);
				_delay_ms(10);
			}

			while ( UART_receiveByte () != PASS_CONFIRMATION);

			byte_received = UART_receiveByte ();

			switch (byte_received)
			{

			case PASS_CORRECT :

				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0, "Password");
				LCD_displayStringRowColumn(1,5, "confirmed");

				break;

			case PASS_INCORRECT :

				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0, "Password");
				LCD_displayStringRowColumn(1,5, "Incorrect");
				counter = 0;
				while(counter < 2);

				break;
			}

			while(UART_receiveByte() != ERROR_CONFIRMATION);
			security_error = UART_receiveByte();

			switch(security_error)
			{

			case DISPLAY_ERROR :

				LCD_clearScreen();
				LCD_displayStringRowColumn(0,4, "ERROR!!");
				while (UART_receiveByte () != CONTINUE);
				break;

			case NO_ERROR :
				break;

			}
			break;


			case OPEN_DOOR :

				LCD_clearScreen();
				LCD_displayStringRowColumn (0,0, "Enter Password");
				_delay_ms(200);


				for( i = 0; i < 5 ; i++)
				{
					_delay_ms(10);
					current_password[i] =  KEYPAD_getPressedKey();
					LCD_moveCursor (1,i);
					LCD_displayCharacter('*');
					_delay_ms(200);
				}

				while ( KEYPAD_getPressedKey() != PASS_END );


				UART_sendByte ( READY_TO_OPEN_DOOR);

				for( i = 0; i < 5 ; i++)
				{
					UART_sendByte ( current_password[i] );

				}

				while ( UART_receiveByte() != PASS_CONFIRMATION );

				byte_received = UART_receiveByte();

				switch (byte_received)
				{

				case ( PASS_CORRECT) :


			    while ( UART_receiveByte () != DOOR_UNLOCKING );

				LCD_clearScreen();
				LCD_displayStringRowColumn ( 0,0, "Door unlocking");
				counter = 0;
				while ( counter < 2 );

				LCD_clearScreen();
				LCD_displayStringRowColumn ( 0,0, "wait for people");
				LCD_displayStringRowColumn ( 1,4, "to enter");
				counter = 0;
				while ( counter < 6 );

				while( UART_receiveByte () != DOOR_LOCKING );

				LCD_clearScreen();
				LCD_displayStringRowColumn ( 0,0, "Door locking");
				counter = 0;
				while (counter < 6 );

				break;


				case PASS_INCORRECT :

					LCD_clearScreen();
					LCD_displayStringRowColumn(0,3, "INCORRECT");
					LCD_displayStringRowColumn(1,3, "PASSWORD");
					counter = 0;
					while(counter < 2);

					break;
				}

				while(UART_receiveByte() != ERROR_CONFIRMATION);
				security_error = UART_receiveByte();

				switch(security_error)
				{

				case DISPLAY_ERROR :

					LCD_clearScreen();
					LCD_displayStringRowColumn(0,4, "ERROR!!");
					while (UART_receiveByte () != CONTINUE);
					break;

				case NO_ERROR :
					break;

				}
				break;
		}
	}
}

