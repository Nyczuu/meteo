#include <avr/io.h>
#include <avr/pgmspace.h>
#include "display.h"

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

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
