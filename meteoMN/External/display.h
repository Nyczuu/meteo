#include "stdint.h"

#define DISPaddrW 0x78
#define DISPaddrW2 0x7A
#define DISPaddrW3 0x79

void ssd1306_start_command(void);
void ssd1306_start_data(void);
void ssd1306_init(void);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);
void ssd1306_data_byte(uint8_t b);
void ssd1306_stop(void);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]);
void ssd1306_clear_display(void);
void ssd1306tx_init(const uint8_t *fron_src, uint8_t char_base);
void ssd1306tx_char(char ch);
void ssd1306tx_string(char *s);
void ssd1306tx_stringxy(const uint8_t *fron_src, uint8_t x, uint8_t y, const char s[]);