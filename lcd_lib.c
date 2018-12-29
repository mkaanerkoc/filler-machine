/*
 * lcd_lib.c
 *
 *      Created on: 23 Ara 2015
 *      Author: Mehmet Kaan ERKOC
 *      This library functions written by Mehmet Kaan ERKOÇ , 
 *      for driving 16x4 LCD screen with 3-to-8 shift register. avvv yea 
 */

#include <stdint.h>
#include "constants.h"
#include <msp430.h>
#include "lcd_lib.h"



//kaantodo delay micro second u cikar disari 
//kaantodo port ve pin numaralarini definition la gec

void delayMicroseconds(unsigned int ms) 
{
	while (ms--) 
	{
		__delay_cycles(1); // set for 16Mhz change it to 1000 for 1 Mhz
	}
}

void pinWrite(unsigned int bit, unsigned char val) 
{
	if( val ) 
	{
		P1OUT |= bit;
	} 
	else 
	{
		P1OUT &= ~bit;
	}
}

// Pulse the clock pin
void pulseClock(void) 
{
	P1OUT |= CLOCKPIN;
	P1OUT ^= CLOCKPIN;
}

void shiftOut(unsigned char val) 
{
	//Set latch to low (should be already)
	P1OUT &= ~LATCHPIN;

	char i;

	// Iterate over each bit, set data pin, and pulse the clock to send it
	// to the shift register
	for (i = 0; i < 8; i++) 
	{

		pinWrite(DATAPIN, (val & (1 << (7 - i))));
		pulseClock();
	}

	// Pulse the latch pin to write the values into the storage register
	P1OUT |= LATCHPIN;
	P1OUT &= ~LATCHPIN;
}

/************ low level data pushing commands **********/
void setEPin(uint8_t pinValue) 
{
	if (pinValue == HIGH) 
	{
		_register |= ENABLE_PIN;    // HIGH
	} 
	else 
	{
		_register &= ~ENABLE_PIN;   // LOW
	}
}

void setD4Pin(uint8_t pinValue) 
{
	if( pinValue == HIGH) 
	{
		_register |= PIN_D4;    // HIGH
	} 
	else 
	{
		_register &= ~PIN_D4;   // LOW
	}
}

void setD5Pin(uint8_t pinValue) 
{
	if( pinValue == HIGH) 
	{
		_register |= PIN_D5;    // HIGH
	} 
	else 
	{
		_register &= ~PIN_D5;   // LOW
	}
}

void setD6Pin(uint8_t pinValue) 
{
	if (pinValue == HIGH) 
	{
		_register |= PIN_D6;    // HIGH
	} 
	else 
	{
		_register &= ~PIN_D6;   // LOW
	}
}
void setD7Pin(uint8_t pinValue) 
{
	if (pinValue == HIGH) 
	{
		_register |= PIN_D7;    // HIGH
	} 
	else 
	{
		_register &= ~PIN_D7;   // LOW
	}
}

void setRSPin(uint8_t pinValue) 
{
	if (pinValue == HIGH) 
	{
		_register |= RS_PIN;    // HIGH
	} 
	else 
	{
		_register &= ~RS_PIN;   // LOW
	}
}

void pulseEnable(void) 
{
	// LOW / HIGH / LOW of ENABLE_PIN
	setEPin(LOW);	// LOW
	shiftOut(_register);

	delayMicroseconds(1);
	setEPin(HIGH);	// HIGH
	shiftOut(_register);

	__delay_cycles(60);	// enable pulse must be >450ns

	setEPin(LOW);	// LOW
	shiftOut(_register);
	__delay_cycles(600);	// commands need > 37us to settle
}


void write4bits(uint8_t value) 
{
	int val_nibble = value & 0x0F;  //clean the value.  (unnecessary)

	setD4Pin(val_nibble & 01);
	val_nibble >>= 1;
	setD5Pin(val_nibble & 01);
	val_nibble >>= 1;
	setD6Pin(val_nibble & 01);
	val_nibble >>= 1;
	setD7Pin(val_nibble & 01);

	pulseEnable();
}

// write either command or data, with automatic 4/8-bit selection
void send(uint8_t value, uint8_t mode) 
{
	setRSPin(mode);
	shiftOut(_register);

	//digitalWrite(_rs_pin, mode);

	// if there is a RW pin indicated, set it low to Write
	//if (_rw_pin != 255) {
	//  digitalWrite(_rw_pin, LOW);
	//}

	if (_displayfunction & LCD_8BITMODE) 
	{
		//write8bits(value);
	} else {
		write4bits(value >> 4);
		write4bits(value);
	}
}

void command(uint8_t value) 
{
	send(value, LOW);
}

void lcd_write(uint8_t value) 
{
	send(value, HIGH);
}

void lcd_clear() 
{
	command( LCD_CLEARDISPLAY );  // clear display, set cursor position to zero
	delayMicroseconds(500);  // this command takes a long time!
}

void lcd_home() 
{
	command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row) 
{
	int row_offsets[] = { 0x00, 0x40, 0x10, 0x50 };
	if (row > _numlines) 
	{
		row = _numlines - 1;    // we count rows starting w/0
	}

	command( LCD_SETDDRAMADDR | (col + row_offsets[row]) );
}

// Turn the display on/off (quickly)
void noDisplay() 
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_display() 
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize) 
{
	if (lines > 1) 
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
	_currline = 0;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands.
	delayMicroseconds(50000);
	// Now we pull both RS and R/W low to begin commands

	setRSPin(LOW);
	setEPin(LOW);
	shiftOut(_register);

	//digitalWrite(_rs_pin, LOW);
	//digitalWrite(_enable_pin, LOW);

	//if (_rw_pin != 255) {
	//  digitalWrite(_rw_pin, LOW);
	//}

	//put the LCD into 4 bit or 8 bit mode
	if (!(_displayfunction & LCD_8BITMODE)) 
	{
		// this is according to the hitachi HD44780 datasheet
		// figure 24, pg 46

		// we start in 8bit mode, try to set 4 bit mode
		write4bits(0x03);
		delayMicroseconds(4500); // wait min 4.1ms

		// second try
		write4bits(0x03);
		delayMicroseconds(4500); // wait min 4.1ms

		// third go!
		write4bits(0x03);
		delayMicroseconds(150);

		// finally, set to 4-bit interface
		write4bits(0x02);
	} 
	else 
	{
		// this is according to the hitachi HD44780 datasheet
		// page 45 figure 23

		// Send function set command sequence
		command(LCD_FUNCTIONSET | _displayfunction);
		delayMicroseconds(4500);  // wait more than 4.1ms

		// second try
		command(LCD_FUNCTIONSET | _displayfunction);
		delayMicroseconds(150);

		// third go
		command(LCD_FUNCTIONSET | _displayfunction);
	}

	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_display();

	// clear it off
	lcd_clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

}
void lcd_print(const char *c) 
{
	while (*c) 
	{
		lcd_write(*c++);
	}
}

void lcd_print2(char a[]) 
{
	int i=0;
	for( i = 0; i<5; i++ )
	{
		lcd_write( a[i] );
	}
}


