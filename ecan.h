#ifndef ECAN_H
#define ECAN_H

#include <stdint.h>

#define ECAN_NUM_BUFFERS 8
#define ECAN_DECLARE_BUFFER(name) uint16_t name[ECAN_NUM_BUFFERS][8] \
		__attribute__((aligned(ECAN_NUM_BUFFERS * 16)));

struct ecan_message {
	uint16_t sid;
	union {
		uint8_t data_bytes[8];
		uint16_t data_words[4];
	};
	uint8_t dlc;
};

struct ecan_adapter {
	// this is a bit bizzare, but it allows us to 
	// pass a pointer and still use 2d array syntax
	uint16_t (*buffer)[8];
};

int ecan_init(struct ecan_adapter *adapter);

int ecan_write_baud_cfg();

int ecan_set_filter(struct ecan_adapter *adapter, int n, uint16_t id, int m);

int ecan_set_mask(struct ecan_adapter *adapter, int m, uint16_t id_mask);

// ecan_broadcast - non-blocking CAN message transmit.
// returns the number of messages transmitted (0 or 1)
int ecan_broadcast(struct ecan_adapter *adapter, struct ecan_message *m);

// ecan_read - non-blocking CAN message receive
// returns the number of messages received (0 or 1)
int ecan_read(struct ecan_adapter *adapter, struct ecan_message *m);

#endif // ECAN_H
