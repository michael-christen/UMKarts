/*
 * controller.h
 *
 *  Created on: Nov 11, 2013
 *      Author: mikechri
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "CMSIS/a2fxxxm3.h"
#include <stdio.h>

#define CONTROLLER_BASE (FPGA_FABRIC_BASE)

typedef struct
{
	uint8_t right : 8;
	uint8_t left : 8;

	uint8_t cstick_y : 8;
	uint8_t cstick_x : 8;

	uint8_t joystick_y : 8;
	uint8_t joystick_x : 8;
	_Bool empty3 : 1;

	_Bool d_left : 1;
	_Bool d_right : 1;
	_Bool d_down : 1;
	_Bool d_up : 1;
	_Bool z : 1;
	_Bool r : 1;
	_Bool l : 1;
	_Bool empty5 : 1;


	_Bool a : 1;
	_Bool b : 1;
	_Bool x : 1;
	_Bool y : 1;
	_Bool start : 1;

	_Bool empty2 : 1;
	_Bool empty1 : 1;


} controller_t;

controller_t * CONTROLLER;
uint32_t *MEM;

void CONTROLLER_print();
void CONTROLLER_init();
void CONTROLLER_load();
void CONTROLLER_setup_mem();
#endif /* CONTROLLER_H_ */
