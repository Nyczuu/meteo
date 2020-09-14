#include <util/twi.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "avr/interrupt.h"
#include "External/I2C.h"
#include "stdint.h"
#include "Logic/menu.h"

#define LED1_PORT PORTD
#define LED2_PORT PORTB
#define LED1 (1<<PD7)
#define LED2 (1<<PB0)
#define BUZZER (1<<PD5)

void port_init()
{
	DDRC &= ~(BUTTON_2 | BUTTON_1);
	DDRB &= ~(BUTTON_4 | BUTTON_3);
	
	TCCR1B = (1<<CS12) | (1<<WGM12);
	OCR1AH = 0x7A;
	OCR1AL = 0x12;
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);
}

int main(void)
{
	sei();
	port_init();
	display_init();

	while (1)
	{
		if(timer_is_running() == 1 && timer_is_ready() == 1)
		DDRD |= LED1;
		
		if(BUTTON_PRESSED)
			on_button_pressed();
		
		refresh_screen();
	}
}

ISR (TIMER1_COMPA_vect)
{
	trigger();
}