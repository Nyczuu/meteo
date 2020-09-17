#include <avr/io.h>

#define LED1_PORT PORTD
#define LED2_PORT PORTB
#define LED1 (1<<PD7)
#define LED2 (1<<PB0)
#define BUZZER (1<<PD5)

void play_alarm_sound();
void play_timer_sound();