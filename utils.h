/*
 * utils.h
 *
 *  Created on: 26 Ara 2015
 *  Author: Mehmet Kaan ERKOC
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "constants.h"
#include "lcd_lib.h"
#include "uart_lib.h"
#include "numpad_lib.h"
#include "stepper_lib.h"


void 	floatToStr( float arg0, char arg1[] );
void 	getWeightStr( char a[], char b[] );
float 	stringToFloat( char a[], int tarti );
int 	attenuate( float errval );
void 	append( char a[], const char b);
void 	delaySec( unsigned int ms);
void 	getWeightInfo( char a[], char b[] );
void 	clearStr( char a[] );
void 	resetStr( char a[] );


#endif /* UTILS_H_ */
