#include <util/delay.h>
#include "menu.h"

int selectedMenu = MENU_CLOCK;

void welcome()
{
	selectedMenu = MENU_CLOCK;
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
	display_temperature_view();
	else if (selectedMenu == MENU_ALARM)
	display_alarm_view();
	else if (selectedMenu == MENU_ALARM_SET_HOUR)
	display_alarm_hour_set_view();
	else if (selectedMenu == MENU_ALARM_SET_MINUTE)
	display_alarm_minute_set_view();
}

void switch_menu()
{
	if(BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK)
	selectedMenu = MENU_CLOCK_SET_HOUR;

	else if (BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_add_hour();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	clock_subtract_hour();
	
	else if(BUTTON_3_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	selectedMenu = MENU_CLOCK_SET_MINUTE;
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_CLOCK_SET_HOUR)
	selectedMenu = MENU_CLOCK;

	else if (BUTTON_1_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_add_minute();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_subtract_minute();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	clock_set_minute_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_CLOCK_SET_MINUTE)
	selectedMenu = MENU_CLOCK_SET_HOUR;
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER)
	timer_set_hour();
	
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_add_hour();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_subtract_hour();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	selectedMenu = MENU_TIMER_SET_MINUTE;
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_TIMER_SET_HOUR)
	timer_view();

	else if (BUTTON_1_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_add_minute();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_subtract_minute();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_minute_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_TIMER_SET_MINUTE)
	timer_set_hour();
	
	else if (BUTTON_3_PRESSED && selectedMenu == MENU_ALARM)
	alarm_set_hour();
	
	else if (BUTTON_1_PRESSED && selectedMenu == MENU_ALARM_SET_HOUR)
	alarm_add_hour();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_ALARM_SET_HOUR)
	alarm_subtract_hour();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_ALARM_SET_HOUR)
	selectedMenu = MENU_ALARM_SET_MINUTE;
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_ALARM_SET_HOUR)
	alarm_view();

	else if (BUTTON_1_PRESSED && selectedMenu == MENU_ALARM_SET_MINUTE)
	alarm_add_minute();
	
	else if (BUTTON_2_PRESSED && selectedMenu == MENU_ALARM_SET_MINUTE)
	alarm_subtract_minute();

	else if(BUTTON_3_PRESSED && selectedMenu == MENU_ALARM_SET_MINUTE)
	alarm_set_minute_confirm();
	
	else if(BUTTON_4_PRESSED && selectedMenu == MENU_ALARM_SET_MINUTE)
	alarm_set_hour();
	
	else if(BUTTON_1_PRESSED)
	selectedMenu = MENU_CLOCK;
	
	else if (BUTTON_2_PRESSED)
	timer_view();
	
	else if (BUTTON_3_PRESSED)
	alarm_view();
	
	else if (BUTTON_4_PRESSED)
	temperature_view();
}

void clock_set_minute_confirm()
{
	clock_save();
	selectedMenu = MENU_CLOCK;
}

void timer_view()
{
	if(timer_is_running() == 0)
	timer_reset();
	
	selectedMenu = MENU_TIMER;
}

void timer_set_hour()
{
	if(timer_is_running() == 1 && timer_is_ready() == 1)
	timer_reset();

	selectedMenu = MENU_TIMER_SET_HOUR;
}

void timer_set_minute_confirm()
{
	timer_run();
	timer_view();
}

void alarm_view()
{
	if(alarm_is_running() == 0)
	alarm_reset();
	
	selectedMenu = MENU_ALARM;
}

void alarm_set_hour()
{
	if(alarm_is_running() == 1 && alarm_is_ready() == 1)
	alarm_reset();

	selectedMenu = MENU_ALARM_SET_HOUR;
}

void alarm_set_minute_confirm()
{
	alarm_run();
	alarm_view();
}

void temperature_view()
{
	selectedMenu = MENU_TEMPEREATURE;
}
