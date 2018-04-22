/*
 * TWItest2.c
 *
 * Created: 04.07.2016 22:57:40
 *  Author: LV
 */

#include "USART_DEBUG.h"
#include <avr/io.h>
#include <stdbool.h>
#include "lcd1602.h"
#include "i2c.h"
#include "uart.h"
#include <avr/interrupt.h>

volatile uint8_t B[16];
volatile uint8_t ib = 0;
volatile uint8_t ilcd = 0;

volatile bool btn_interrupt = false;
//volatile bool uart_interrupt = false;

int main(void)
{
	cli();

    // interrupt on falling edge of INT0 (PD2)
    DDRD &= ~_BV(PORTD2);
	PORTD |= _BV(PORTD2);

    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);

//	_delay_ms(50);

//	_delay_ms(500);
	lcd1602_init_alt();
	init_UART(9600);

//	lcd1602_send_string("Hello, world!");
//	lcd1602_send_hex(0xAB);
//	lcd1602_goto_xy(0, 1);

	sei();

	while (true) {
		if(ib > ilcd){
			lcd1602_send_hex(B[ilcd++]);
			if (ilcd == 8) 	lcd1602_goto_xy(0, 1);
		}
	}
}
//---------------------------------------------------------------------------------

ISR(USART_RX_vect) {
	if (ib < 16) {
		B[ib++] = UDR0;
	}
}
//---------------------------------------------------------------------------------

// INT0 interrupt
ISR(INT0_vect) {
	cli();
//    EIMSK &= ~(1<<INT0);	//disable INT0

	lcd1602_clear();
	ilcd = 0;
	ib = 0;
	UCSR0B &= ~(1<<RXEN0);	//LV: flush UDR
	_delay_us(100);
	UCSR0B |= (1<<RXEN0);	//LV: flush UDR
//	B[0]=0;
	_delay_ms(50);
	sei();
//    EIMSK |= (1<<INT0);		//enable INT0
}
//---------------------------------------------------------------------------------
