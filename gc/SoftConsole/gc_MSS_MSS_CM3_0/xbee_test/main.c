#include "xbee_interface.h"
#include "xbee.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "mss_uart.h"

uint8_t message[] = {0x10, /* API ID */
		             0x01, /* Frame ID */
		             0x00, 0x13, 0xA2, 0x00, 0x40, 0xA7, 0x11, 0xE0, /* Dest Address */
		             0xFF, 0xFE, /* Reserved */
		             0x00, /* Broadcast radius */
		             0x00, /* Transmit Options */
		             0x48, 0x45, 0x4C, 0x4C, 0x4F}; /* RF Data */

int main() {
	int err;
	volatile int i;
	uint8_t modem_status, receive_status;
	struct xbee_packet * myPacket;
	struct xbee_packet_received * receivedPacket;
	err = xbee_interface_init();
	if (err != 0) {
		//printf("Unable to successfully initialize xbee interface. Error: %d\r\n", err);
		return 0;
	}

	while (1) {
		modem_status = MSS_UART_get_modem_status(&g_mss_uart1);
		receive_status = MSS_UART_get_rx_status(&g_mss_uart1);
		while (!(myPacket = xbee_interface_create_packet())) {
			//printf("Unable to create a new packet\r\n");
		}
		memcpy(myPacket->payload, message, sizeof(message));
		myPacket->len = sizeof(message)/sizeof(uint8_t);
		err = xbee_send(myPacket);
		if (err != 0) {
			//printf("Error sending packet: %d\r\n", err);
		}
		else {
			//printf("Packet sent!\n\r");
		}
		while (!(receivedPacket = xbee_read())) ;
		xbee_interface_free_packet_received(receivedPacket);
		while ((receivedPacket = xbee_read())) {
			//printf("Received packet: It's delivery status is: %x\r\n", receivedPacket->payload[5]);
			xbee_interface_free_packet_received(receivedPacket);
		}
		for (i = 0; i < 1000; i++) ;
	}



}
