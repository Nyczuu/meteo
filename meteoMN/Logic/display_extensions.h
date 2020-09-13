#include <stdbool.h> 

void display_init();
void draw_digit(int xstart, int ystart, int digit);
void draw_number(int xstart, int ystart, int number);
void draw_string(int xstart, int ystart, char* str, int stringLenght);
void draw_clock(int xstart, int ystart, int hours, int minutes, bool displayHour, bool displayMinute);