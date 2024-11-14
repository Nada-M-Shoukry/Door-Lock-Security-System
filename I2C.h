/*
 * I2C.h
 *
 *  Created on: Nov 1, 2024
 *      Author: nadam
 */
/********************************************************************/
#ifndef I2C_H_
#define I2C_H_

#include "std_types.h"
/************************Data Types*********************************/

typedef uint8 I2C_AddressType;

typedef uint32 I2C_BaudRateType;

typedef enum
{
	prescaler_1, prescaler_4, prescaler_16, prescaler_64
}I2C_PrescalerValue;

typedef struct {
I2C_AddressType address;
I2C_BaudRateType bit_rate;
I2C_PrescalerValue prescaler;
}I2C_ConfigType;

/************************Definitions********************************/

/* I2C Status Bits in the TWSR Register */

#define I2C_START         0x08 /* start has been sent */
#define I2C_REP_START     0x10 /* repeated start */
#define I2C_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define I2C_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define I2C_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define I2C_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define I2C_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************Functions Prototype*****************************/

void I2C_init(const I2C_ConfigType * config_ptr);

void I2C_start(void);

void I2C_stop(void);

void I2C_writeByte(uint8 data);

uint8 I2C_readByteWithACK(void);

uint8 I2C_readByteWithNACK(void);

uint8 I2C_getStatus(void);


#endif /* I2C_H_ */
