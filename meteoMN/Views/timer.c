#include "../Logic/display_extensions.h"
#include "timer.h"

int timerSelectedHour = 0;
int timerSelectedMinute = 0;
bool isRunning = 0;

bool timer_is_running()
{
	return isRunning;	
}	

void timer_reset()
{
	timerSelectedHour = 0;
	timerSelectedMinute = 0;
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
	isRunning = 1;
}

void display_timer_view()
{
	draw_string(0, 0, "TIMER");
	display_timer(1,1);
}

void display_timer_hour_set_view()
{
	draw_string(0, 0, "TIMER SET H");
	display_timer(1,0);
}

void display_timer_minute_set_view()
{
	draw_string(0, 0, "TIMER SET M");
	display_timer(0,1);
}

void display_timer(bool displayHour, bool displayMinute)
{
	draw_clock(0,2, timerSelectedHour, timerSelectedMinute, displayHour, displayMinute);
}
