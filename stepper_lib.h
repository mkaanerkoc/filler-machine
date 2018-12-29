/*
 * stepper_lib.h
 *
 *  Created on: 24 Ara 2015
 *  Author: Mehmet Kaan ERKOC 
 *  This library written for ELE417 course term project
 */

#ifndef STEPPER_LIB_H_
#define STEPPER_LIB_H_

volatile int _countNum;
volatile char _done;
volatile int motorSpeed;

/*  Function Prototypes */
void init_stepper();
void pause_stepper();
void resume_stepper(int count);

#endif /* STEPPER_LIB_H_ */
