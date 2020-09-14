#include "avr/interrupt.h"

#include "../Views/clock.h"
#include "../Views/timer.h"

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

#define BUTTON_PRESSED (BUTTON_1_PRESSED || BUTTON_2_PRESSED || BUTTON_3_PRESSED || BUTTON_4_PRESSED)

#define MENU_CLOCK 10
#define MENU_CLOCK_SET_HOUR 20
#define MENU_CLOCK_SET_MINUTE 21
#define MENU_TIMER 30
#define MENU_TIMER_SET_HOUR 40
#define MENU_TIMER_SET_MINUTE 41
#define MENU_TEMPEREATURE 50
#define MENU_DASHBOARD 60

void refresh_screen();
void on_button_pressed();