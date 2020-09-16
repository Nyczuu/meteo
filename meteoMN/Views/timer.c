#include <util/delay.h>
#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "timer.h"
#include "clock.h"

bool timerIsRunning = 0;
bool timerIsReady = 0;

int timerSelectedHour = 0;
int timerSelectedMinute = 0;

int timerExpectedHour = 0;
int timerExpectedMinute = 0;
int timerExpectedSecond = 0;

bool timer_is_ready()
{
	return timerIsReady;
}

bool timer_is_running()
{
	return timerIsRunning;
}

void timer_reset()
{
	timerIsReady = 0;
	timerIsRunning = 0;
	
	timerSelectedHour = 0;
	timerSelectedMinute = 0;

	timerExpectedHour = 0;
	timerExpectedMinute = 0;
	timerExpectedSecond = 0;
}

void timer_run()
{
	if(timerIsRunning == 0)
	{
		timerExpectedSecond = 0;
		timerExpectedHour = timerSelectedHour;
		timerExpectedMinute = timerSelectedMinute;
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

void timer_add_hour()
{
	timerSelectedHour = add_hour(timerSelectedHour);
}

void timer_add_minute()
{
	timerSelectedMinute = add_minute(timerSelectedMinute);
}

void timer_subtract_hour()
{
	timerSelectedHour = subtract_hour(timerSelectedHour);
}

void timer_subtract_minute()
{
	timerSelectedMinute = subtract_minute(timerSelectedMinute);
}

void display_timer_view()
{
	draw_string(0, 0, "TIMER");
	display_timer(1,1,1);
}

void display_timer_hour_set_view()
{
	draw_string(0, 0, "TIMER SET H");
	display_timer(1,0,0);
}

void display_timer_minute_set_view()
{
	draw_string(0, 0, "TIMER SET M");
	display_timer(0,1,0);
}

void display_timer(bool displayHour, bool displayMinute, bool displaySecond)
{
	if(timerIsRunning == 0)
	{
		draw_clock(0,2, timerSelectedHour, timerSelectedMinute,0, displayHour, displayMinute, displaySecond);
	}
	else if(timerIsRunning == 1 && timerIsReady == 0)
	{
		draw_clock(0,2, timerExpectedHour, timerExpectedMinute, timerExpectedSecond, displayHour, displayMinute, displaySecond);
	}
	else
	{
		draw_its_time();
	}
}
