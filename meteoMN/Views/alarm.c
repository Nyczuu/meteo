#include <util/delay.h>
#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "alarm.h"
#include "clock.h"

bool alarmIsReady = 0;
bool alarmIsRunning = 0;

int alarmSelectedHour = 0;
int alarmSelectedMinute = 0;

int alarmExpectedHour = 0;
int alarmExpectedMinute = 0;
int alarmExpectedSecond = 0;


void alarm_reset()
{
	alarmIsReady = 0;
	alarmIsRunning = 0;
	
	alarmSelectedHour = 0;
	alarmSelectedMinute = 0;
	
	alarmExpectedHour = 0;
	alarmExpectedMinute = 0;
	alarmExpectedSecond = 0;
}

void alarm_run()
{
	if(alarmIsRunning == 0)
	{
		alarmExpectedSecond = 0;
		alarmExpectedHour = alarmSelectedHour;
		alarmExpectedMinute = alarmSelectedMinute;
		alarmIsRunning = 1;
	}
}

void alarm_trigger()
{
	if(alarmIsRunning == 1 
	&& alarmIsReady == 0
	&& get_current_hour() == alarmExpectedHour 
	&& get_current_minute() == alarmExpectedMinute 
	&& get_current_second() == alarmExpectedSecond)
	{
		alarmIsReady = 1;	
	}
}

void alarm_add_hour()
{
	alarmSelectedHour = add_hour(alarmSelectedHour);
}

void alarm_add_minute()
{
	alarmSelectedMinute = add_minute(alarmSelectedMinute);
}

void alarm_subtract_hour()
{
	alarmSelectedHour = subtract_hour(alarmSelectedHour);
}

void alarm_subtract_minute()
{
	alarmSelectedMinute = subtract_minute(alarmSelectedMinute);
}

bool alarm_is_running()
{
	return alarmIsRunning;
}

bool alarm_is_ready()
{
	return alarmIsReady;
}

void display_alarm_view()
{
	draw_string(0, 0, "ALARM");
	display_alarm(alarmExpectedHour,alarmExpectedMinute,0,1,1,0);
}

void display_alarm_hour_set_view()
{
	draw_string(0, 0, "TIMER SET H");
	display_alarm(alarmSelectedHour,alarmSelectedMinute,0,1,0,0);
}

void display_alarm_minute_set_view()
{
	draw_string(0, 0, "TIMER SET M");
	display_alarm(alarmSelectedHour,alarmSelectedMinute,0,0,1,0);
}

void display_alarm(int hour, int minute, int second, bool displayHour, bool displayMinute, bool displaySeconds)
{
	if(alarmIsRunning == 0)
	{
		draw_clock(0,2, hour, minute,second, displayHour, displayMinute, displaySeconds);
	}
	else if(alarmIsRunning == 1 && alarmIsReady == 0)
	{
		draw_clock(0,2, hour, minute, second, displayHour, displayMinute, displaySeconds);
	}
	else
	{
		draw_its_time();
	}
}