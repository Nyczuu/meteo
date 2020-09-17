#ifndef TIME_H
#define TIME_H
#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24

uint8_t selectedHour;
uint8_t selectedMinute;
uint8_t selectedSecond;

void reset();

uint8_t add_safe_hour(uint8_t number);
uint8_t add_safe_minute(uint8_t number);
uint8_t add_safe_second(uint8_t number);

void add_hour();
void add_minute();
void add_second();

void subtract_hour();
void subtract_minute();
void subtract_second();
#endif
