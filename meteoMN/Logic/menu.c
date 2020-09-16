#include <util/delay.h>
#include "menu.h"

int selectedMenu = MENU_CLOCK;

void welcome()
{
	clock_view();
}

void on_button_pressed()
{
	ssd1306_clear_display();
	switch_menu();
	_delay_ms(200);
}

void refresh_screen()
{
	if(selectedMenu == MENU_CLOCK)
		display_clock_view();
	else if (selectedMenu == MENU_CLOCK_SET_HOUR)
		display_clock_hour_set_view();
	else if (selectedMenu == MENU_CLOCK_SET_MINUTE)
		display_clock_minute_set_view();
	else if (selectedMenu == MENU_TIMER)
		display_timer_view();
	else if (selectedMenu == MENU_TIMER_SET_HOUR)
		display_timer_hour_set_view();
	else if (selectedMenu == MENU_TIMER_SET_MINUTE)
		display_timer_minute_set_view();
	else if (selectedMenu == MENU_TEMPEREATURE)
		temperature_view();
	else if (selectedMenu == MENU_DASHBOARD)
		dashboard_view();
}

void switch_menu()
{
	//Clock
	if(BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK)
	clock_set_hour();

	//Clock set hour
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_set_hour_add();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_set_hour_subtract();
	
	else if(BUTTON_3_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_set_hour_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_view();

	//Clock set minute
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_set_minute_add();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_set_minute_subtract();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_set_minute_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_set_hour();
	
	//Timer
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER)
	{
		timer_reset();
		timer_set_hour();
	}
	
	//Timer set hour
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_set_hour_add();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_set_hour_subtract();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_set_hour_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_view();

	//Timer set minute
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_minute_add();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_minute_subtract();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_minute_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_hour();
	
	//Basic menu
	else if(BUTTON_1_PRESSED)
	clock_view();
	
	else if (BUTTON_2_PRESSED)
	timer_view();
	
	else if (BUTTON_3_PRESSED)
	temperature_view();
	
	else if (BUTTON_4_PRESSED)
	dashboard_view();
}

void clock_view()
{
	selectedMenu = MENU_CLOCK;
}

void clock_set_hour()
{
	selectedMenu = MENU_CLOCK_SET_HOUR;
}

void clock_set_hour_add()
{
	clock_add_hour();
}

void clock_set_hour_subtract()
{
	clock_subtract_hour();
}

void clock_set_hour_confirm()
{
	selectedMenu = MENU_CLOCK_SET_MINUTE;
}

void clock_set_minute_add()
{
	clock_add_minute();
}

void clock_set_minute_subtract()
{
	clock_subtract_minute();
}

void clock_set_minute_confirm()
{
	clock_save();
	clock_view();
}

void timer_view()
{
	if(timer_is_running() == 0)
		timer_reset();
		
	selectedMenu = MENU_TIMER;
}

void timer_set_hour()
{
	selectedMenu = MENU_TIMER_SET_HOUR;
}

void timer_set_hour_add()
{
	timer_add_hour();
}

void timer_set_hour_subtract()
{
	timer_subtract_hour();
}

void timer_set_hour_confirm()
{
	selectedMenu = MENU_TIMER_SET_MINUTE;
}

void timer_set_minute_add()
{
	timer_add_minute();
}

void timer_set_minute_subtract()
{
	timer_subtract_minute();
}

void timer_set_minute_confirm()
{
	timer_run();
	timer_view();
}

void temperature_view()
{
	selectedMenu = MENU_TEMPEREATURE;
	draw_string(0, 0, "TEMPERATURE");
}

void dashboard_view()
{
	selectedMenu = MENU_DASHBOARD;
	draw_string(0, 0, "DASHBOARD");
}
