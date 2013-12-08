#include "zeromq.h"
#include <string.h>
#include <stdlib.h>

static struct {
	void *context;
	void *socket;
	int rc;
} _zero_mq;

static void _listener_zero_mq_destroy(); 

int listener_zero_mq_init(const char * host) {
	atexit(_listener_zero_mq_destroy);
	_zero_mq.context = zmq_ctx_new();
	_zero_mq.socket = zmq_socket(_zero_mq.context, ZMQ_PUB);
	_zero_mq.rc = zmq_bind(_zero_mq.socket, host);
	return zero_mq.rc;
}

int listener_zero_mq_send(const uint8_t *data, size_t len) {
	zmq_msg_t msg;
	int err;
	err = zmq_msg_init_size(&msg, len);
	if (err != 0) {
		return err;
	}

	memcpy(zmq_msg_data(&msg), data, len);

	err = zmq_send(_zero_mq.socket, &msg, 0);

	/* No error checking, because really can't fail */
	zmq_msg_close(&msg);
	return err;
}

static void _listener_zero_mq_destroy() {
	zmq_close(_zero_mq.socket);
	zmq_ctx_destroy(_zero_mq.context);
}
