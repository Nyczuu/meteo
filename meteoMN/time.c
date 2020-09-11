#include "time.h"

int add_hour(int number);
int add_minute(int number);
int subtract_hour(int number);
int subtract_minute(int number);
int add_number(int number, int max);
int subtract_number(int number, int max);

int add_hour(int number)
{
	return add_number(number, DAY_HOURS - 1);
}

int add_minute(int number)
{
	return add_number(number, HOUR_MINUTES - 1);
}

int subtract_hour(int number)
{
	return subtract_number(number, DAY_HOURS - 1);
}

int subtract_minute(int number)
{
	return subtract_number(number, HOUR_MINUTES - 1);
}

int add_number(int number, int max)
{
	number++;
	
	if(number > max)
	{
		number = 0;
	}
	
	return number;
}

int subtract_number(int number, int max)
{
	number--;
	
	if(number < 0)
	{
		number = max;
	}
	
	return number;
}