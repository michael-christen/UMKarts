#ifndef _XBEE_SERIAL_H_
#define _XBEE_SERIAL_H_

#include <inttypes.h>

typedef struct {
  int fd;
} xbee_serial_t;

enum xbee_serial_baud {
  EXBEE_SERIAL_9600 = 9600,
  EXBEE_SERIAL_19200 = 19200,
  EXBEE_SERIAL_38400 = 38400,
  EXBEE_SERIAL_57600 = 57600,
  EXBEE_SERIAL_115200 = 115200,
};

int xbee_ser_baudrate(xbee_serial_t * serial, enum xbee_serial_baud rate); 
int xbee_ser_open(xbee_serial_t * serial, const char * path); 
int xbee_ser_close(xbee_serial_t *serial); 
int xbee_ser_write(xbee_serial_t * serial, const void * buffer, int length); 
int xbee_ser_read(xbee_serial_t * serial, void * buffer, int bufsize); 
int xbee_ser_getchar(xbee_serial_t *serial);
int xbee_ser_putchar(xbee_serial_t *serial, uint8_t ch); 

#endif /* _XBEE_SERIAL_H_ */
