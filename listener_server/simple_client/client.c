//  Hello World client
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

char exitnow;

void signal_handler(int signum) {
	exitnow = 1;
}

int main (void)
{
	signal(SIGINT, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGABRT, signal_handler);
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_SUB);
    zmq_connect(requester, "tcp://localhost:6666");
	zmq_setsockopt(requester, ZMQ_SUBSCRIBE, 0x0, 0);

	int i;
	int data_len;
	exitnow = 0;
	while (!exitnow) {
        char buffer [100];
        data_len = zmq_recv (requester, buffer, 100, 0);
		printf("Received packet: ");
		for (i = 0; i < data_len; i++) {
			printf("0x%x ", buffer[i]);
		}
		printf("\n");
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
