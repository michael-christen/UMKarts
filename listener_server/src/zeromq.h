#ifndef __ZERO_MH_LISTENER_H_
#define __ZERO_MH_LISTENER_H_

#include <zmq.h>
#include <inttypes.h>

int listener_zero_mq_init(const char * host); 
int listener_zero_mq_send(const uint8_t *data, size_t len); 
#endif /* __ZERO_MH_LISTENER_H_ */
