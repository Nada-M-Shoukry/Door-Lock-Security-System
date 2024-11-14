/*
 * EXT_EEPROM.h
 *
 *  Created on: Nov 1, 2024
 *      Author: nadam
 */

#ifndef EXT_EEPROM_H_
#define EXT_EEPROM_H_

#include "std_types.h"

#define ERROR 0
#define SUCCESS 1

/******************************Functions Prototypes ******************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);



#endif /* EXT_EEPROM_H_ */
