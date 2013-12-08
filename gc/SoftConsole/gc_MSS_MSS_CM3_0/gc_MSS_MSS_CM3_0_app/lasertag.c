#include "ir_led.h"
#include "lasertag.h"
#include "player.h"

#include "drivers/mss_gpio/mss_gpio.h"

const uint8_t KEY = 0xE;

void LASER_TAG_init() {
  IR_LED_init();
  // Hit LED (initialize)
  MSS_GPIO_config(MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
  // Is active low
  MSS_GPIO_set_output(MSS_GPIO_3, 1);
}

bool LASER_TAG_shoot() {
  return IR_LED_write((KEY << 4) + DRIVER);
}

uint8_t LASER_TAG_hit() {
  uint8_t data = IR_LED_read();
  if (data >> 4 != KEY) return 0;
  return data & 0x0F;
}

//REQ: on is either a 0 or a 1
//EFF: turns Hit LED on or off
void LASER_TAG_set_hit_LED(uint8_t on) {
	MSS_GPIO_set_output(MSS_GPIO_3, !on);
}
