/*
 * nokia5110lcd.h
 *
 *  Created on: Apr 11, 2012
 *      Author: xinranmsn
 */

#ifndef NOKIA5110LCD_H_
#define NOKIA5110LCD_H_

typedef unsigned char u8;
typedef unsigned int u16;

void nokia5110lcd_init(void);
void nokia5110lcd_clr(void);
void nokia5110lcd_setpos(u8 x, u8 y);
void nokia5110lcd_drawbmpatpos(u8 posx, u8 posy, unsigned char *bmp, u8 width, u8 height);
void nokia5110lcd_printchar(unsigned char c);
void nokia5110lcd_printstratpos(u8 posx, u8 posy, char *str);

#endif /* NOKIA5110LCD_H_ */
