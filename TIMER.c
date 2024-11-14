/*
 * TIMER.c
 *
 *  Created on: Nov 4, 2024
 *      Author: nadam
 */
/***********************************************************/
#include "TIMER.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/********************* Global Variables *******************/

static  void (*g_Timer0_CallBackPtr)(void) = NULL_PTR;                  /* Global variables to hold the address of the call back function in the application */
static  void (*g_Timer1_CallBackPtr)(void) = NULL_PTR;
static  void (*g_Timer2_CallBackPtr)(void) = NULL_PTR;

/******************* ISRs *********************************/

ISR (TIMER0_OVF_vect)
{
	if ( *g_Timer0_CallBackPtr != NULL_PTR)
	{
		(*g_Timer0_CallBackPtr)();             /* Call Back Notification */
	}
}

ISR (TIMER0_COMP_vect)
{
	if ( *g_Timer0_CallBackPtr != NULL_PTR)
	{
		(*g_Timer0_CallBackPtr)();            /* Call Back Notification */
	}
}

ISR (TIMER1_OVF_vect)
{
	if ( *g_Timer1_CallBackPtr != NULL_PTR)
	{
		(*g_Timer1_CallBackPtr)();            /* Call Back Notification */
	}
}

ISR (TIMER1_COMPA_vect)
{
	if ( *g_Timer1_CallBackPtr != NULL_PTR)
	{
		(*g_Timer1_CallBackPtr)();            /* Call Back Notification */
	}
}

ISR (TIMER2_OVF_vect)
{
	if ( *g_Timer2_CallBackPtr != NULL_PTR)
	{
		(*g_Timer2_CallBackPtr)();            /* Call Back Notification */
	}
}

ISR (TIMER2_COMP_vect)
{
	if ( *g_Timer2_CallBackPtr != NULL_PTR)
	{
		(*g_Timer2_CallBackPtr)();            /* Call Back Notification */
	}
}


/***************** Functions Definitions ******************/

void Timer_init(const Timer_ConfigType * config_ptr){


	switch (config_ptr->timer_ID)
	{
	case  TIMER0_ID :

		TCCR0 |= ( config_ptr->timer_clock);

		TCNT0 = ( config_ptr-> timer_InitialValue);

		if ( (config_ptr->timer_mode != FAST_PWM) && (config_ptr->timer_mode != PWM_PHASE_CORRECT) )
		{
			TCCR0 |= (1 << FOC0);
		}

		if ( config_ptr-> timer_mode == NORMAL_MODE )
		{
			TIMSK |= (1 << TOIE0);
		}

		else  if ( config_ptr-> timer_mode == COMPARE_MODE)
		{
			TCCR0 |= (1 << WGM01);

			OCR0 = ( config_ptr-> timer_compare_MatchValue);

			TIMSK |= (1 << OCIE0);
		}

		else if ( config_ptr-> timer_mode == PWM_PHASE_CORRECT)
		{
			TCCR0 |= (1 << WGM00);
		}

		else if ( config_ptr-> timer_mode == FAST_PWM)
		{
			TCCR0 |= (1 << WGM00) | (1 << WGM01);
		}

		break;

	case TIMER1_ID :

		TCCR1B |= ( config_ptr-> timer_clock);

		TCNT1   = ( config_ptr->timer_InitialValue);


		if ( (config_ptr->timer_mode != FAST_PWM) && (config_ptr->timer_mode != PWM_PHASE_CORRECT) )
		{
			TCCR1A |= (1 << FOC1A) | (1 << FOC1B);

		}

		if ( config_ptr-> timer_mode == NORMAL_MODE )
		{
			TIMSK  |= (1 << TOIE1);
		}

		else  if ( config_ptr-> timer_mode == COMPARE_MODE)
		{
			SET_BIT(TCCR1B,WGM12);

			OCR1A   = config_ptr->timer_compare_MatchValue;

			TIMSK  |= (1 << OCIE1A);

			SET_BIT(TIFR,OCF1A);
		}

		else if ( config_ptr-> timer_mode == PWM_PHASE_CORRECT)
		{
			TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1<< WGM13);
		}

		else if ( config_ptr-> timer_mode == FAST_PWM)
		{
			TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 <<WGM12) | (1<< WGM13) ;
		}

		break;

	case TIMER2_ID :

		TCCR2 |= ( config_ptr-> timer_clock);
		TCNT2  = ( config_ptr->timer_InitialValue);

		if ( (config_ptr->timer_mode != FAST_PWM) && (config_ptr->timer_mode != PWM_PHASE_CORRECT) )
		{
			TCCR2 |= (1 << FOC2);

		}

		if ( config_ptr-> timer_mode == NORMAL_MODE )
		{
			TIMSK |= (1 << TOIE2);
		}

		else  if ( config_ptr-> timer_mode == COMPARE_MODE)
		{
			TCCR2 |= (1 << WGM21);

			OCR2   = config_ptr->timer_compare_MatchValue;

			TIMSK |= (1 << OCIE2);
		}

		else if ( config_ptr-> timer_mode == PWM_PHASE_CORRECT)
		{
			TCCR2 |= (1 << WGM20);
		}

		else if ( config_ptr-> timer_mode == FAST_PWM)
		{
			TCCR2 |= (1 << WGM20) | (1 << WGM21);
		}

		break;
	}
}
	void Timer_deInit(Timer_ID_Type timer_type)
	{
		switch (timer_type)
		{
		case TIMER0_ID :

			TCCR0  = 0;
			TCNT0  = 0;
			OCR0   = 0;
			TIMSK &= ~(1 << TOIE0 ) & ~(1 << OCIE0 );

			break;

		case TIMER1_ID :

			TCCR1A = 0;
			TCNT1  = 0;
			OCR1A  = 0;
			TIMSK &= ~(1 << TOIE1 ) & ~(1 << OCIE1A );
			TCCR1B&= ~(1 << CS12  ) & ~(1 << CS10 ) & ~(1 << CS11 );

			break;

		case TIMER2_ID :

			TCCR2  = 0;
			TCNT2  = 0;
			OCR2   = 0;
			TIMSK &= ~(1 << TOIE2 ) & ~(1 << OCIE2 );

			break;

		}
	}

	void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
	{
		switch (a_timer_ID)
		{
		case TIMER0_ID :

			g_Timer0_CallBackPtr = a_ptr;
			break;

		case TIMER1_ID :

			g_Timer1_CallBackPtr = a_ptr;
			break;

		case TIMER2_ID :

			g_Timer2_CallBackPtr = a_ptr;
			break;
		}
	}


