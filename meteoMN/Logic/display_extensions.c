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

//char * numberToCharArray(uint8_t number)
//{
//if(number == 0) return number0;
//else if(number == 1) return number1;
//else if(number == 2) return number2;
//else if(number == 3) return number3;
//else if(number == 4) return number4;
//else if(number == 5) return number5;
//else if(number == 6) return number6;
//else if(number == 7) return number7;
//else if(number == 8) return number8;
//else if(number == 9) return number9;
//}

//void draw_digit(uint8_t xstart, uint8_t ystart, uint8_t digit)
//{
//ssd1306_draw_bmp(xstart, ystart, xstart+24, ystart+4, numberToCharArray(digit));
//}

//void draw_number(uint8_t xstart, uint8_t ystart, uint8_t number)
//{
//uint8_t first, second;
//
//if (number > 99)
//{
//return;
//}
//else if (number > 9)
//{
//first = number/10;
//second = number%10;
//}
//else
//{
//first = 0;
//second = number;
//}
//
//draw_digit(xstart, ystart, first);
//draw_digit(xstart+ 24,ystart, second);
//}

void draw_number(uint8_t xstart, uint8_t ystart, uint8_t number)
{
	char number_as_char[2];
	
	if(number < 10)
	sprintf(number_as_char, "0%d", number);
	else
	sprintf(number_as_char, "%d", number);
	
	draw_string(xstart + 110, ystart + 8,number_as_char);
}

void draw_string(uint8_t xstart, uint8_t ystart, char* str)
{
	ssd1306tx_stringxy(ssd1306xled_font8x16data, xstart, ystart, str);
}

void draw_clock(uint8_t xstart, uint8_t ystart, uint8_t hours, uint8_t minutes, uint8_t seconds, bool displayHour, bool displayMinute, bool displaySecond)
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
		draw_number(xstart + 110, ystart+8,seconds);
	}
}

void draw_its_time()
{
	draw_string(2,2,"IT IS TIME");
	draw_string(2,4,"IT IS TIME");
	draw_string(2,6,"IT IS TIME");
}
