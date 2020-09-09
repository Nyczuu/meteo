#include <avr/io.h>
#include "avr/interrupt.h"
#include <util/delay.h>
#include <util/twi.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "I2C.h"
//#include "images.h"
#include "stdint.h"
#include "fonts.h"

#define LED1_PORT PORTD
#define LED2_PORT PORTB
#define LED1 (1<<PD7)
#define LED2 (1<<PB0)

#define BUTTON1_PORT PORTB
#define BUTTON2_PORT PORTB
#define BUTTON3_PORT PORTC
#define BUTTON4_PORT PORTC
#define BUTTON1 (1<<PB1)
#define BUTTON2 (1<<PB2)
#define BUTTON3 (1<<PC2)
#define BUTTON4 (1<<PC3)

#define BUTTON1_PRESSED !(PINB & BUTTON1)
#define BUTTON2_PRESSED !(PINB & BUTTON2)
#define BUTTON3_PRESSED !(PINC & BUTTON3)
#define BUTTON4_PRESSED !(PINC & BUTTON4)

#define DISPaddrW 0x78
#define DISPaddrW2 0x7A
#define DISPaddrW3 0x79

#define ACK 1
#define NACK 0

#define MENU_CLOCK 1
#define MENU_CLOCK_SET 20
#define MENU_TIMER 30
#define MENU_TIMER_SET 40
#define MENU_TEMPEREATURE 50
#define MENU_HUMIDITY 60

uint8_t I2COutBuffer;

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

volatile uint8_t hour = 12;
volatile uint8_t minute = 0;
volatile uint8_t second = 0;

volatile uint8_t selected_hour = 12;
volatile uint8_t selected_minute = 0;

bool clockChangedFlag = 0;
bool inClockSet = 0;
bool inTimerSet = 0;

volatile uint8_t selected_menu = 1;

const uint8_t ssd1306_init_sequence [] PROGMEM = {
	
	// Initialization Sequence
	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0xF0,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40 | 0x00,	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
	0xA0 | 0x01,	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0x3F,		// Set contrast control register
	0xD9, 0x22,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x20,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0x2E,			// Deactivate Scroll command
	0xAF,			// Set Display ON/OFF - AE=OFF, AF=ON
	0x22, 0x00, 0x3f,	// Set Page Address (start,end)
	0x21, 0x00,	0x7f,	// Set Column Address (start,end)
};

void ssd1306_start_command(void) {
	I2C_start();
	I2C_write(DISPaddrW);	// Slave address: R/W(SA0)=0 - write
	I2C_write(0x00);			// Control byte: D/C=0 - write command
}

void ssd1306_start_data(void) {
	I2C_start();
	I2C_write(DISPaddrW);	// Slave address, R/W(SA0)=0 - write
	I2C_write(0x40);			// Control byte: D/C=1 - write data
}

void ssd1306_init(void) {
	ssd1306_start_command();	// Initiate transmission of command
	for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++) {
		I2C_write(pgm_read_byte(&ssd1306_init_sequence[i]));	// Send the command out
	}
	I2C_stop();	// Finish transmission
}

void ssd1306_setpos(uint8_t x, uint8_t y) {
	ssd1306_start_command();
	I2C_write(0xb0 | (y & 0x07));	// Set page start address
	I2C_write(x & 0x0f);			// Set the lower nibble of the column start address
	I2C_write(0x10 | (x >> 4));		// Set the higher nibble of the column start address
	I2C_stop();	// Finish transmission
}

void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
	ssd1306_setpos(0, 0);
	ssd1306_start_data();	// Initiate transmission of data
	for (uint16_t i = 0; i < 128 * 8 / 4; i++) {
		I2C_write(p1);
		I2C_write(p2);
		I2C_write(p3);
		I2C_write(p4);
	}
	I2C_stop();	// Finish transmission
}

void ssd1306_data_byte(uint8_t b) {
	I2C_write(b);
}

void ssd1306_stop(void) {
	I2C_stop();
}

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) {
	uint16_t j = 0;
	uint8_t y;
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		ssd1306_setpos(x0, y);
		ssd1306_start_data();
		for (uint8_t x = x0; x < x1; x++) {
			ssd1306_data_byte(pgm_read_byte(&bitmap[j++]));
		}
		
	}
}

void ssd1306_clear_display(void)
{
	uint8_t x0 = 0;
	uint8_t x1 = 128;
	uint8_t y0 = 0;
	uint8_t y1 = 8;
	int16_t j = 0;
	uint8_t y;
	
	if (y1 % 8 == 0)
	y = y1 / 8;
	else
	y = y1 / 8 + 1;
	
	for (y = y0; y < y1; y++) {
		ssd1306_setpos(x0, y);
		ssd1306_start_data();
		for (uint8_t x = x0; x < x1; x++) {
			ssd1306_data_byte(0x00);
		}
	}
}

void ssd1306tx_init(const uint8_t *fron_src, uint8_t char_base) {
	ssd1306tx_font_src = fron_src;
	ssd1306tx_font_char_base = char_base;
}

void ssd1306tx_char(char ch) {
	uint16_t j = (ch << 2) + (ch << 1) - 192;
	ssd1306_start_data();
	for (uint8_t i = 0; i < 6; i++) {
		ssd1306_data_byte(pgm_read_byte(&ssd1306tx_font_src[j + i]));
	}
	ssd1306_stop();
}

void ssd1306tx_string(char *s) {
	while (*s) {
		ssd1306tx_char(*s++);
	}
}

void ssd1306tx_stringxy(const uint8_t *fron_src, uint8_t x, uint8_t y, const char s[]) {
	uint16_t j, k = 0;
	while (s[k] != '\0') {
		j = s[k] * 16 - (32 * 16); // Convert ASCII code to font data index. NOTE: (x*16) already optimized to (x<<4).
		if (x > 120) {
			x = 0;    // Go to the next line.
			y++;
		}
		ssd1306_setpos(x, y);
		ssd1306_start_data();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_data_byte(pgm_read_byte(&fron_src[j + i]));
		}
		ssd1306_stop();
		ssd1306_setpos(x, y + 1);
		ssd1306_start_data();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_data_byte(pgm_read_byte(&fron_src[j + i + 8]));
		}
		ssd1306_stop();
		x += 8;
		k++;
	}
}

char* numberToCharArray(int number)
{
	if(number == 0) return number0;
	else if(number == 1) return number1;
	else if(number == 2) return number2;
	else if(number == 3) return number3;
	else if(number == 4) return number4;
	else if(number == 5) return number5;
	else if(number == 6) return number6;
	else if(number == 7) return number7;
	else if(number == 8) return number8;
	else if(number == 9) return number9;
}

int add_hour(int number) {
	return add_number(number,23);
}

int add_minute(int number) {
	return add_number(number,59);
}

int add_number(int number, int max) {
	number++;
	if(number > max)
		number = 0;
	return number; 		
}

void draw_number(int xstart, int ystart, int number){
	
	if(number < 9) {
		ssd1306_draw_bmp(xstart, ystart, xstart+24, ystart+4, number0);
		ssd1306_draw_bmp(xstart+ 24,ystart, xstart+48, ystart+4, numberToCharArray(number));
	}
	else if(number < 99) {
		int first = number/10;
		int second = number%10;
		
		ssd1306_draw_bmp(xstart, ystart, xstart+24, ystart+4, numberToCharArray(first));
		ssd1306_draw_bmp(xstart+ 24,ystart, xstart+48, ystart+4, numberToCharArray(second));
	}
	
	else return;
}

void display_air_humidity(){
	ssd1306_draw_bmp(0,0, 24,4, number3);
}

void display_temperature(){
	ssd1306_draw_bmp(0,0, 24,4, number2);
}

void display_timer_set() {
	inTimerSet = 1;
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 0, 0, "S");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 8, 0, "E");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 16, 0, "T");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 24, 0, " ");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 32, 0, "T");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 40, 0, "I");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 48, 0, "M");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 56, 0, "E");
		ssd1306tx_stringxy(ssd1306xled_font8x16data, 64, 0, "R");
		
		
}

void display_timer() {
	draw_number(0,0,0);
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 48, 1, ":");
	draw_number(60,0,0);
}

void display_clock_set() {
	inClockSet = 1;
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 0, 0, "S");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 8, 0, "E");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 16, 0, "T");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 24, 0, " ");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 32, 0, "C");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 40, 0, "L");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 48, 0, "O");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 56, 0, "C");
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 64, 0, "K");

	draw_number(0,2,selected_hour);
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 48, 3, ":");
	draw_number(60,2,selected_minute);
}

void display_clock() {
	draw_number(0,0,hour);
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 48, 1, ":");
	draw_number(60,0,minute);
	draw_number(60,4,second);
}

void run(){	
	if(selected_menu == MENU_HUMIDITY) display_air_humidity();
	else if(selected_menu == MENU_TEMPEREATURE) display_temperature();
	else if(selected_menu == MENU_TIMER) display_timer();
	else if(selected_menu == MENU_TIMER_SET) display_timer_set();
	else if(selected_menu == MENU_CLOCK) display_clock();
	else if(selected_menu == MENU_CLOCK_SET) display_clock_set();
}

void switch_menu(int number){
	ssd1306_clear_display();
	selected_menu = number;
	_delay_ms(250);
}

void port_init(){
	
	//DDRD |= LED1;
	//DDRB |= LED2;
	
	DDRC &= ~(BUTTON3 | BUTTON4);
	DDRB &= ~(BUTTON1 | BUTTON2);
	
	TCCR1B = (1<<CS12) | (1<<WGM12);
	OCR1AH = 0x7A;
	OCR1AL = 0x12;
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);
}
void display_init(){
	I2C_Initialize(100);
	I2COutBuffer = 0;
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font8x16data, ' ');
	ssd1306_start_command();
	I2C_write(0xA6);
	ssd1306_stop();
}

int main(void)
{
	sei();
	port_init();
	display_init();

	while (1)
	{
		if(!inClockSet && !inTimerSet)
		{
			if(BUTTON4_PRESSED && selected_menu == MENU_CLOCK) switch_menu(MENU_CLOCK_SET);
			else if(BUTTON4_PRESSED) switch_menu(MENU_CLOCK);
			else if(BUTTON3_PRESSED && selected_menu == MENU_TIMER) switch_menu(MENU_TIMER_SET);
			else if(BUTTON3_PRESSED) switch_menu(MENU_TIMER);
			else if(BUTTON2_PRESSED) switch_menu(MENU_TEMPEREATURE);
			else if(BUTTON1_PRESSED) switch_menu(MENU_HUMIDITY);
		}
		else if(inClockSet)
		{
			if(BUTTON1_PRESSED) { 
				inClockSet = 0;
				switch_menu(MENU_CLOCK);
			};
			if(BUTTON4_PRESSED){
				selected_hour = add_hour(selected_hour);
				_delay_ms(200);
			}
				
		}
		else if (inTimerSet)
		{
			if(BUTTON1_PRESSED) { 
				inTimerSet = 0; 
				switch_menu(MENU_TIMER);
			}
		}
			
		run();
	}
}

ISR (TIMER1_COMPA_vect)
{
	second++;
	if (second > 59)
	{
		second = 0;
		minute++;
		if (minute > 59)
		{
			minute = 0;
			hour++;
			if (hour > 23)
			{
				hour = 0;
			}
			
		}
		clockChangedFlag = 1;
	}
}