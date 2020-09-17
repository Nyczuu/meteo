#include "tempertature.h"
#include "../External/DHT/dht.h"

float temperature[1];
float humidity[1];

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

void display_temperature_view()
{
	draw_string(0, 0, "TEMPERATURE");
	
	char tempAsChar[5];
	
	DHT_Read(temperature, humidity);
	ftoa(temperature[0],tempAsChar,2);
	
	switch (DHT_GetStatus())
	{
		case (DHT_Ok):
		draw_string(0,2,tempAsChar);
		break;
	}
}
