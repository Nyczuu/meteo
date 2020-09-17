#ifndef CLOCK_H
#define CLOCK_H

void clock_add_hour();
void clock_add_minute();
void clock_subtract_hour();
void clock_subtract_minute();

void clock_save();
void clock_trigger();

uint8_t current_hour;
uint8_t current_minute;
uint8_t current_second;

void display_clock_view();
void display_clock_hour_set_view();
void display_clock_minute_set_view();
#endif