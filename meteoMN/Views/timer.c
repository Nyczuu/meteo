#include <util/delay.h>
#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "timer.h"
#include "clock.h"

int timerSelectedHour = 0;
int timerSelectedMinute = 0;

int timerExpectedHour = 0;
int timerExpectedMinute = 0;
int timerExpectedSecond = 0;

bool timerMinutesElapsed = 0;
bool timerHoursElapsed = 0;

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
	timerMinutesElapsed = 0;
	timerHoursElapsed = 0;
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
		draw_running_timer(displayHour, displayMinute, displaySecond);
	}
	else
	{
		draw_string(2,12,"IT IS TIME!!!");
	}
}

void draw_running_timer(bool displayHour, bool displayMinute, bool displaySecond)
{
	int hour = timer_get_hour();
	int minute = timer_get_minute();
	int second = timer_get_second();
	
	if(second == (SECONDS_IN_MINUTE - 1))
	{
		timerMinutesElapsed = 1;
		_delay_ms(1000);
	}
	
	if(minute == (MINUTES_IN_HOUR - 1))
	{
		timerHoursElapsed = 1;
		_delay_ms(1000);
	}
	
	draw_clock(0,2, hour, minute, second, displayHour, displayMinute, displaySecond);
}

int get_difference(int currentValue, int expectedValue, int maxValue, bool elapsedValue)
{
	int result = 0;
	int distinction = currentValue - expectedValue;
	
	if(distinction < 0)
	result = abs(distinction);
	else
	result = maxValue - distinction;
	
	if(elapsedValue == 1)
	result = result - 1;
	
	if(result == maxValue)
	result = 0;
	
	return result;
}

int timer_get_second()
{
	return get_difference(get_current_second(),timerExpectedSecond, SECONDS_IN_MINUTE, 0);
}

int timer_get_minute()
{
	return get_difference(get_current_minute(),timerExpectedMinute, MINUTES_IN_HOUR, timerMinutesElapsed);
}

int timer_get_hour()
{
	return get_difference(get_current_hour(),timerExpectedHour, HOURS_IN_DAY, timerHoursElapsed);
}
