#include "../Logic/display_extensions.h"
#include "clock.h"

 int hour = 12;
 int clockSelectedHour = 12;
 int minute = 0;
 int clockSelectedMinute= 0;
 int second = 0;

void trigger()
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

void clock_add_hour()
{
	clockSelectedHour = add_hour(clockSelectedHour);
}

void clock_add_minute()
{
	clockSelectedMinute = add_minute(clockSelectedMinute);
}

void clock_subtract_hour()
{
	clockSelectedHour = subtract_hour(clockSelectedHour);
}

void clock_subtract_minute()
{
	clockSelectedMinute = subtract_minute(clockSelectedMinute);
}

void clock_save()
{
	second = 0;
	hour = clockSelectedHour;
	minute = clockSelectedMinute;
}

void display_clock_view()
{
	draw_string(0, 0, "CLOCK");
	display_clock(hour, minute, 1,1,1);
}

void display_clock_hour_set_view()
{
	draw_string(0, 0, "CLOCK SET H");
	display_clock(clockSelectedHour, clockSelectedMinute, 1,0,0);
}

void display_clock_minute_set_view()
{
	draw_string(0, 0, "CLOCK SET M");
	display_clock(clockSelectedHour, clockSelectedMinute, 0,1,0);
}

void display_clock(int hour, int minute, bool displayHour, bool displayMinute, bool displaySeconds)
{
	draw_clock(0,2, hour, minute, displayHour, displayMinute);
	char seconds[2];
	sprintf(seconds, "%d", second);
	
	if(displaySeconds == 1)
	draw_string(110,12,seconds);
}