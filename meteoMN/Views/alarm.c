#include "../Logic/display_extensions.h"
#include "../Logic/time.h"
#include "../Logic/buzzer.h"
#include "alarm.h"
#include "clock.h"

bool alarmReady = 0;
bool alarmRunning = 0;

uint8_t alarmExpectedHour = 0;
uint8_t alarmExpectedMinute = 0;
uint8_t alarmExpectedSecond = 0;

bool alarm_is_ready() {return alarmReady;}
bool alarm_is_running() {return alarmRunning;}

void alarm_reset()
{
	PORTD |= LED1;
	PORTD &= ~BUZZER;
	reset();
	
	alarmReady = 0;
	alarmRunning = 0;
	
	alarmExpectedHour = 0;
	alarmExpectedMinute = 0;
	alarmExpectedSecond = 0;
}

void alarm_run()
{
	if(alarmRunning == 0)
	{
		alarmExpectedSecond = 0;
		alarmExpectedHour = selectedHour;
		alarmExpectedMinute = selectedMinute;
		alarmRunning = 1;
	}
}

void alarm_trigger()
{
	if(alarmRunning == 1
	&& alarmReady == 0
	&& current_hour == alarmExpectedHour
	&& current_minute == alarmExpectedMinute
	&& current_second == alarmExpectedSecond)
	{
		alarmReady = 1;
	}
}

void display_alarm_view()
{
	draw_header("ALARM",timer_is_running(),alarm_is_running());
	display_alarm(alarmExpectedHour,alarmExpectedMinute,0,1,1,0);
}

void display_alarm_hour_set_view()
{
	draw_header("ALARM SET H",timer_is_running(),alarm_is_running());
	display_alarm(selectedHour,selectedMinute,0,1,0,0);
}

void display_alarm_minute_set_view()
{
	draw_header("ALARM SET M",timer_is_running(),alarm_is_running());
	display_alarm(selectedHour,selectedMinute,0,0,1,0);
}

void display_alarm(uint8_t hour, uint8_t minute, uint8_t second, bool displayHour, bool displayMinute, bool displaySeconds)
{
	if(alarmRunning == 0)
	{
		draw_clock(0,2, hour, minute,second, displayHour, displayMinute, displaySeconds);
	}
	else if(alarmRunning == 1 && alarmReady == 0)
	{
		draw_clock(0,2, hour, minute, second, displayHour, displayMinute, displaySeconds);
	}
	else
	{
		draw_its_time();
		play_alarm_sound();
	}
}