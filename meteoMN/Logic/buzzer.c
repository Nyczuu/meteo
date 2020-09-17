#include <util/delay.h>
#include "buzzer.h"

void play_alarm_sound()
{
	PORTD ^= BUZZER;
	_delay_ms(200);
}

void play_timer_sound()
{
	PORTD ^= (BUZZER|LED1);
	_delay_ms(100);
}