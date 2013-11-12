#include <stdio.h>
#include <stdint.h>
#include "mytimer.h"
#include "drivers/mss_uart/mss_uart.h"
#include "controller.h"
#include "lcd.h"


volatile uint32_t count;
volatile uint32_t period, cmpVal;
volatile int curClock, prevClock;
const double CONVERSION_FACTOR = 0.00017006802;
int lastVals[10];
int arrCount=0;


// Range between -1 and 1
void setPWM(double percentage) {
        volatile uint32_t lengthOfLow;
        int isNegative = percentage < 0;
        if(isNegative) {
                percentage *= -1;
                MYTIMER_set_motor_direction(-1);
        }
        else {
                MYTIMER_set_motor_direction(1);
        }
        if(percentage < 0.125) {
                MYTIMER_set_motor_direction(0);
        }
        lengthOfLow = period*(1-percentage);
        if(percentage <= 1 && percentage >= -1) {
                MYTIMER_setCompareVal(lengthOfLow);
        }
}

__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{

    //uint32_t time = MYTIMER_getCounterVal();
    uint32_t status = MYTIMER_getInterrupt_status();
    /*
    int diff;
    //uint32_t async_out = MYTIMER_get_async_capture();
    prevClock = curClock;
    curClock = MYTIMER_get_sync_capture();
    lastVals[(arrCount = (arrCount+1)%10)] = curClock;
    //curClock = MYTIMER_getCounterVal();
    diff = curClock - prevClock;
    if(diff < 0)
            diff += period;
        count--;
        if(diff < 1500000)
                printf("%f Inches \n\r", diff*CONVERSION_FACTOR);
        */
    //printf("Interrupt occurred at %lu FABINT \n\r", time);
    //printf("Status = %lu \n\r", status);
        /*
        printf("%d Prev \n\r", prevClock);
        printf("%d Cur \n\r", curClock);
    printf("%d Inches \n\r", diff);
    */
    printf("%d Count \n\r", (int)count);
    if(status & 0x01 && count > 1000)
    {
        setPWM(.5);
        count = 0;
    }
    /*
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
        volatile int d = 0;
        cmpVal = 200000;
        period = 2000000;
        curClock = prevClock = 0;
   /* Setup MYTIMER */
        MYTIMER_init();
        MYTIMER_setOverflowVal(period);
        MYTIMER_setCompareVal(cmpVal);
        setPWM(.25);
        MYTIMER_set_servo_direction(1);
        //MYTIMER_setCompareVal(2034);

        MYTIMER_enable_overflowInt();
        MYTIMER_enable_compareInt();
        MYTIMER_enable_allInterrupts();
        MYTIMER_enable_pwm();
        MYTIMER_enable_capture();

        NVIC_EnableIRQ(Fabric_IRQn);

        MYTIMER_enable();
        printf("HEY\n");
        count = 0;
        int lastVal = 1;
        double speed = 0;
        int dir = 1;

        CONTROLLER_setup_mem();

        /*int x = 1;
        LCD_init();
        while(1) {
        	LCD_printf("Hi! %d", x++);
        	for (d = 0; d < 5000000; d++);
        	LCD_printf("%s %s", "Hello", "World");
        	for (d = 0; d < 5000000; d++);

        }*/

        while( 1 )
        {

				//CONTROLLER_print();
        	CONTROLLER_load();
                if(CONTROLLER->a) {
                        setPWM(0.5);
                }
                else if(CONTROLLER->b) {
                        setPWM(-0.5);
                }
                else {
                        setPWM(0);
                }

                if(CONTROLLER->d_right || CONTROLLER->joystick_x > 158) {
                        MYTIMER_set_servo_direction(1);
                }
                else if(CONTROLLER->d_left || CONTROLLER->joystick_x < 98) {
                        MYTIMER_set_servo_direction(-1);
                }
                else {
                        MYTIMER_set_servo_direction(0);
                }
                //for (d = 0; d < 1000000; d++);
        }
}
