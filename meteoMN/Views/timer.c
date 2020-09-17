#include <util/delay.h>
#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "../Logic/buzzer.h"
#include "timer.h"
#include "clock.h"

bool timerIsRunning;
bool timerIsReady;

int8_t timerExpectedHour = 0;
int8_t timerExpectedMinute = 0;
int8_t timerExpectedSecond = 0;

bool timer_is_ready(){ return timerIsReady;}
bool timer_is_running(){ return timerIsRunning;}

void timer_reset()
{
	PORTD &= ~BUZZER;
	
	reset();
	
	timerIsReady = 0;
	timerIsRunning = 0;
	
	timerExpectedHour = 0;
	timerExpectedMinute = 0;
	timerExpectedSecond = 0;
}

void timer_run()
{
	if(timerIsRunning == 0)
	{
		timerExpectedSecond = 0;
		timerExpectedHour = selectedHour;
		timerExpectedMinute = selectedMinute;
		timerIsRunning = 1;
	}
}

void timer_trigger()
{	
	if(timerIsReady == 0 && timerIsRunning == 1)
	{
		if(timerExpectedSecond == 0 && timerExpectedMinute == 0 && timerExpectedHour == 0)
		{
			timerIsReady = 1;
		}
		else
		{
			timerExpectedSecond--;
			
			if(timerExpectedSecond < 0 && timerExpectedMinute >= 0)
			{
				timerExpectedSecond = SECONDS_IN_MINUTE - 1;
				timerExpectedMinute--;
			}
			
			if(timerExpectedMinute < 0 && timerExpectedHour > 0)
			{
				timerExpectedMinute = MINUTES_IN_HOUR - 1;
				timerExpectedHour--;
			}
		}
	}
}

void display_timer_view()
{
	draw_header("TIMER",timer_is_running(),alarm_is_running());
	display_timer(1,1,1);
}

void display_timer_hour_set_view()
{
	draw_header("TIMER SET H",timer_is_running(),alarm_is_running());
	display_timer(1,0,0);
}

void display_timer_minute_set_view()
{
	draw_header("TIMER SET M",timer_is_running(),alarm_is_running());
	display_timer(0,1,0);
}

void display_timer(bool displayHour, bool displayMinute, bool displaySecond)
{
	if(timerIsRunning == 0)
	{
		draw_clock(0,2, selectedHour, selectedMinute,0, displayHour, displayMinute, displaySecond);
	}
	else if(timerIsRunning == 1 && timerIsReady == 0)
	{
		draw_clock(0,2, timerExpectedHour, timerExpectedMinute, timerExpectedSecond, displayHour, displayMinute, displaySecond);
	}
	else
	{
		draw_its_time();
		play_timer_sound();
	}
}
