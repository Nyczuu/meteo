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

void draw_string(uint8_t xstart, uint8_t ystart, char* str)
{
	ssd1306tx_stringxy(ssd1306xled_font8x16data, xstart, ystart, str);
}

void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

void ftoa(float n, char* res, int afterpoint)
{
	int ipart = (int)n;
	float fpart = n - (float)ipart;
	int i = intToStr(ipart, res, 0);
	
	if (afterpoint != 0) {
		res[i] = '.';
		fpart = fpart * pow(10, afterpoint);
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void draw_number(uint8_t xstart, uint8_t ystart, uint8_t number)
{
	char number_as_char[3];
	
	if(number < 10)
	{
		intToStr(number, number_as_char, 1);
		number_as_char[1] = number_as_char[0];
		number_as_char[0] = '0';
	}
	else
	{
		intToStr(number, number_as_char, 2);
	}

	number_as_char[2] = '\0';
	
	draw_string(xstart, ystart, number_as_char);
}

void draw_clock(uint8_t xstart, uint8_t ystart, uint8_t hours, uint8_t minutes, uint8_t seconds, bool displayHour, bool displayMinute, bool displaySecond)
{
	if(displayHour == 1)
	{
		draw_number(xstart,ystart, hours);
	}
	
	draw_string(xstart +20, ystart, ":");
	
	if(displayMinute == 1)
	{
		draw_number(xstart + 30, ystart, minutes);
	}
	if(displaySecond == 1)
	{
		draw_string(xstart +50, ystart, ":");
		draw_number(xstart + 60, ystart,seconds);
	}
}

void draw_its_time()
{
	draw_string(2,2,"IT IS TIME");
	draw_string(2,4,"IT IS TIME");
	draw_string(2,6,"IT IS TIME");
}

draw_header(char* header, bool timer, bool alarm)
{
	draw_string(0,0,header);
	if(timer == 1)
	{
		draw_string(110, 0, "-T");
	}
	
	if(alarm == 1)
	{
		draw_string(90, 0, "-A");	
	}
}