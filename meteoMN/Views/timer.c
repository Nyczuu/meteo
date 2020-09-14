#include "../Logic/display_extensions.h"
#include "timer.h"
#include "clock.h"

int timerSelectedHour = 0;
int timerSelectedMinute = 0;

int timerExpectedHour = 0;
int timerExpectedMinute = 0;
int timerExpectedSecond = 0;

bool isRunning = 0;
bool isReady = 0;

bool timer_is_ready()
{
	if(get_current_hour() == timerExpectedHour
	&& get_current_minute() == timerExpectedMinute
	&& get_current_second() == timerExpectedSecond)
	{
		isReady = 1;
	}

	return isReady;
}

bool timer_is_running()
{
	return isRunning;
}

void timer_reset()
{
	isReady = 0;
	isRunning = 0;
	timerSelectedHour = 0;
	timerSelectedMinute = 0;
	timerExpectedHour = 0;
	timerExpectedMinute = 0;
	timerExpectedSecond = 0;
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

void timer_run()
{
	if(isRunning == 0)
	{
		timerExpectedSecond = get_current_second();
		timerExpectedHour = get_current_hour() + timerSelectedHour;
		timerExpectedMinute = get_current_minute() + timerSelectedMinute;
		isRunning = 1;
	}
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
	if(isRunning == 0)
	{
		draw_clock(0,2, timerSelectedHour, timerSelectedMinute,0, displayHour, displayMinute, displaySecond);
	}
	else if(isRunning == 1 && isReady == 0)
	{
		draw_clock(0,2, timerExpectedHour - get_current_hour(), timerExpectedMinute - get_current_minute(),
		timer_get_second(), displayHour, displayMinute,displaySecond);
	}
	else
	{
		draw_new_string(2,12,"IT IS TIME!!!");
	}
}

int timer_get_second()
{
	int currentSecond = get_current_second();
	int second = timerExpectedSecond - currentSecond;
	if(second > 0)
	{
		return second;
	}
	else
	{
		return currentSecond - timerExpectedSecond; 
	}
}