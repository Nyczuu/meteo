#include <stdbool.h> 

void display_init();
void draw_digit(int xstart, int ystart, int digit);
void draw_number(int xstart, int ystart, int number);
void draw_string(int xstart, int ystart, char* str);
void draw_new_string(int xstart, int ystart, char* str);
void draw_clock(int xstart, int ystart, int hours, int minutes, int seconds,
 bool displayHour, bool displayMinute, bool displaySecond);