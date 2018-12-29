/*
 * lcd_lib.c
 *
 *      Created on: 23 Ara 2015
 *      Author: Mehmet Kaan ERKOC
 *      This project created by Mehmet Kaan ERKOC and Ahmet Atil Çetin
 *      ELE417 Embedded System Design Course Term Project
 */

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

char buffering = 0;
char _startScr = 0;
char _configScr = 0;
char _processScr = 0;
char _finishScr = 0;
char _startLatch = 0;
char _configLatch = 0;
char _processLatch = 0;
char _finishLatch = 0;
char mbuffer[32] = "-";
char tbuffer[32] = "-";
char wbuffer[5] = "-";
char ebuffer[32] = "-";
char cbuffer[32] = "-";
float setValue = 0;
float curValue = 0;
float errValue = 0;
int attValue = 0;

void resetAllBuffer() 
{
	clearStr(wbuffer);
	clearStr(ebuffer);
	clearStr(cbuffer);
	clearStr(tbuffer);
	clearStr(mbuffer);

	resetStr(tbuffer);
	resetStr(wbuffer);
}

void startScreen() 
{
	clearStr( mbuffer );

	if( _startLatch ) 
	{
		__disable_interrupt();
		_startLatch = 0;
		setCursor(0, 0);
		lcd_print("Welcome!");
		setCursor(0, 1);
		lcd_print("Press # to");
		setCursor(0, 2);
		lcd_print("start!");
	}

	const char r = scan_numpad();

	if( '#' == r ) 
	{
		lcd_clear();
		_startScr = 0;
		_configScr = 1;
		_configLatch = 1;
	}
}

/* @info: device has three basic menu screen.
*	      1st screen is configuration screen where user inputs the desired weight
*		  2nd screen is process screen where user get notified about the filling process
*		  3rd screen is finish screen show up when the filling process finished
*/
void configScreen() 
{
	if( _configLatch ) 
	{
		motorSpeed=200;
		_configLatch = 0;
		clearStr(mbuffer);
		setCursor(0, 0);
		lcd_print("Set the desired");
		setCursor(0, 1);
		lcd_print("weight");
		setCursor(0, 2);
		lcd_print("then press #");
		setCursor(0, 3);
	}
	const char r = scan_numpad();

	if( r != 9 ) 
	{
		if( '*' == r) 
		{
			clearStr(mbuffer);
			lcd_clear();
			setCursor(0, 0);
			lcd_print("Set the desired");
			setCursor(0, 1);
			lcd_print("weight");
			setCursor(0, 2);
			lcd_print("then press #");
			setCursor(0, 3);
		} 
		else if( '#' == r ) 
		{
			_configScr = 0;
			_processScr = 1;
			_processLatch = 1;
			lcd_clear();
		} 
		else 
		{
			append( mbuffer, r );
			lcd_write( r );
		}
	}
}

void processScreen() 
{
	if( _processLatch ) 
	{
		lcd_clear();
		init_stepper(); //de2
		_countNum = 200;
		setValue = stringToFloat(mbuffer, 0);
		__enable_interrupt();
		setCursor(0, 0);
		_processLatch = 0;
	}
}

void finishScreen() 
{
	if (_finishLatch) 
	{
		_disable_interrupt();
		curValue = -200;
		resetAllBuffer();
		lcd_clear();
		setCursor(0, 0);
		lcd_print("Turning back to");
		setCursor(0, 1);
		lcd_print("config screen");
		delaySec(500);
		_finishLatch = 0;
		_finishScr = 0;
		_configScr = 1;
		_configLatch = 1;
	}
}

// INTERRUPT SERVICE ROUTINES //

#pragma vector=TIMER0_A0_VECTOR

/* @info: that interrupt fires when Timer which responsible for feeding the step motor expired.
*		  if required step count is reached then it will stops the motor
*/
__interrupt void Timer_A( void ) 
{
	_countNum--;  // decrease the step count of the step motor 
	P1OUT ^= BIT3;      // Toggle P1.0
	if( _countNum <= 0 ) 
	{
		pause_stepper();
	}
}

#pragma vector=USCIAB0RX_VECTOR
/* @info: 	that interrupt fires when serial port reads a byte.
*			the weighning machine send its data in a frame that starts with 0x02 and end with 0x03
*			so this isr builds a package using a finite state machine techniques like automata
*			when its correctly receive a whole package, then it parse the meaningful data
*/
__interrupt void USCI0RX_ISR(void) 
{
	unsigned char rchar; 
	rchar = UCA0RXBUF;
	if( rchar == 2 ) 
	{ 
		buffering = 1; 
		clearStr(tbuffer);
	} 
	else if ( rchar == 3 ) 
	{
		__disable_interrupt();
		buffering = 0;
		//lcd_clear();
		// all the magic goes here
		//kaanbak buralar cirkin oldu biraz ayirsak mi ilerde function call felan bsiler 
		//kaanbak bu kadar yuklemeyelim ISR functionunu !!
		setCursor(0, 0);
		lcd_print("Set Value:");
		lcd_print( mbuffer );
		setCursor( 0, 1 );

		// parse the weight info from the received whole package
		getWeightInfo( tbuffer, wbuffer );
		getWeightStr( wbuffer, cbuffer );
		lcd_print( "Cur Value:" );
		lcd_print( cbuffer);
		curValue = stringToFloat( wbuffer, 1);

		// calculate the error value by using current weight value and user input
		errValue = setValue - curValue;
		floatToStr(errValue, ebuffer);
		setCursor(0, 2);
		lcd_print("Err Value:");
		lcd_print(ebuffer);
		if( errValue < 10 )
		{
			motorSpeed = 400;
		}
		if( errValue > 0.3f ) 
		{
			// calculates the next step count
			attValue = attenuate( errValue);
			// drives the step motor
			resume_stepper( attValue );
		} 
		else 
		{
			_processScr = 0;
			_finishScr = 1;
			_finishLatch = 1;
		}
	}

	if ( buffering == 1 && rchar != 2) 
	{
		append(tbuffer, rchar);
	}
}


int main( void ) 
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	config_uart_clock();
	P1DIR |= DATAPIN + CLOCKPIN + LATCHPIN;	// Set SHIFT Register pins 
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	lcd_begin(16, 4, 0);
	lcd_clear();
	_startScr = 1;
	_startLatch = 1;
	init_numpad();

	for (;;) 
	{
		if (_startScr) 
		{
			startScreen();
		} 
		else if (_configScr) 
		{
			configScreen();
		} 
		else if (_processScr) 
		{
			processScreen();
		} 
		else if (_finishScr) 
		{
			finishScreen();
		}
	}

	return 0;
}
