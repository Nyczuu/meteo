#include <stdbool.h>
#include "../External/fonts.h"
#include "../External/display.h"
#include "display_extensions.h"


void display_init()
{
	I2C_Initialize(100);
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font8x16data, ' ');
	ssd1306_start_command();
	I2C_write(0xA6);
	ssd1306_stop();
	ssd1306_clear_display();
}

char * numberToCharArray(int number)
{
	if(number == 0) return number0;
	else if(number == 1) return number1;
	else if(number == 2) return number2;
	else if(number == 3) return number3;
	else if(number == 4) return number4;
	else if(number == 5) return number5;
	else if(number == 6) return number6;
	else if(number == 7) return number7;
	else if(number == 8) return number8;
	else if(number == 9) return number9;
}

void draw_digit(int xstart, int ystart, int digit)
{
	ssd1306_draw_bmp(xstart, ystart, xstart+24, ystart+4, numberToCharArray(digit));
}

void draw_number(int xstart, int ystart, int number)
{
	int first, second;
	
	if (number > 99)
	{
		return;
	}
	else if (number > 9)
	{
		first = number/10;
		second = number%10;
	}
	else
	{
		first = 0;
		second = number;
	}
	
	draw_digit(xstart, ystart, first);
	draw_digit(xstart+ 24,ystart, second);
}

void draw_string(int xstart, int ystart, char* str)
{
	ssd1306tx_stringxy(ssd1306xled_font8x16data, xstart, ystart, str);
}

void draw_new_string(int xstart, int ystart, char* str)
{
	ssd1306_clear_display();
	draw_string(xstart,ystart,str);
}

void draw_clock(int xstart, int ystart, int hours, int minutes, int seconds, bool displayHour, bool displayMinute, bool displaySecond)
{
	if(displayHour == 1)
	{
		draw_number(xstart,ystart, hours);
	}
	
	draw_string(xstart +48, ystart +1, ":");
	
	if(displayMinute == 1)
	{
		draw_number(xstart + 60, ystart, minutes);
	}
	
	if(displaySecond == 1)
	{
		char seconds_as_char[2];
		
		if(seconds < 10)
		sprintf(seconds_as_char, "0%d", seconds);
		else
		sprintf(seconds_as_char, "%d", seconds);
		
		if(displaySecond == 1)
		draw_string(xstart + 110, ystart + 8,seconds_as_char);
	}
}

void draw_its_time()
{
	draw_string(2,2,"IT IS TIME!!!");
	draw_string(2,4,"IT IS TIME!!!");
	draw_string(2,6,"IT IS TIME!!!");
}
