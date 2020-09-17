#ifndef DISP_EXT_H
#include <stdint.h>
#include <stdbool.h>

void display_init();
void draw_number(uint8_t xstart, uint8_t ystart, uint8_t number);
void draw_string(uint8_t xstart, uint8_t ystart, char* str);
void draw_its_time();
void draw_clock(uint8_t xstart, uint8_t ystart, uint8_t hours, uint8_t minutes, uint8_t seconds,
bool displayHour, bool displayMinute, bool displaySecond);
draw_header(char* header, bool timer, bool alarm);
#endif
