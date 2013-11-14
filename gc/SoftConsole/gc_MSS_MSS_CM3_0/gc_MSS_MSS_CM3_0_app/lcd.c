/*
 * lcd.c
 *
 *  Created on: Nov 12, 2013
 *      Author: knekritz
 */

#include "lcd.h"
#include "CMSIS/a2fxxxm3.h"
#include "drivers/CoreUARTapb/core_uart_apb.h"

#define LCD_UART_BASE_ADDR (FPGA_FABRIC_BASE + 0x200)
#define BAUD_VALUE_9600 ((100000000 /(9600 * 16)) - 1)

UART_instance_t lcd_uart;

void LCD_init() {
	//MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

	UART_init(&lcd_uart, LCD_UART_BASE_ADDR, BAUD_VALUE_9600, (DATA_8_BITS | NO_PARITY));
}

void LCD_clear() {
	uint8_t clearCommand[2];
	clearCommand[0] = 0xfe;
	clearCommand[1] = 0x01;

	//MSS_UART_polled_tx(&g_mss_uart1, clearCommand, 2);

	UART_send(&lcd_uart, clearCommand, 2);
}

void LCD_print_append(char *string) {
	//MSS_UART_polled_tx_string(&g_mss_uart1, string);
	UART_polled_tx_string(&lcd_uart, string);
}

void LCD_print(char *string) {
	LCD_clear();
	LCD_print_append(string);
}
