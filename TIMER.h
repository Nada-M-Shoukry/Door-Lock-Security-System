/*
 * TIMER.h
 *
 *  Created on: Nov 4, 2024
 *      Author: nadam
 */
/****************************************************************/
#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

/********************* Data types *******************************/

typedef enum
{
  TIMER0_ID, TIMER1_ID, TIMER2_ID
}Timer_ID_Type;

typedef enum
{
	STOP_TIMER, NO_PRESCALING, PRESCALER_8, PRESCALER_64, PRESCALER_256, PRESCALER_1024,
	EXTERNAL_CLK_FALLING_EDGE, EXTERNAL_CLK_RISING_EDGE
}Timer_ClockType;

typedef enum
{
	NORMAL_MODE, PWM_PHASE_CORRECT, COMPARE_MODE, FAST_PWM
}Timer_ModeType;

typedef uint16 timer_InitialValue;

typedef uint16 timer_compare_MatchValue;


typedef struct
{
	uint16          timer_InitialValue;
	uint16          timer_compare_MatchValue;                /*it will be used in compare mode only*/
	Timer_ID_Type   timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType  timer_mode;

}Timer_ConfigType;

/******************** Functions Prototypes **********************/

 void Timer_init(const Timer_ConfigType * config_ptr);

 void Timer_deInit(Timer_ID_Type timer_type);

 void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );


#endif /* TIMER_H_ */
