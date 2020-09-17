#include "tempertature.h"
#include "../External/DHT/dht.h"

float temperature[1];
float humidity[1];

void display_temperature_view()
{
	draw_header("WEATHER",timer_is_running(),alarm_is_running());
	
	char tempAsChar[5];
	char humAsChar[5];
	
	DHT_Read(temperature, humidity);
	
	ftoa(temperature[0],tempAsChar,1);
	ftoa(humidity[0],humAsChar,2);
	
	switch (DHT_GetStatus())
	{
		case (DHT_Ok):
		draw_string(0, 2, "TEMP");
		draw_string(50,2,tempAsChar);
		draw_string(0, 4, "HUM");
		draw_string(50,4,humAsChar);
		break;
	}
}
