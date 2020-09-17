#include "avr/interrupt.h"
#include "External/I2C.h"
#include "stdint.h"
#include "Logic/menu.h"
#include "Logic/buzzer.h"

void port_init()
{
	DDRC &= ~(BUTTON_2 | BUTTON_1);
	DDRB &= ~(BUTTON_4 | BUTTON_3);
	DDRB |= LED2;
	DDRD |= BUZZER | LED1;
	
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
	DHT_Setup();

	while (1)
	{
		if(BUTTON_PRESSED)
			on_button_pressed();
		
		refresh_screen();
	}
}

ISR (TIMER1_COMPA_vect)
{
	clock_trigger();
	timer_trigger();
	alarm_trigger();
}
