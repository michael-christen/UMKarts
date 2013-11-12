/*
 * lcd.c
 *
 *  Created on: Nov 12, 2013
 *      Author: knekritz
 */

#include "lcd.h"
#include "drivers/mss_uart/mss_uart.h"

void LCD_init() {
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
}

void LCD_clear() {
	uint8_t clearCommand[2];
	clearCommand[0] = 0xfe;
	clearCommand[1] = 0x01;
	MSS_UART_polled_tx(&g_mss_uart1, clearCommand, 2);
}

void LCD_print_append(char *string) {
	MSS_UART_polled_tx_string(&g_mss_uart1, string);
}

void LCD_print(char *string) {
	LCD_clear();
	LCD_print_append(string);
}
