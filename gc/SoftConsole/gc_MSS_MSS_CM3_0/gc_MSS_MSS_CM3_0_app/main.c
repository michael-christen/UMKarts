#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "xbee_interface.h"
#include "xbee.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
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
	struct xbee_packet * xbee_read_packet;
	/* Initialize the XBee interface */
	int err = xbee_interface_init();
	if (err != 0) {
		return 0;
	}
	else {
		xbee_printf("XBee successfully initiated\n");
	}

	player_init();
	send_message_init();

	sound_init();

	xbee_printf("Sound initialized\n");
	//volatile int d = 0;
	MOTOR_cmpVal = 2000000;
	MOTOR_period = 20000000;
	curClock = prevClock = 0;
	/* Setup MYTIMER */
	MOTOR_init();
	MOTOR_set_speed(0);
	MOTOR_set_servo_direction(0);

	xbee_printf("Mike Loves Double Dash!!!\n");
	count = 0;
	int lastVal = 1;
	double speed = 0;
	int dir = 1;

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

	int x = 1;
	LCD_init();
	xbee_printf("%s %s\n", "Hello", "World");

	driver_discovery();
	player_discovery();

	while( 1 )
	{
		//CONTROLLER_print();
		CONTROLLER_load();
		if (CONTROLLER->a) {
			MOTOR_set_speed(1);
		} else if (CONTROLLER->b) {
			MOTOR_set_speed(-1);
		} else {
			MOTOR_set_speed(0);
		}

		if (CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
			MOTOR_set_servo_direction(1);
		} else if (CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
			MOTOR_set_servo_direction(-1);
		} else {
			MOTOR_set_servo_direction(0);
		}

		if (CONTROLLER->l) {
			useCurrentItem();
		} else if (CONTROLLER->x) {
			handleItemGrab();
		} else if (CONTROLLER->y) {
			use_green_shell();
		}
		while ((xbee_read_packet = xbee_read())) {
			mario_xbee_interpret_packet(xbee_read_packet);
			xbee_interface_free_packet(xbee_read_packet);
		}

		switch (g_game_state) {
		case GAME_WAIT:
			if (CONTROLLER->start) {
				game_trans_wait_to_host();
			}
			break;
		case GAME_HOST:
			if (CONTROLLER->start) {
				/* NEED TO RATE LIMIT */
				message_game_host();
			}
			else {
				message_game_start(g_player_table.players, g_player_table.size);
			}
			break;
		case GAME_OVER:
			if (CONTROLLER->start) {
				game_trans_over_to_wait();
			}
			break;
		default:
			/* We don't care */
		}
	}

	return 0;
}
