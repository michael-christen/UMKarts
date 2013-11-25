#include "sound.h"
#include "spi_flash.h"
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_timer/mss_timer.h"

static uint32_t cur_addr = 0;
static uint32_t stop_addr = 0;
static uint32_t	buf_start = -1;
static uint8_t sound_buf[512];

void sound_init() {
	spi_flash_init();

	ACE_init();

	/* DAC initialization */
	ACE_configure_sdd(
		SDD1_OUT,
		SDD_8_BITS,
		SDD_VOLTAGE_MODE | SDD_RETURN_TO_ZERO,
		INDIVIDUAL_UPDATE
	);
	ACE_enable_sdd(SDD1_OUT);

	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);

	// 8000 Hz
	MSS_TIM1_load_background((uint32_t) 100000000/8000);

	MSS_TIM1_enable_irq();
}

void sound_play(uint32_t begin, uint32_t end) {
	cur_addr = begin;
	stop_addr = end;

	MSS_TIM1_start();
}

__attribute__ ((interrupt)) void Timer1_IRQHandler( void ){
	uint8_t buf;

	if (cur_addr < buf_start || cur_addr >= (buf_start + sizeof(sound_buf))) {
		buf_start = cur_addr & 0xFFFFFFFE;
		spi_flash_read(buf_start, &sound_buf, sizeof(sound_buf));
	}

	ACE_set_sdd_value(SDD1_OUT, (uint32_t) sound_buf[cur_addr - buf_start]);

	cur_addr++;

	if (cur_addr >= stop_addr)
		MSS_TIM1_stop();

	MSS_TIM1_clear_irq();


}

void flash_write() {
	spi_flash_init();

	uint32_t addr = 0;
	uint8_t buf;

	MSS_UART_init( &g_mss_uart0,
			MSS_UART_19200_BAUD,
				       MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );

	spi_flash_control_hw(SPI_FLASH_GLOBAL_UNPROTECT, 0, 0);
	spi_flash_control_hw(SPI_FLASH_CHIP_ERASE, 0, 0);
	while (1) {
		while (MSS_UART_get_rx(&g_mss_uart0, &buf, 1) == 0);
		spi_flash_write(addr, &buf, 1);
		addr++;
	}
}
