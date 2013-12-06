#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "xbee_interface.h"
#include "xbee.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_rtc/mss_rtc.h"
#include "controller.h"
#include "lcd.h"
#include "item.h"
#include "lasertag.h"
#include "drivers/mss_ace/mss_ace.h"
#include "sound.h"
#include "sound_samples.h"
#include "mario_xbee.h"
#include "messages.h"
#include "player.h"
#include "player_drive.h"
#include "game.h"


volatile uint32_t count;

volatile int curClock, prevClock;
const double CONVERSION_FACTOR = 0.00017006802;
int lastVals[10];
int arrCount=0;

__attribute__ ((interrupt)) void GPIO1_IRQHandler( void ){
	printf("Magnetic sensor sees something\n\r");
	MSS_GPIO_clear_irq( MSS_GPIO_1 );
}

__attribute__ ((interrupt)) void GPIO2_IRQHandler( void ){
	printf("Reflective sensor sees something\n\r");
	handleItemGrab();
	MSS_GPIO_clear_irq( MSS_GPIO_2 );
}

int main()
{
	uint32_t xbee_rapid_packet_limiter = 0;
	/* Initialize the timer */
	MSS_RTC_init();
	MSS_RTC_start();
	/* End initializing timer */

	struct xbee_packet * xbee_read_packet;
	/* Initialize the XBee interface */
	int err = xbee_interface_init();
	if (err != 0) {
		return 0;
	}
	else {
		xbee_printf("XBee successfully initiated");
	}

	player_init();
	send_message_init();

	sound_init();

	xbee_printf("Sound initialized");
	//volatile int d = 0;
	MOTOR_cmpVal = 20000;
	// Old values
	// 20 mil (to high)
	// 200 thous (to low)
	// 2 mil (to high)
	// 1 mil (to high)
	// 500 thou (to low)
	MOTOR_period = 1000000;
	curClock = prevClock = 0;
	/* Setup MYTIMER */
	MOTOR_init();
	MOTOR_set_speed(0);
	MOTOR_set_servo_direction(0);

	xbee_printf("Mike Loves Double Dash!!!");
	count = 0;

	CONTROLLER_setup_mem();

	// Setting up GPIO interrupts for item pick ups
	MSS_GPIO_init();

	// Reflective Sensor
	MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_2);

	// Magnetic sensor
	MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_1);

	initItemWeights();

	LASER_TAG_init();

	LCD_init();
	xbee_printf("%s %s", "Hello", "World");

	driver_discovery();

	// Sets turns motor off and sets servo to straight
	PLAYER_DRIVE_reset();

	while( 1 )
	{
		//CONTROLLER_print();
		PLAYER_DRIVE_update();
		PLAYER_DRIVE_apply();

		while ((xbee_read_packet = xbee_read())) {
			mario_xbee_interpret_packet(xbee_read_packet);
			xbee_interface_free_packet(xbee_read_packet);
		}

		switch (g_game_state) {
		case GAME_WAIT:
			if (CONTROLLER->start) {
				err = game_trans_wait_to_host();
				if (err < 0) {
					if (MSS_RTC_get_seconds_count() - 1 > xbee_rapid_packet_limiter) {
						driver_discovery();
						xbee_rapid_packet_limiter = MSS_RTC_get_seconds_count();
					}
				}
			}
			break;
		case GAME_HOST:
			if (CONTROLLER->start) {
				/* NEED TO RATE LIMIT */
				if (MSS_RTC_get_seconds_count() - 1 > xbee_rapid_packet_limiter) {
					message_game_host();
					xbee_printf("Hosting game. Registered %d players", g_player_table.size);
					xbee_rapid_packet_limiter = MSS_RTC_get_seconds_count();
				}
			}
			else {
				/* Don't actually start a game until we have more than just us in our table */
				if (g_player_table.size > 1) {
					message_game_start(g_player_table.players, g_player_table.size);
					game_trans_host_to_in_game();
				}
				else {
					game_trans_host_to_wait();
				}
			}
			break;
		case GAME_OVER:
			if (CONTROLLER->start) {
				game_trans_over_to_wait();
			}
			break;
		default:
			/* Do nothing */
			break;
		}
	}

	return 0;
}
