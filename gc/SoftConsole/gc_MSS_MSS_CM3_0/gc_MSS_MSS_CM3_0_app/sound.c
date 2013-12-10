#include "sound.h"
#include "spi_flash.h"
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_timer/mss_timer.h"

static uint8_t sound_playing1 = 0;
static uint32_t cur_addr1 = 0;
static uint32_t stop_addr1 = 0;
static uint32_t	buf_start1 = -1;
static uint32_t repeat_addr1 = -1;
static uint8_t sound_buf1[512];

static uint8_t sound_playing2 = 0;
static uint32_t cur_addr2 = 0;
static uint32_t stop_addr2 = 0;
static uint32_t	buf_start2 = -1;
static uint32_t repeat_addr2 = -1;
static uint8_t sound_buf2[512];

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

void sound_play_start(uint32_t begin, uint32_t end, int repeat) {
	// If we're already playing the same sound, don't start it over.
	if ((sound_playing1 && end == stop_addr1) || (sound_playing2 && end == stop_addr2))
		return;

	if (!sound_playing1) {
		cur_addr1 = begin;
		stop_addr1 = end;
		if (repeat)
			repeat_addr1 = begin;
		else
			repeat_addr1 = -1;
		sound_playing1 = 1;
	} else {
		cur_addr2 = begin;
		stop_addr2 = end;
		if (repeat)
			repeat_addr2 = begin;
		else
			repeat_addr2 = -1;
		sound_playing2 = 1;
	}

	MSS_TIM1_start();
}

void sound_play_repeat(uint32_t begin, uint32_t end) {
	sound_play_start(begin, end, 1);
}

void sound_play(uint32_t begin, uint32_t end) {
	sound_play_start(begin, end, 0);
}

void sound_stop() {
	repeat_addr1 = -1;
	stop_addr1 = 0;
	
	repeat_addr2 = -1;
	stop_addr2 = 0;
}

__attribute__ ((interrupt)) void Timer1_IRQHandler( void ){
	uint32_t sample = 0;

	if (sound_playing1) {
		if (cur_addr1 < buf_start1 || cur_addr1 >= (buf_start1 + sizeof(sound_buf1))) {
			buf_start1 = cur_addr1 & 0xFFFFFFFE;
			spi_flash_read(buf_start1, &sound_buf1, sizeof(sound_buf1));
		}
		
		sample += sound_buf1[cur_addr1 - buf_start1];
		
		cur_addr1++;
		
		if (cur_addr1 >= stop_addr1) {
			if (repeat_addr1 != -1) {
				cur_addr1 = repeat_addr1;
			} else {
				sound_playing1 = 0;
			}
		}
	}
	
	if (sound_playing2) {
		if (cur_addr2 < buf_start2 || cur_addr2 >= (buf_start2 + sizeof(sound_buf2))) {
			buf_start2 = cur_addr2 & 0xFFFFFFFE;
			spi_flash_read(buf_start2, &sound_buf2, sizeof(sound_buf2));
		}
		
		sample += sound_buf2[cur_addr2 - buf_start2];
		
		cur_addr2++;
		
		if (cur_addr2 >= stop_addr2) {
			if (repeat_addr2 != -1) {
				cur_addr2 = repeat_addr2;
			} else {
				sound_playing2 = 0;
			}
		}
	}
	
	if (sound_playing1 && sound_playing2) {
		sample -= 128;
	}
	
	ACE_set_sdd_value(SDD1_OUT, sample);
	
	if (!sound_playing1 && !sound_playing2) {
		MSS_TIM1_stop();
	}

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
