#include <stdbool.h> 

void alarm_reset();
void alarm_run();
void alarm_trigger();

void alarm_add_hour();
void alarm_add_minute();
void alarm_subtract_hour();
void alarm_subtract_minute();

bool alarm_is_running();
bool alarm_is_ready();

void display_alarm_view();
void display_alarm_hour_set_view();
void display_alarm_minute_set_view();