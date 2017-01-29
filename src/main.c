/*
 * main.c
 *
 *  Created on: Feb 26, 2012
 *      Author: xinranmsn
 */

#include <avr/io.h>
#include "nokia5110lcd.h"

int main() {
	nokia5110lcd_init();
	nokia5110lcd_printstratpos(0, 0, "Hello");
	while (1);
	return 0;
}
