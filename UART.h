/*
 * UART.h
 *
 *  Created on: Oct 25, 2024
 *      Author: nadam
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/***********************************************************************/

typedef enum
{
	 one_stopBit, two_stopBit
}UART_StopBitType;

typedef enum
{
	BitDataType_5bit, BitDataType_6bit, BitDataType_7bit, BitDataType_8bit
}UART_BitDataType;

typedef enum
{
	parityType_OFF, parityType_ON, parityType_EVEN, parityType_ODD
}UART_ParityType;

typedef uint32 UART_BaudRateType;

typedef struct {
UART_BitDataType bit_data;
UART_ParityType parity;
UART_StopBitType stop_bit;
UART_BaudRateType baud_rate;

}UART_ConfigType;


/***********************Functions Protoypes*****************************/


void UART_init(const UART_ConfigType * config_Ptr);   /* Function Description :
                                                       * Functional responsible for Initialize the UART device by:
                                                       * 1. Setup the Frame format like number of data bits,
                                                            parity bit type and number of stop bits.
                                                       * 2. Enable the UART.
                                                       * 3. Setup the UART baud rate. */


void UART_sendByte(const uint8 data);                 /* Function Description :
                                                       * Functional responsible for send byte to another UART device. */

uint8 UART_receiveByte(void);                         /* Function Description :
                                                       * Functional responsible for receive byte from another UART device. */


void UART_sendString(const uint8 *Str);               /* Function Description :
                                                       * Send the required string through UART to the other UART device. */


void UART_receiveString(uint8 *Str);                  /* Function Description :
                                                       * Receive the required string until the '#' symbol
                                                         through UART from the other UART device. */

void UART_flush(void);

#endif /* UART_H_ */
