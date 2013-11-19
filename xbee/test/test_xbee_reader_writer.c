#include "xbee.h"
#include "xbee_writer.h"
#include "xbee_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t payload1[] = {0x23, 0x11}; /* Simple test found in digi documentation */
static uint8_t payload2[] = {0x7D, 0x7E, 0x11, 0x13}; /* Test escaping of data */
static uint8_t payload3[] = {0x7D}; /* Test to make sure escaping of checksum works */
static uint8_t payload4[] = {0x1, 0x1, 0x11, 0x00, 0x00};

void test_payload(uint8_t * payload, uint16_t len); 

int main(int argc, char **argv) {
  test_payload(payload1, sizeof(payload1) / sizeof(uint8_t));
  test_payload(payload2, sizeof(payload2) / sizeof(uint8_t));
  test_payload(payload3, sizeof(payload3) / sizeof(uint8_t));
  test_payload(payload4, sizeof(payload4) / sizeof(uint8_t));
  return 0;
}
  
void test_payload(uint8_t * payload, uint16_t len) {
  uint16_t i, j;
  uint8_t * end_w, *end_r;
  struct xbee_packet xp_w;
  struct xbee_packet xp_r;
  struct xbee_writer xw;
  struct xbee_reader xr;
  uint8_t buffer[2];
  xp_w.len = len;
  for (i = 0; i < len; i++) {
    xp_w.payload[i] = payload[i];
  }
  XBeeWriterInit(&xw, &xp_w);
  XBeeReaderInit(&xr, &xp_r);
  for (i = 0; i < len; i++) {
    printf("0x%X ", payload[i]);
  }
  printf("\n");
  while (!XBeeWriterDone(&xw)) {
    end_w = buffer + sizeof(buffer) / sizeof(uint8_t);
    end_w = XBeeWriterWrite(&xw, buffer, end_w);
    for (i = 0; i < (end_w - buffer); i++) {
      printf("0x%X ", buffer[i]);
    }
    end_r = end_w;
    end_r = XBeeReaderRead(&xr, buffer, end_r);
  }
  printf("\n");
  if (!XBeeReaderDone(&xr)) {
    printf("ERROR: Reader should have read a complete packet, but it didn't\n");
    return;
  }
  if (!XBeeReaderGood(&xr)) {
    if (xr.state == XBEE_READ_DONE_BAD_CHECKSUM) {
      printf("ERROR: Packet checksum failed\n");
    }
    else if (xr.state == XBEE_READ_DONE_UNEXPECTED_START_BYTE) {
      printf("ERROR: Packet had unescaped start byte\n");
    }
    else {
      printf("ERROR: Packet marked as bad, but we're not sure why\n");
    }
    return;
  }
  for (i = 0; i < xp_w.len; i++) {
    if (xp_w.payload[i] != xp_r.payload[i]) {
      printf("ERROR: Packets differed!! Packets below\n");
      printf("W Packet\t");
      for (j = 0; j < xp_w.len; j++) printf("0x%X ", xp_w.payload[j]);
      printf("\nR Packet\t");
      for (j = 0; j < xp_r.len; j++) printf("0x%X ", xp_r.payload[j]);
      printf("\n");
      return;
    }
  }
  printf("SUCCESS\n");
}
