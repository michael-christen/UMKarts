/*
 * lcd.h
 *
 *  Created on: Nov 12, 2013
 *      Author: knekritz
 */

#ifndef LCD_H_
#define LCD_H_

/*
 * LCD display connections:
 * VDD - 5v
 * GND - ground
 * RX - F5
 *
 * Example usage:
 *
 * LCD_init();
 * LCD_printf("%s %s", "Hello", "World");
 *
 */

#define LCD_printf(...) {char LCD_printf_buf[80];snprintf(LCD_printf_buf, 80, __VA_ARGS__);LCD_print(LCD_printf_buf);}

void LCD_init();

void LCD_clear();

void LCD_print_append(char *string);

void LCD_print(char *string);

#endif /* LCD_H_ */
