
#include <util/delay.h>
#include <util/twi.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "avr/interrupt.h"
#include "I2C.h"
#include "stdint.h"
#include "time.h"
#include "display_extensions.h"

#define LED1_PORT PORTD
#define LED2_PORT PORTB
#define LED1 (1<<PD7)
#define LED2 (1<<PB0)

#define BUTTON_1_PORT PORTC
#define BUTTON_2_PORT PORTC
#define BUTTON_3_PORT PORTB
#define BUTTON_4_PORT PORTB

#define BUTTON_1 (1<<PC3)
#define BUTTON_2 (1<<PC2)
#define BUTTON_3 (1<<PB2)
#define BUTTON_4 (1<<PB1)

#define BUTTON_1_PRESSED !(PINC & BUTTON_1)
#define BUTTON_2_PRESSED !(PINC & BUTTON_2)
#define BUTTON_3_PRESSED !(PINB & BUTTON_3)
#define BUTTON_4_PRESSED !(PINB & BUTTON_4)

#define MENU_CLOCK 1
#define MENU_CLOCK_SET_HOUR 20
#define MENU_CLOCK_SET_MINUTE 21
#define MENU_TIMER 30
#define MENU_TIMER_SET_HOUR 40
#define MENU_TIMER_SET_MINUTE 41
#define MENU_TEMPEREATURE 50
#define MENU_DASHBOARD 60

volatile uint8_t hour = 12;
volatile uint8_t minute = 0;
volatile uint8_t second = 0;

volatile uint8_t selected_hour = 12;
volatile uint8_t selected_minute = 0;

bool displayHour = true;
bool displayMinute = true;
bool minuteChanged = false;
bool hourChanged = false;

volatile uint8_t selected_menu = MENU_CLOCK;

void display_dashboard()
{
}

void display_temperature()
{
}

void display_timer_set() 
{	
	draw_string(0, 0, "SET TIMER", 9);
	draw_clock(0,2, selected_hour, selected_minute, displayHour,displayMinute);
}

void display_timer() 
{
	draw_string(0, 0, "TIMER", 5);
	draw_clock(0,2, 0, 0, displayHour,displayMinute);
}

void display_clock_set() 
{
	draw_string(0, 0, "SET CLOCK", 9);
	draw_clock(0,2, selected_hour, selected_minute, displayHour,displayMinute);
}

void display_clock() 
{
	draw_clock(0,0, hour, minute, displayHour, displayMinute);
	draw_number(60,4,second);
}

void run()
{	
	if(selected_menu == MENU_DASHBOARD) display_dashboard();
	else if(selected_menu == MENU_TEMPEREATURE) display_temperature();
	else if(selected_menu == MENU_TIMER) display_timer();
	else if(selected_menu == MENU_TIMER_SET_HOUR || selected_menu == MENU_TIMER_SET_MINUTE) display_timer_set();
	else if(selected_menu == MENU_CLOCK) display_clock();
	else if(selected_menu == MENU_CLOCK_SET_HOUR || selected_menu == MENU_CLOCK_SET_MINUTE) display_clock_set();
}

void switch_menu(int number)
{
	ssd1306_clear_display();
	selected_menu = number;
	_delay_ms(250);
}

void port_init()
{
	//DDRD |= LED1;
	//DDRB |= LED2;
	
	DDRC &= ~(BUTTON_2 | BUTTON_1);
	DDRB &= ~(BUTTON_4 | BUTTON_3);
	
	TCCR1B = (1<<CS12) | (1<<WGM12);
	OCR1AH = 0x7A;
	OCR1AL = 0x12;
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);
}

void refreshBools()
{
	displayMinute = (selected_menu != MENU_CLOCK_SET_HOUR && selected_menu != MENU_TIMER_SET_HOUR);
	displayHour = (selected_menu != MENU_CLOCK_SET_MINUTE && selected_menu != MENU_TIMER_SET_MINUTE);
}

int main(void)
{
	sei();
	port_init();
	display_init();

	while (1)
	{
		
		if(selected_menu == MENU_CLOCK || selected_menu == MENU_TIMER || selected_menu == MENU_TEMPEREATURE || selected_menu == MENU_DASHBOARD)
		{
			if(BUTTON_1_PRESSED && selected_menu == MENU_CLOCK) switch_menu(MENU_CLOCK_SET_HOUR);
			else if(BUTTON_1_PRESSED) switch_menu(MENU_CLOCK);
			else if(BUTTON_2_PRESSED && selected_menu == MENU_TIMER) switch_menu(MENU_TIMER_SET_HOUR);
			else if(BUTTON_2_PRESSED) switch_menu(MENU_TIMER);
			else if(BUTTON_3_PRESSED) switch_menu(MENU_TEMPEREATURE);
			else if(BUTTON_4_PRESSED) switch_menu(MENU_DASHBOARD);
		}
		else if(selected_menu == MENU_CLOCK_SET_HOUR || selected_menu == MENU_TIMER_SET_HOUR)
		{				
			if(BUTTON_1_PRESSED)
			{
				selected_hour = add_hour(selected_hour);
				_delay_ms(200);
			}
			else if(BUTTON_2_PRESSED)
			{
				selected_hour = subtract_hour(selected_hour);	
				_delay_ms(200);
			}
			else if(BUTTON_3_PRESSED)
			{
				switch_menu(MENU_CLOCK_SET_MINUTE);
				_delay_ms(200);
			}
			else if(BUTTON_4_PRESSED) 
			{
				switch_menu(MENU_CLOCK);
				_delay_ms(200);
			};
		}
		else if(selected_menu == MENU_CLOCK_SET_MINUTE) 
		{
			if(BUTTON_1_PRESSED)
			{
				selected_minute = add_minute(selected_minute);
				_delay_ms(200);
			}
			else if(BUTTON_2_PRESSED)
			{
				selected_minute = subtract_minute(selected_minute);
				_delay_ms(200);
			}
			else if(BUTTON_3_PRESSED)
			{
				hour = selected_hour;
				minute = selected_minute;
				second, selected_hour, selected_minute = 0;
				switch_menu(MENU_CLOCK);
				_delay_ms(200);
			}
			else if(BUTTON_4_PRESSED)
			{
				switch_menu(MENU_CLOCK_SET_HOUR);
				_delay_ms(200);
			};
		}
		refreshBools();	
		run();
	}
}

ISR (TIMER1_COMPA_vect)
{
	int previous = second;
	second = add_second(second);
	
	if(previous == 59 && second == 0)
	{
		previous = minute;
		minute = add_minute(minute);
	}
	
	if(previous == 59 && minute == 0)
		hour = add_hour(hour);
}