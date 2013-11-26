#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "controller.h"
#include "lcd.h"
#include "item.h"
#include "lasertag.h"
#include "drivers/mss_ace/mss_ace.h"
#include "sound.h"
#include "sound_samples.h"


volatile uint32_t count;

volatile int curClock, prevClock;
const double CONVERSION_FACTOR = 0.00017006802;
int lastVals[10];
int arrCount=0;



__attribute__ ((interrupt)) void GPIO2_IRQHandler( void ){
	printf("Reflective sensor sees something\n\r");
	handleItemGrab();
	MSS_GPIO_clear_irq( MSS_GPIO_2 );
}
__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{

    //uint32_t time = MOTOR_getCounterVal();
    uint32_t status = MOTOR_getInterrupt_status();
    /*

    printf("%d Count \n\r", (int)count);
    if(status & 0x01 && count > 1000)
    {
        MOTOR_set_speed(.5);
        count = 0;
    }

    if(status & 0x02)
    {
        printf("Compare latency %ld\n\r", (1<<27) - time);
    }
    if(status == 12)
    {
            printf("Sync time: %ld, Async time: %ld, diff: %ld\n\r", sync_out, async_out, sync_out - async_out);
    }
    */
    count ++;
    NVIC_ClearPendingIRQ( Fabric_IRQn );
}

int main()
{
	sound_init();
        volatile int d = 0;
        MOTOR_cmpVal = 2000000;
        MOTOR_period = 20000000;
        curClock = prevClock = 0;
   /* Setup MYTIMER */
        MOTOR_init();
        MOTOR_set_speed(0);
        MOTOR_set_servo_direction(0);

        printf("Mike Loves Double Dash!!!\r\n");
        count = 0;
        int lastVal = 1;
        double speed = 0;
        int dir = 1;

        CONTROLLER_setup_mem();

        MSS_GPIO_init();
        MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE);
        MSS_GPIO_enable_irq(MSS_GPIO_2);
        MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
        MSS_GPIO_enable_irq(MSS_GPIO_0);
        initItemWeights();

        LASER_TAG_init();

        LASER_TAG_shoot();

        int x = 1;
        LCD_init();
        LCD_printf("%s %s", "Hello", "World");
        /*
        while(1) {
        	LCD_printf("Hi! %d", x++);
        	CONTROLLER_print();
        	for (d = 0; d < 5000000; d++);
        	LCD_printf("%s %s", "Hello", "World");
        	CONTROLLER_print();
        	for (d = 0; d < 5000000; d++);

        }
        */

        while( 1 )
        {

        	//CONTROLLER_print();
        	CONTROLLER_load();
                if(CONTROLLER->a) {
                        MOTOR_set_speed(1);
                }
                else if(CONTROLLER->b) {
                        MOTOR_set_speed(-1);
                }
                else {
                        MOTOR_set_speed(0);
                }

                if(CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
                        MOTOR_set_servo_direction(1);
                }
                else if(CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
                        MOTOR_set_servo_direction(-1);
                }
                else {
                        MOTOR_set_servo_direction(0);
                }

                if (CONTROLLER->l) {
                	useCurrentItem();
                } else if (CONTROLLER->x) {
                	handleItemGrab();
                } else if (CONTROLLER->y) {
                	use_green_shell();
                }

                //for (d = 0; d < 1000000; d++);
        }
}
