/*
 * lcd_lib.h
 *
 *  Created on: 23 Ara 2015
 *      Author: 21128195 , Mehmet Kaan ERKOÇ
 *      
 */

#include <msp430.h>

void config_uart_clock() {
	DCOCTL = 0; // Dahili osilatör ayarlarýný en düþük yap
	BCSCTL1 = CALBC1_1MHZ; // Dahili osilatörü 1MHz'e ayarla
	DCOCTL = CALDCO_1MHZ; // Dahili osilatörü 1MHz'e ayarla
	P1SEL = BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2; // UART Ayarlarý, Saat kaynaðý SMCLK
	UCA0BR0 = 104; // 1MHz 9600 bud ayarı
	UCA0BR1 = 0; // 1MHz 9600 baud ayarı
	UCA0MCTL = UCBRS0; // UART Baud hassas ayar
	UCA0CTL1 &= ~UCSWRST; // USCI birimini hazýrla
	IE2 |= UCA0RXIE; // USCI_A0 RX kesmesini aç

}

void pause_uart(){
	IE2 =0x00;
}

void resume_uart(){
	IE2 |= UCA0RXIE;
}
