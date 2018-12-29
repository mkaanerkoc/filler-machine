/*
 * numpad_lib.c
 *
 *  Created on: 23 Ara 2015
 *  Author: Mehmet Kaan ERKOC 
 */

#include <msp430.h>
#include "numpad_lib.h"

#define NUMPAD_DEBOUNCING_DELAY 25
#define DELAY_FOR_16MHZ 6000 // set for 16Mhz change it to 1000 for 1 Mhz
#define DELAY_FOR_1MHZ 1000

void delayMicroseconds1(unsigned int ms) 
{
	while (ms--) 
	{
		__delay_cycles( DELAY_FOR_16MHZ ); 
	}
}

/* @desc : initialize io pins which responsible for numpad
*
*/
void init_numpad() 
{
	P2SEL = 0x00;
	P2SEL2 = 0x00;
	P1SEL &= ~BIT6;
	P1SEL2 &= ~BIT6;
	P2DIR |= BIT3 + BIT4 + BIT5;
	P1DIR |= BIT6;  // rows set as output
	P2DIR &= ~(BIT0 + BIT1 + BIT2); // colons set as input
	P2REN |= BIT0 + BIT1 + BIT2; // internal resistors enabled
	P2OUT &= ~(BIT0 + BIT1 + BIT2); // pull down resistor enabled
}

/* @desc : reads the input pins and decide which button pressed by user
*   
*/
char scan_numpad() 
{
	// Main Scanning function
	P1OUT &= ~BIT6;
	P2OUT &= ~(BIT4 + BIT5);
	P2OUT |= BIT3;
	if (P2IN & BIT2) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '1';
	}
	else if (P2IN & BIT1) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '2';
	}
	else if (P2IN & BIT0) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '3';
	}
	P1OUT &= ~BIT6;
	P2OUT &= ~(BIT3 + BIT5);
	P2OUT |= BIT4;
	
	if (P2IN & BIT2)
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '4';
	} 
	else if (P2IN & BIT1) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '5';
	} 
	else if (P2IN & BIT0)
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '6';
	}
	
	P1OUT &= ~BIT6;
	P2OUT &= ~(BIT3 + BIT4);
	P2OUT |= BIT5;
	
	if (P2IN & BIT2) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '7';
	} 
	else if (P2IN & BIT1) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '8';
	} 
	else if (P2IN & BIT0) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '9';
	}

	P1OUT |= BIT6;
	P2OUT &= ~(BIT3 + BIT4 + BIT5);

	if (P2IN & BIT2) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '*';
	} 
	else if (P2IN & BIT1) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '0';
	} 
	else if (P2IN & BIT0) 
	{
		delayMicroseconds1( NUMPAD_DEBOUNCING_DELAY );
		return '#';
	}

	delayMicroseconds1(25);
	return 9;   // If no key pressed , it returns 9, ( not ascii, binary 0b1001 )
}

