/*
 * PIR.h
 *
 *  Created on: Nov 3, 2024
 *      Author: nadam
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*********************** Definitons ********************/

#define PIR_PORT_ID       PORTC_ID
#define PIR_PIN_ID        PIN2_ID

/*******************Functions Prototype****************/

void PIR_init(void);

uint8 PIR_getState (void);



#endif /* PIR_H_ */
