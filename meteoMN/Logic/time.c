#include <stdint.h>
#include "time.h"

uint8_t add_number(int8_t number, int8_t max)
{
	number++;
	
	if(number > max)
	{
		number = 0;
	}
	
	return number;
}

uint8_t subtract_number(int8_t number, int8_t max)
{
	number--;
	
	if(number < 0)
	{
		number = max;
	}
	
	return number;
}

uint8_t add_safe_hour(uint8_t number)
{
	return add_number(number, HOURS_IN_DAY- 1);
}

uint8_t add_safe_minute(uint8_t number)
{
	return add_number(number, MINUTES_IN_HOUR- 1);
}

uint8_t add_safe_second(uint8_t number)
{
	return add_number(number, SECONDS_IN_MINUTE- 1);
}


void reset()
{
	selectedHour = 0;
	selectedMinute = 0;
	selectedSecond = 0;
}

void add_hour()
{
	selectedHour = add_number(selectedHour, HOURS_IN_DAY - 1);
}

void add_minute()
{
	selectedMinute = add_number(selectedMinute, MINUTES_IN_HOUR - 1);
}

void add_second()
{
	selectedSecond = add_number(selectedSecond, SECONDS_IN_MINUTE -1);
}

void subtract_hour()
{
	selectedHour = subtract_number(selectedHour, HOURS_IN_DAY - 1);
}

void subtract_minute()
{
	selectedMinute = subtract_number(selectedMinute, MINUTES_IN_HOUR - 1);
}

void subtract_second()
{
	selectedSecond = subtract_number(selectedSecond, SECONDS_IN_MINUTE - 1);
}
