#include <avr/io.h>
#include "avr/interrupt.h"
#include <util/delay.h>
#include <util/twi.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdbool.h>
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

uint8_t I2COutBuffer;

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

volatile uint8_t hour = 17;
volatile uint8_t minute = 30;
volatile uint8_t second = 0;

bool clockChangedFlag = 0;

const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence

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
	//
	0x22, 0x00, 0x3f,	// Set Page Address (start,end)
	0x21, 0x00,	0x7f,	// Set Column Address (start,end)
	//
	// 0xD6, 0x01,		// Set Zoom In, 0=disabled, 1=enabled
/*
	0xAE,			// Display OFF (sleep mode)
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, , 10=Page, 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// --set start line address
	0x81, 0x3F,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
	0xA4,			// Output RAM to Display - 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5, 0xF0,		// --set display clock divide ratio/oscillator frequency
	0xD9, 0x22,		// Set pre-charge period
	0xDA, 0x12,		// Set com pins hardware configuration
	0xDB, 0x20,		// --set vcomh, 0x20,0.77xVcc
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0xAF,			// Display ON in normal mode
*/
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
		// TODO: QUESTION - Is this correct: ssd1306_setpos() and then just ssd1306_data_byte() ???
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
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		// TODO: QUESTION - Is this correct: ssd1306_setpos() and then just ssd1306_data_byte() ???
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

// ----------------------------------------------------------------------------

void ssd1306tx_char(char ch) {
	uint16_t j = (ch << 2) + (ch << 1) - 192; // Equiv.: j=(ch-32)*6 <== Convert ASCII code to font data index.
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

int main(void)
{
	sei();
	DDRD |= LED1;
	DDRB |= LED2;
	
	DDRC &= ~(BUTTON3 | BUTTON4);
	DDRB &= ~(BUTTON1 | BUTTON2);
	
	//Timer 1 - 1 sekunda
	TCCR1B = (1<<CS12) | (1<<WGM12);
	OCR1AH = 0x7A;
	OCR1AL = 0x12;
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);
	
	I2C_Initialize(100);
	
	I2COutBuffer = 0;
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font8x16data, ' ');
	ssd1306_clear_display();
	//ssd1306_draw_bmp(0,0,128,8,img0s128x64c1);
	ssd1306_draw_bmp(0,0, 24,4, number0);
	ssd1306_draw_bmp(24,0, 48, 4, number1);
	ssd1306_draw_bmp(48,0, 72, 4, number2);
	ssd1306_draw_bmp(72,0, 96, 4, number3);
	ssd1306_draw_bmp(96,0, 120, 4, number4);
	ssd1306_draw_bmp(0,4, 24, 8, number5);
	ssd1306_draw_bmp(24,4, 48, 8, number6);
	ssd1306_draw_bmp(48,4, 72, 8, number7);
	ssd1306_draw_bmp(72,4, 96, 8, number8);
	ssd1306_draw_bmp(96,4, 120, 8, number9);
	while (1) 
    {
		//PORTB ^= LED5;
		//_delay_ms(1000);
			/*I2C_start();
			
			
			I2C_write(PCFaddrW);
			I2C_write(0b00100000);
			PORTB ^= LED5;
			I2C_stop();
			_delay_ms(100);
						
			I2C_start();			
			I2C_write(PCFaddrW);			
			I2C_write(0b01011001);
			PORTB ^= LED5;
			I2C_stop();
			_delay_ms(100); */
			//ssd1306_init();
			//ssd1306_fill4(0xC0, 0x30, 0x0C, 0x03);
			
			_delay_ms(500);
			ssd1306_start_command();
			I2C_write(0xA6);
			ssd1306_stop();
			_delay_ms(500);
			ssd1306_start_command();
			I2C_write(0xA7);
			ssd1306_stop();
			//ssd1306_setpos(0, 0);
			//ssd1306tx_stringxy(ssd1306tx_font_src, 0, 0, "21/37");
			/*
			ssd1306_setpos(7, 1);
			ssd1306tx_string("SSD1306xLED Library");
			ssd1306_setpos(0, 2);
			ssd1306tx_string("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[5]^_`abcdefghijklmnopqrstuvwxyz~");
			ssd1306_setpos(0, 6);
			ssd1306tx_string(" This is the TINUSUR ");
			ssd1306_setpos(7, 7);
			ssd1306tx_string("http://tinusaur.org");
			_delay_ms(500);
			*/
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