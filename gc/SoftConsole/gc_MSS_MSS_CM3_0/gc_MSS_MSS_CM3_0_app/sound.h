/*
 * sound.h
 *
 *  Created on: Nov 20, 2013
 *      Author: knekritz
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <stdint.h>

void flash_write();

void sound_init();

void sound_play(uint32_t begin, uint32_t end);

void sound_play_repeat(uint32_t begin, uint32_t end);

void sound_stop();

#endif /* SOUND_H_ */
