#ifndef ALARM_H
#define ALARM_H
#include <stdbool.h> 

void alarm_reset();
void alarm_run();
void alarm_trigger();

void display_alarm_view();
void display_alarm_hour_set_view();
void display_alarm_minute_set_view();
#endif