#include <stdbool.h> 

void timer_reset();
void timer_add_hour();
void timer_add_minute();
void timer_subtract_hour();
void timer_subtract_minute();
void timer_run();

bool timer_is_running();

void display_timer_view();
void display_timer_hour_set_view();
void display_timer_minute_set_view();