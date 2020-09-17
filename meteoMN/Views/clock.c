#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "clock.h"

void clock_trigger()
{
	uint8_t previous = current_second;
	current_second = add_safe_second(current_second);
	
	if(previous == (SECONDS_IN_MINUTE - 1) && current_second == 0)
	{
		previous = current_minute;
		current_minute = add_safe_minute(current_minute);
	}
	
	if(previous == (MINUTES_IN_HOUR - 1) && current_minute == 0)
	current_hour = add_safe_hour(current_hour);
}

void clock_save()
{
	current_second = 0;
	current_hour = selectedHour;
	current_minute	= selectedMinute;
}

void display_clock_view()
{
	draw_header("CLOCK",timer_is_running(),alarm_is_running());
	display_clock(current_hour, current_minute,current_second, 1,1,1);
}

void display_clock_hour_set_view()
{
	draw_header("CLOCK SET H",timer_is_running(),alarm_is_running());
	display_clock(selectedHour, selectedMinute, 0,1,0,0);
}

void display_clock_minute_set_view()
{
	draw_header("CLOCK SET M",timer_is_running(),alarm_is_running());
	display_clock(selectedHour, selectedMinute, 0, 0, 1, 0);
}

void display_clock(uint8_t hour, uint8_t minute, uint8_t second, bool displayHour, bool displayMinute, bool displaySeconds)
{
	draw_clock(0,2, hour, minute, second, displayHour, displayMinute, displaySeconds);
}