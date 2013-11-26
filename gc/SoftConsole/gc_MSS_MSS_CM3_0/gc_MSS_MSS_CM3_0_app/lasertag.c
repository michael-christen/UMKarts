#include "ir_led.h"
#include "lasertag.h"

const uint8_t KEY = 0xE;

void LASER_TAG_init() {
  IR_LED_init();
}

// @TODO: Set car ID as shooting
bool LASER_TAG_shoot() {
  return IR_LED_write((KEY << 4) + 0x0);
}

uint8_t LASER_TAG_hit() {
  uint8_t data = IR_LED_read();
  if (data >> 4 != KEY) return false;
  return data & 0x0F;
}
