/*
 * lcd_lib.h
 *
 *  Created on: 23 Ara 2015
 *  Author: Mehmet Kaan ERKOÇ
 *      
 */

#ifndef LCD_LIB_H_
#define LCD_LIB_H_

#include <stdint.h>

#define DATAPIN BIT0 // DS -> 1.0      // 14 on 74HC595
#define CLOCKPIN BIT4 // SH_CP -> 1.4  // 11 on 74HC595
#define LATCHPIN BIT5 // ST_CP -> 1.5  // 12 on 74HC595
#define ENABLE_PIN  BIT0
#define RS_PIN		BIT7
#define DATABITS	BIT1+BIT2+BIT3+BIT4
#define PIN_D4		BIT4
#define PIN_D5		BIT3
#define PIN_D6		BIT2
#define PIN_D7		BIT1

uint8_t _datapin;
uint8_t _latchpin;
uint8_t _clockpin;
char _register; //Stores the current state of the data

uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;

uint8_t _initialized;

uint8_t _numlines, _currline;

void delayMicroseconds(unsigned int ms);
void pinWrite(unsigned int bit, unsigned char val);
void pulseClock(void);
void shiftOut(unsigned char val);
void setEPin(uint8_t pinValue);
void setD4Pin(uint8_t pinValue);
void setD5Pin(uint8_t pinValue);
void setD6Pin(uint8_t pinValue);
void setD7Pin(uint8_t pinValue);
void setRSPin(uint8_t pinValue);
void pulseEnable(void);
void write4bits(uint8_t value);
void send(uint8_t value, uint8_t mode);
void command(uint8_t value);
void lcd_write(uint8_t value);
void lcd_clear();
void lcd_home();
void setCursor(uint8_t col, uint8_t row);
void noDisplay();
void lcd_display();
void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
void lcd_print(const char *c);
void lcd_print2(char a[]);

#endif /* LCD_LIB_H_ */
