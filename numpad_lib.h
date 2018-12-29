/*
 * numpad_lib.h
 *
 *  Created on: 23 Ara 2015
 *  Author: Mehmet Kaan ERKOC
 */

#ifndef NUMPAD_LIB_H_
#define NUMPAD_LIB_H_

/*
	@info: numpad requires 7 pin to drive. 4 for rows and 3 for columns. numpads are driven as matrix grid.
	@desc: pin abstraction
*/

#define ROW1 BIT3 // P2.3
#define ROW2 BIT4 // P2.4
#define ROW3 BIT5 // P2.5
#define ROW4 BIT6 // P1.6
#define COL3 BIT0 // P2.0
#define COL2 BIT1 // P2.1
#define COL1 BIT2 // P2.2

char _pressedVal;
void delayMicroseconds1(unsigned int ms);
char scan_numpad();
void init_numpad();

#endif /* NUMPAD_LIB_H_ */
