/*
 * ir_led.h
 */
#ifndef IR_LED_H_
#define IR_LED_H_

#include <stdint.h>

/*
 * Must be called before other IR commands
 * Initializes the LED Recv GPIO pins
 *
 * REQUIRES: MSS_GPIO_INIT() has already been called
 */
void IR_LED_init();

/*
 * Sends out a byte on the IR LED using Manchester Encoding
 *
 * @param uint8_t msg The byte to send
 * @return uint8_t 1 if message was sent sucessfully, 0 otherwise
 *
 * Will return 0 if the send buffer is full
 */
uint8_t IR_LED_write(uint8_t msg);

/*
 * Reads back the byte recieved from the LED Reciever
 * @return uint8_t byte receieved
 */
uint8_t IR_LED_read();

/*
 * Disables the recv interrupt for a set amount of time
 * @param int millsecs number of milliseconds to be disabled
                       if 0, the indefinintely
 */
void IR_LED_recv_disable(int millisecs);

/*
 * Enables the recv interrupt
 */
void IR_LED_recv_enable();

#endif /* IR_LED_H_ */
