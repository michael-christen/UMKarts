#include "xbee_interface.h"
#include "lib/simple_allocator.h"

/* Allocator */
static struct {
  struct xbee_packet mem[XBEE_PACKET_BUF_SIZE];
  SimpleAllocator alloc;
} _xbee_alloc;

int _xbee_interface_alloc_init() {
  int check
  check = SimpleAllocatorInit(&(_xbee_alloc.alloc),
                              _xbee_alloc.mem,
                              XBEE_PACKET_BUF_SIZE,
                              sizeof(struct xbee_packet));
  return check;
}

struct xbee_packet * xbee_interface_create_packet() {
  return SimpleAllocatorAlloc(&(_xbee_alloc.alloc));
}

void xbee_interface_free_packet(struct xbee_packet *xp) {
  SimpleAllocatorFree(&(_xbee_alloc.alloc), xp);
}
