/*
 * stepper_lib.c
 *
 *  Created on: 24 Ara 2015
 *  Author: Mehmet Kaan ERKOC
 */


#include <msp430.h>
#include "stepper_lib.h"

void init_stepper() 
{
	P1DIR |= BIT3;
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	TACTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode
	CCR0 = motorSpeed;
}

void pause_stepper() 
{
	CCTL0 = 0x00;
}

void resume_stepper( int countParam ) 
{
	if ( (_countNum <=0 ) && ( count != 0 ) )  
	{
		_countNum = count;
		CCTL0 = CCIE;
	}
}
