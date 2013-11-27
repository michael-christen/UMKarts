#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "serial.h"
#include "log.h"
#include "xbee.h"
#include "xbee_packet.h"

struct opts {
  const char *log_file_name;
  const char *xbee_file_name;
};

void print_usage(const char * name);

int parse_args(int argc, char **argv, struct opts * myOpts);


int main(int argc, char **argv) {
  struct opts myOpts;
  xbee_serial_t myxbee;
  struct xbee_packet xp;
  int err;
  
  if ((err = parse_args(argc, argv, &myOpts)) < 0) {
    print_usage(argv[0]);
    return -1;
  }

  log_file_open(myOpts.log_file_name);

  if ((err = xbee_ser_open(&myxbee, myOpts.xbee_file_name)) < 0) {
    Log(EERROR, "Unable to open XBee on path %s because %s", myOpts.xbee_file_name, strerror(errno));
    return -2;
  }

  while (1) {
    err = xbee_get_next_packet(&myxbee, &xp);
    if (err == 0) {
      switch (xbee_packet_api_id(&xp)) {
      case XBEE_API_RX:
        xbee_received_packet(&xp);
        break;
      default:
        Log(EINFO, "Received packet that hasn't been implemented %x", xbee_packet_api_id(&xp));
      }
    }
    else {
      Log(EWARNING, "Recevied packet with error");
    }
  }

  return 0;
}

void print_usage(const char * name) {
  printf("%s -l log_file_path path_to_xbee\n"
         "          -l The path to the file you want to log. This is optional\n"
         "             and need not be set. If not set, we log to stdout\n"
         "          path_to_xbee - Should be the path to where the XBee device\n"
         "                         is, probably /dev/ttyUSB0\n",
         name);
}

int parse_args(int argc, char **argv, struct opts * myOpts) {
  int opt;
  myOpts->log_file_name = NULL;
  myOpts->xbee_file_name = NULL;
  while ((opt = getopt(argc, argv, "l:")) != -1) {
    switch (opt) {
    case 'l':
      myOpts->log_file_name = optarg;
      break;
    default:
      return -EINVAL;
    }
  }

  if (optind >= argc) {
    return -EINVAL;
  }

  myOpts->xbee_file_name = argv[optind];
  return 0;
}
