#include "xbee_reader.h"
#include "log.h"
#include "xbee_packet.h"
#include <stdlib.h>

static int _xbee_printf(uint8_t * data, uint16_t len) {
  int i;
  char * str = malloc((len + 1) * sizeof(char));
  for (i = 0; i < len; i++) str[i] = data[i];
  str[i] = '\0';
  printf("%s\n", str);
  free(str);
  return 0;
}

int xbee_get_next_packet(xbee_serial_t *serial, struct xbee_packet * xp) {
  struct xbee_reader xr;
  uint8_t ch;
  XBeeReaderInit(&xr, xp);
  while (!XBeeReaderDone(&xr)) {
    ch = xbee_ser_getchar(serial);
    if (ch >= 0) {
      XBeeReaderRead(&xr, &ch, &ch + 1);
    }
    else if (ch != -ENOSPC) {
      Log(EERROR, "Unable to read packet because %s", strerror(errno));
    }
  }

  if (XBeeReaderGood(&xr)) {
    return 0;
  }
  else {
    return -1;
  }
}

int xbee_received_packet(struct xbee_packet *xp) {
  uint8_t * data;
  uint16_t data_len;
  data_len = xbee_rxpt_get_payload_size(xp);
  if (data_len > 0) {
    data = xbee_rxpt_payload_start(xp);
    switch (*data) {
      case 0x00:
        _xbee_printf(data + 1, data_len - 1);
        break;
      default:
        Log(EINFO, "Recevied a tx packet that we have not configured for %x", *data);
    }
  }
  return 0;
}

