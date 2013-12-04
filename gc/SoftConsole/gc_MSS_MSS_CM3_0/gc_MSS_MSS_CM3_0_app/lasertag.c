#include "ir_led.h"
#include "lasertag.h"
#include "player.h"

const uint8_t KEY = 0xE;

void LASER_TAG_init() {
  IR_LED_init();
}

bool LASER_TAG_shoot() {
  return IR_LED_write((KEY << 4) + DRIVER);
}

uint8_t LASER_TAG_hit() {
  uint8_t data = IR_LED_read();
  if (data >> 4 != KEY) return false;
  return data & 0x0F;
}
