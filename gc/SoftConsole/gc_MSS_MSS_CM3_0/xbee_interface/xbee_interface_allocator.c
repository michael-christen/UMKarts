#include "xbee_interface.h"
#include "simple_allocator.h"

/* Allocator */
static struct {
	struct xbee_packet mem[XBEE_PACKET_BUF_SIZE];
	SimpleAllocator alloc;
} _xbee_alloc_tx;

static struct {
	struct xbee_packet_received mem[XBEE_PACKET_BUF_SIZE];
	SimpleAllocator alloc;
} _xbee_alloc_rx;

int _xbee_interface_alloc_init() {
	int check;
	check = SimpleAllocatorInit(&(_xbee_alloc_tx.alloc), _xbee_alloc_tx.mem,
			XBEE_PACKET_BUF_SIZE, sizeof(struct xbee_packet));
	if (check != 0) {
		return check;
	}
	check = SimpleAllocatorInit(&(_xbee_alloc_rx.alloc), _xbee_alloc_rx.mem,
			XBEE_PACKET_BUF_SIZE, sizeof(struct xbee_packet_received));
	return check;
}

struct xbee_packet * xbee_interface_create_packet() {
	return SimpleAllocatorAlloc(&(_xbee_alloc_tx.alloc));
}

void xbee_interface_free_packet(const struct xbee_packet *xp) {
	SimpleAllocatorFree(&(_xbee_alloc_tx.alloc), xp);
}

struct xbee_packet_received * xbee_interface_create_receive_packet() {
	return SimpleAllocatorAlloc(&(_xbee_alloc_rx.alloc));
}

void xbee_interface_free_packet_received(const struct xbee_packet_received *xpr) {
	SimpleAllocatorFree(&(_xbee_alloc_rx.alloc), xpr);
}
