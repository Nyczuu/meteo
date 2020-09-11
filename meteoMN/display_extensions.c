#include <stdbool.h> 
#include "fonts.h"
#include "display.h"
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

void draw_string(int xstart, int ystart, char* str, int stringLenght)
{
	ssd1306tx_stringxy(ssd1306xled_font8x16data, xstart, ystart, str);
}

void draw_clock(int xstart, int ystart, int hours, int minutes, bool displayHour, bool displayMinute)
{
	if(displayHour)
	{
		draw_number(xstart,ystart, hours);
	}
	
	draw_string(xstart +48, ystart +1, ":",1);
	
	if(displayMinute)
	{	
		draw_number(xstart + 60, ystart, minutes);
	}
}