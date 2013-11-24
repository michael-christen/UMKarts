/*
 * ir_led.h
 */
#ifndef IR_LED_H_
#define IR_LED_H_

#include <stdint.h>
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

#endif /* IR_LED_H_ */
