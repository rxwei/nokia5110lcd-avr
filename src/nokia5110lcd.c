/*
 * nokia5110lcd.c
 *
 *  Created on: Apr 11, 2012
 *      Author: xinranmsn
 */

#include "nokia5110lcd.h"
#include "nokia5110lcd_bmp.h"
#include <avr/io.h>
#include <util/delay.h>

#define nokia5110lcd_shared

#ifdef nokia5110lcd_shared
#define nokia5110lcd_shared_ddr DDRD
#define nokia5110lcd_shared_port PORTD
#endif

#define nokia5110lcd_rst_ddr  DDRD
#define nokia5110lcd_rst_port PORTD
#define nokia5110lcd_rst_bit  _BV(4)
static inline void rst_set()  { nokia5110lcd_rst_port |= nokia5110lcd_rst_bit;  }
static inline void rst_rst()  { nokia5110lcd_rst_port &= ~nokia5110lcd_rst_bit; }

#define nokia5110lcd_ce_ddr   DDRD
#define nokia5110lcd_ce_port  PORTD
#define nokia5110lcd_ce_bit   _BV(0)
static inline void ce_set()   { nokia5110lcd_ce_port |= nokia5110lcd_ce_bit;  }
static inline void ce_rst()   { nokia5110lcd_ce_port &= ~nokia5110lcd_ce_bit; }

#define nokia5110lcd_dc_ddr   DDRD
#define nokia5110lcd_dc_port  PORTD
#define nokia5110lcd_dc_bit   _BV(1)
static inline void dc_set()   { nokia5110lcd_dc_port |= nokia5110lcd_dc_bit;  }
static inline void dc_rst()   { nokia5110lcd_dc_port &= ~nokia5110lcd_dc_bit; }

#define nokia5110lcd_din_ddr  DDRD
#define nokia5110lcd_din_port PORTD
#define nokia5110lcd_din_bit  _BV(2)
static inline void din_set()  { nokia5110lcd_din_port |= nokia5110lcd_din_bit;  }
static inline void din_rst()  { nokia5110lcd_din_port &= ~nokia5110lcd_din_bit; }

#define nokia5110lcd_clk_ddr  DDRD
#define nokia5110lcd_clk_port PORTD
#define nokia5110lcd_clk_bit  _BV(3)
static inline void clk_set()  { nokia5110lcd_clk_port |= nokia5110lcd_clk_bit;  }
static inline void clk_rst()  { nokia5110lcd_clk_port &= ~nokia5110lcd_clk_bit; }

static void writedat(u8 dat);
static void writecmd(u8 cmd);

#define delay_us _delay_us
#define delay_ms _delay_ms

void nokia5110lcd_init(void)
{
#ifdef nokia5110lcd_shared
	nokia5110lcd_shared_ddr |= (nokia5110lcd_rst_bit | nokia5110lcd_ce_bit | nokia5110lcd_dc_bit | nokia5110lcd_din_bit | nokia5110lcd_clk_bit);
#else
	//
#endif // nokia5110lcd_shared

	rst_set();
	delay_us(1);
	rst_rst();
	delay_us(1);
	rst_set();

	ce_rst();
	delay_us(1);
	ce_set();
	delay_us(1);

	writecmd(0x21);
	writecmd(0xc8);
	writecmd(0x06);
	writecmd(0x13);
	writecmd(0x20);
	nokia5110lcd_clr();
	writecmd(0x0c);

	ce_rst();
}

void nokia5110lcd_clr(void)
{
	writecmd(0x0c);
	writecmd(0x80);
	for (u16 i = 0; i < 504; i++)
		writedat(0x00);
}

void nokia5110lcd_setpos(u8 x, u8 y)
{
	writecmd(0x40 | y);
	writecmd(0x80 | x);
}

void nokia5110lcd_printchar(unsigned char c)
{
	c -= 32;
	for (u8 line = 0; line < 6; line++)
		writedat(nokia5110lcd_bmp_ascii[c][line]);
}

void nokia5110lcd_drawbmpatpos(u8 posx, u8 posy, u8 *bmp, u8 width, u8 height)
{
	u8 row;
	if (width % 8 == 0)
		row = height / 8;
	else
		row = height / 8 + 1;
	for (u16 n = 0; n < row; n++) {
		nokia5110lcd_setpos(posx, posy);
		for (u16 i = 0; i < width; i++)
			writedat(bmp[i + n * width]);
		posy++;
	}
}

void nokia5110lcd_printstratpos(u8 posx, u8 posy, char *str)
{
	nokia5110lcd_setpos(posx, posy);
	while (str) {
		nokia5110lcd_printchar(*str);
		str++;
	}
}

static void writedat(u8 dat)
{
	ce_rst();
	dc_set();
	for (u8 i = 0; i < 8; i++) {
		if (dat & 0x80)
			din_set();
		else
			din_rst();
		clk_rst();
		dat <<= 1;
		clk_set();
	}
	ce_set();
}

static void writecmd(u8 cmd)
{
	ce_rst();
	dc_rst();
	for (u8 i = 0; i < 8; i++) {
		if (cmd & 0x80)
			din_set();
		else
			din_rst();
		clk_rst();
		cmd <<= 1;
		clk_rst();
	}
	ce_set();
}
