#include "xbee/xbee.h"
static uint8_t _xbee_send_buf[MAX_XBEE_PACKET_SIZE];

static enum {
	EXBEE_RECEIVE_START_BYTE,
	EXBEE_RECEIVE_LENGTH_1,
	EXBEE_RECEIVE_LENGTH_2,
	EXBEE_RECEIVE_DATA,
	EXBEE_RECEIVE_CHECKSUM
} _xbee_receive_state = EXBEE_RECEIVE_WAIT;

static struct {
	uint8_t buf[MAX_XBEE_PAYLOAD_SIZE];
	uint16_t size;
	uint16_t _cur;
	uint8_t _checksum;
	uint8_t _escape;
} _xbee_receive_data;

int _xbee_send(uint8_t * data, uint16_t len); 
uint8_t * _xbee_add_byte(uint8_t * ptr, uint8_t data, uint8_t escape); 

static int _xbee_send(uint8_t * data, uint16_t len) {
	uint16_t i;
	uint16_t final_len = 0;
	uint8_t checksum = 0;
	uint8_t *buf_ptr = _xbee_send_buf;
	if (MSS_UART_tx_complete(&g_mss_uart1) != 0) {
		return -EXBEEINUSE;
	}
	buf_ptr = _xbee_add_byte(buf_ptr, XBEE_START_BYTE, 0, &final_len);
	buf_ptr = _xbee_add_byte(buf_ptr, (uint8_t) ((len >> 8) & 0x00FF), 1, &final_len);
	buf_ptr = _xbee_add_byte(buf_ptr, (uint8_t) (len & 0x00FF), 1, &final_len);
	for (i = 0; i < len; i++) {
		checksum += data[i];
		buf_ptr = _xbee_add_byte(buf_ptr, data[i], 1, &final_len);
	}
	buf_ptr = _xbee_add_byte(buf_ptr, 0xFF - checksum, 1, &final_len);
	MSS_UART_irq_tx(&g_mss_uart1, _xbee_send_buf, final_len);
	return 0;
}

static int _xbee_receive(uint8_t data) {
	switch (_xbee_receive_state) {
	case EXBEE_RECEIVE_START_BYTE:
		if (data == XBEE_START_BYTE) {
			_xbee_receive_state = EXBEE_RECEIVE_LENGTH_1;
			memset(&_xbee_receive_data, 0, sizeof(_xbee_receive_data));
		}
		break;
	case EXBEE_RECEIVE_LENGTH_1:
		_xbee_receive_data.size = (data << 8) 0xFF00;
		_xbee_receive_state = EXBEE_RECEIVE_LENGTH_2;
		break;
	case EXBEE_RECEIVE_LENGTH_2:
		_xbee_receive_data.size += data;
		_xbee_receive_state = EXBEE_RECEIVE_DATA;
		break;
	case EXBEE_RECEIVE_DATA:
		_xbee_receive_data.buf[_xbee_receive_data._cur++] = data;
	
		break;
	case EXBEE_RECEIVE_CHECKSUM:
	
		break;
	default:
		// Should never make it here
		return -EXBEEUNDEFINEDSTATE;
	}
	return 0;
}

static uint8_t * _xbee_add_byte(uint8_t * ptr, uint8_t data, uint8_t escape, uint16_t * len) {
	if (escape && (
	  data  ==  XBEE_START_BYTE   ||
	  data  ==  XBEE_ESCAPE_BYTE  ||
	  data  ==  XBEE_XON_BYTE     ||
	  data  ==  XBEE_XOFF_BYTE))  {
		*ptr++ = XBEE_ESCAPE_BYTE;
		*prt++ = data ^ XBEE_XOR_BYTE;
		if (len) (*len) += 2;
	}
	else {
		*ptr++ = data;
		(*len) += 1;
	}
	return ptr;
}
