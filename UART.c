/*
 * UART.c
 *
 *  Created on: Oct 25, 2024
 *      Author: nadam
 */

#include <avr/io.h>
#include "UART.h"
#include "common_macros.h"




	void UART_init(const UART_ConfigType * config_ptr)
	{
		uint16 ubrr_value = 0;

		UCSRA = (1<<U2X);                              /* U2X = 1 for double transmission speed */


		UCSRB = (1<<RXEN) | (1<<TXEN);    /************************** UCSRB Description **************************
		                                                * RXCIE = 0 Disable USART RX Complete Interrupt Enable
		                                                * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
		                                                * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
		                                                * RXEN  = 1 Receiver Enable
		                                                * RXEN  = 1 Transmitter Enable
		                                                * UCSZ2 = 0 For 8-bit data mode
		                                                * RXB8 & TXB8 not used for 8-bit data mode
		                                                ***********************************************************************/



		UCSRC = (1<<URSEL) | (config_ptr->parity << UPM0) | (config_ptr->stop_bit << USBS) | (config_ptr->bit_data << UCSZ0);


		                                               /************************** UCSRC Description **************************
		                                                * URSEL   = 1 The URSEL must be one when writing the UCSRC
		                                                * UMSEL   = 0 Asynchronous Operation
		                                                * UPM1:0  = parity bits selected
		                                                * USBS    = stop bits selected
		                                                * UCSZ1:0 = bit data mode selected
		                                                * UCPOL   = 0 Used with the Synchronous operation only
		                                                ***********************************************************************/



		ubrr_value = (uint16)(((F_CPU / (config_ptr->baud_rate * 8UL))) - 1);    /* Calculate the UBRR register value */

		UBRRH = ubrr_value>>8;                                       /* First 8 bits from the BAUD_PRESCALE inside UBRRL */
		UBRRL = ubrr_value;                                          /* and last 4 bits in UBRRH */

	}

	void UART_sendByte(const uint8 data)
	{

		while(BIT_IS_CLEAR (UCSRA , UDRE) ) {}         /* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
		                                                * transmitting a new byte so wait until this flag is set to one. */


		UDR = data;                                    /*Put the required data in the UDR register and it also clear
		                                                * the UDRE flag as the UDR register is not empty now. */
	}

	uint8 UART_receiveByte(void)
	{

		while(BIT_IS_CLEAR(UCSRA,RXC)){}               /* RXC flag is set when the UART receive data so wait until
		                                                  this flag is set to one */


	    return UDR;		                               /* Read the received data from the Rx buffer (UDR)
		                                                * The RXC flag will be cleared automatically after read the data */

	}

	void UART_sendString(const uint8 *Str)
	{

		while(*Str != '\0')                            /* Send the whole string */
		{
			UART_sendByte(*Str);
			Str++;
		}

	}

	void UART_receiveString(uint8 *Str)
	{
		uint8 i = 0;


		Str[i] = UART_receiveByte();                  /* Receive the first byte */

		while(Str[i] != '#')                          /* Receive the whole string until the '#' */
		{
			i++;
			Str[i] = UART_receiveByte();
		}

		Str[i] = '\0';                                /* After receiving the whole string plus the '#',
		                                                 replace the '#' with '\0' */
	}

	void UART_flush(void)
	{
	    while (UCSRA & (1<<RXC)) {
	        volatile uint8 dummy = UDR;  // Read and discard
	    }
	}

