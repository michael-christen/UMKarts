/*
 * controller.c
 *
 *  Created on: Nov 6, 2013
 *      Author: mikechri
 */
#include "controller.h"

controller_t * CONTROLLER;
uint32_t MEM[8];

void CONTROLLER_print() {
        CONTROLLER_load();
        if (CONTROLLER->start)
                printf("Start ");
        if (CONTROLLER->y)
                printf("Y ");
        if (CONTROLLER->x)
                printf("X ");
        if (CONTROLLER->b)
                printf("B ");
        if (CONTROLLER->a)
                printf("A ");
        if (CONTROLLER->l)
                printf("L ");
        if (CONTROLLER->r)
                printf("R ");
        if (CONTROLLER->z)
                printf("Z ");
        if (CONTROLLER->d_up)
                printf("UP ");
        if (CONTROLLER->d_down)
                printf("DOWN ");
        if (CONTROLLER->d_right)
                printf("RIGHT ");
        if (CONTROLLER->d_left)
                printf("LEFT ");
        printf("\n\r");
        printf("Joystick X: %d, Y: %d\n\r", CONTROLLER->joystick_x, CONTROLLER->joystick_y);
        printf("C stick X: %d, Y: %d\n\r", CONTROLLER->cstick_x, CONTROLLER->cstick_y);
        printf("Trigger Pressure Left: %d, Right: %d\n\r\n\r", CONTROLLER->left, CONTROLLER->right);
}

// Not actually necessary, will happen automatically in hardware
void CONTROLLER_init() {
        // Write to send wavebird init command
        *((volatile int *) CONTROLLER_BASE) = 0;
}

void CONTROLLER_setup_mem() {
    CONTROLLER = (controller_t *) MEM;
}

void CONTROLLER_load() {
    MEM[0] = (volatile int) *((volatile int *) CONTROLLER_BASE);
    MEM[1] = (volatile int) *((volatile int *) (CONTROLLER_BASE + 4));
}
