#include "mmio.h"
#include "ecan.h"
#include "ecan_registers.h"
#include "dma_registers.h"
#include <xc.h>

#define CAN1_BASE_ADDRESS 0x400

int dma_setup_rx(struct ecan_adapter *adapter)
{
    // Disable channel and start configuration
    DMA0CON = 0 | (0x2 << 4);

    // Configure channel
	DMA0REQ = 34;
    DMA0CNT = 7;
    DMA0PAD = (uint16_t)(&C1RXD);
    DMA0STAL = (uint16_t)adapter->buffer;
    DMA0STAH = 0;

    // Enable channel 
    DMA0CONbits.CHEN = 1;

	return 0;
}

int dma_setup_tx(struct ecan_adapter *adapter)
{
    // Disable channel and start configuration
    DMA1CON = 0 | (0x2 << 4) | (1 << 13);

    // Configure channel
	DMA1REQ = 70;
    DMA1CNT = 7;
    DMA1PAD = (uint16_t)(&C1TXD);
    DMA1STAL = (uint16_t)adapter->buffer;
    DMA1STAH = 0;

    // Enable channel 
    DMA1CONbits.CHEN = 1;

	return 0;
}

int ecan_write_baud_cfg()
{
	// using 10 time quanta per bit
	// synchronization segment is 1TQ
	// phase segment 2 is 3TQ
	// propagation segment is 3TQ
	// phase segment 1 is 3TQ
	// Fp = 30000000
	// Fcan = 60000000
	// Ftq = 125000 * 10 = 1250000
	// Fbrp = 1250000 * 2 = 2500000
	// Fp / Fbrp = 600/25 = 24
	// BRP = Fp / Fbrp - 1 = 24 - 1 = 23
	
    C1CFG1bits.SJW = 2;
    C1CFG1bits.BRP = 23;
	// PRSEG + 1 = propagation segment bits
    C1CFG2bits.PRSEG = 2;
	// SEG1PH + 1 = phase segment 1 bits
    C1CFG2bits.SEG1PH = 2;
	// SEG2PH + 1 = phase segment 2 bits
    C1CFG2bits.SEG2PH = 2;
	// yes, I want to set phase segment 2
    C1CFG2bits.SEG2PHTS = 1;

	return 0;
}


int ecan_init(struct ecan_adapter *adapter)
{
	// set configuration mode
    C1CTRL1bits.REQOP = 4;

	// wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != 4);

    // Make sure WIN = 0
    C1CTRL1bits.WIN = 0;

    // Select clock source (2*Fp)
    C1CTRL1bits.CANCKS = 1;

    ecan_write_baud_cfg();

    C1FCTRL = 0xC001;

	// set buffer 0 as a transmit buffer
    C1TR01CONbits.TXEN0 = 1;

    // Set normal mode
    C1CTRL1bits.REQOP = 0;

	// wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != 0);

	dma_setup_tx(adapter);
	dma_setup_rx(adapter);
	return 0;
}

/*int ecan_set_filter(struct ecan_adapter *adapter, int n, uint16_t id, int m)
{
	int shift;

	void *bp; // ecan module base pointer

	// local register copies
	uint16_t msksel_reg; // mask selection register
	uint16_t fbp_reg; // filter buffer pointer register
	uint16_t fen_reg; // filter enable register pointer

	// register pointers
	void *sid_reg_ptr;
	void *msksel_reg_ptr;
	void *fbp_reg_ptr;
	void *fen_reg_ptr;

	// find pointer addresses
	bp = (void *)adapter->ecan_base;
	sid_reg_ptr = bp + CiRXFnID + (n << 2);
	msksel_reg_ptr = bp + CiFMSKSELn + ((n >> 3) << 1);
	fbp_reg_ptr = bp + CiBUFPNTn + ((n >> 2) << 1);
	fen_reg_ptr = bp + CiFEN1;

	// set WIN=1
    C1CTRL1bits.WIN = 1;

	// read, modify, write each register
	
	writew(id << CiRXnSID_SID_SHIFT, sid_reg_ptr);	
	writew(0x0, sid_reg_ptr+2);	// zero the EID register

	msksel_reg = readw(msksel_reg_ptr);
	shift = (n & 0x7) << 1;
	msksel_reg = (msksel_reg & ~(0x3U << shift)) | (m << shift);
	writew(msksel_reg, msksel_reg_ptr);

	fbp_reg = readw(fbp_reg_ptr);
	shift = (n & 0x3) << 2;
	// no need to zero the field, since we are always setting it to 0xF for FIFO
	fbp_reg |= (0xF << shift);
	writew(fbp_reg, fbp_reg_ptr);

	fen_reg = readw(fen_reg_ptr);
	fen_reg |= BIT(n);
	writew(fen_reg, fen_reg_ptr);

	// set WIN=0
    C1CTRL1bits.WIN = 0;
	
	return 0;
}*/

/*int ecan_set_mask(struct ecan_adapter *adapter, int m, uint16_t id_mask)
{
	void *bp; // ecan module base pointer

	// register pointer
	void *sid_reg_ptr;

	// find pointer address
	bp = (void*)adapter->ecan_base;
	sid_reg_ptr = bp + CiRXMnID + (m << 2);

	// set WIN=1
    C1CTRL1bits.WIN = 1;

	// write the mask
	writew((id_mask << CiRXnSID_SID_SHIFT) & CiRXnSID_SID_MASK, sid_reg_ptr);	
	writew(0x0, sid_reg_ptr+2);	// zero the EID register

	// set WIN=0
    C1CTRL1bits.WIN = 0;

	return 0;
}*/

int ecan_broadcast(struct ecan_adapter *adapter, struct ecan_message *m)
{
	int i;

	// non-blocking check that buffer 0 is ready
    if (C1TR01CONbits.TXREQ0) {
        return 0;
    }

	// clear the non-data words
	adapter->buffer[0][0] = 0x0;
	adapter->buffer[0][1] = 0x0;
	adapter->buffer[0][2] = 0x0;
	adapter->buffer[0][7] = 0x0;

	// write the identifier
	adapter->buffer[0][0] |= (m->sid << BUFFER_SID_SHIFT) & BUFFER_SID_MASK;
	// write the data length code
	adapter->buffer[0][2] |= m->dlc & 0xF;

	// write the data
	for (i = 0; i < (m->dlc+1)/2; ++i) {
		adapter->buffer[0][i+3] = m->data_words[i];
	}

    // mark buffer for transmission
    C1TR01CONbits.TXREQ0 = 1;

	return 1;
}

int ecan_read(struct ecan_adapter *adapter, struct ecan_message *m)
{
	int i;
	uint16_t fnrb;
	void *bp = (void *)CAN1_BASE_ADDRESS;

    fnrb = C1FIFObits.FNRB;

	if (read_register_bitset(bp + CiRXFULn, fnrb)) {
		// read the identifier
		m->sid = (adapter->buffer[fnrb][0] & BUFFER_SID_MASK) >> BUFFER_SID_SHIFT;
		// read the data length code
		m->dlc = (uint8_t)(adapter->buffer[fnrb][2] & 0xF);

		// read the data
		for (i = 0; (m->dlc+1)/2; ++i) {
			m->data_bytes[i] = adapter->buffer[fnrb][i+3];
		}

		// TODO: handle overflow condition (for logging statistics, if nothing else)
		clear_register_bitset(bp + CiRXFULn, fnrb);

		return 1;
	}
	
	return 0;
}
