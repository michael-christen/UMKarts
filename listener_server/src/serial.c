#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include "serial.h"

int xbee_ser_baudrate(xbee_serial_t * serial, enum xbee_serial_baud rate) {
  struct termios options;

  if (tcgetattr(serial->fd, &options) == -1) {
    return -errno;
  }

  cfsetispeed(&options, rate);
  cfsetospeed(&options, rate);

  options.c_cflag |= (CLOCAL | CREAD);
  
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  if (tcsetattr(serial->fd, TCSADRAIN, &options) == -1) {
    return -errno;
  }

  return 0;
}

int xbee_ser_open(xbee_serial_t * serial, const char * path) {
  int err;
  serial->fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);

  if (serial->fd < 0) {
    return -errno;
  }

  /* Don't block on reads ! */
  fcntl(serial->fd, F_SETFL, FNDELAY);
  if ((err = xbee_ser_baudrate(serial, EXBEE_SERIAL_9600)) < 0) {
    return err;
  }
  return 0;
}

int xbee_ser_write(xbee_serial_t * serial, const void * buffer, int length) {
  int result;

  if (length < 0) {
    return -EINVAL;
  }

  result = write(serial->fd, buffer, length);

  if (result < 0) {
    return -errno;
  }

  return result;
}

int xbee_ser_read(xbee_serial_t * serial, void * buffer, int bufsize) {
  int result;

  if (!buffer || bufsize < 0) {
    return -EINVAL;
  }

  result = read(serial->fd, buffer, bufsize);
  if (result == -1) {
    if (errno == EAGAIN) {
      return 0;
    }
    return -errno;
  }
  return result;
}

int xbee_ser_getchar(xbee_serial_t *serial) {
  uint8_t ch = 0;
  int retval;

  retval = xbee_ser_read(serial, &ch, 1);
  if (retval != 1) {
    return retval ? retval : -ENODATA;
  }

  return ch;
}

int xbee_ser_putchar(xbee_serial_t *serial, uint8_t ch) {
  int retval;

  retval = xbee_ser_write(serial, &ch, 1);
  if (retval == 1) {
    return 0;
  }
  else if (retval == 0) {
    return -ENOSPC;
  }
  else {
    return retval;
  }
}

int xbee_ser_close(xbee_serial_t *serial) {
  int result = 0;

  if (close(serial->fd) == -1) {
    result = -errno;
  }
  serial->fd = -1;
  return result;
}
