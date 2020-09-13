#include "../Logic/display_extensions.h"
#include "clock.h"

int hour = 12, minute = 0, second = 0;

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

void display_clock(bool displayHour, bool displayMinute)
{
	draw_clock(0,0, hour, minute, displayHour, displayMinute);
	draw_number(60,4,second);
}