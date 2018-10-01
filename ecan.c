#include "ecan.h"
#include <xc.h>

int dma_setup_rx(struct ecan_adapter *adapter)
{
	void *bp = adapter->dma_base + 16 * adapter->dma_rx_channel;
	uint16_t dmaicon = 0;
	uint16_t dmaireq = 0;
	uint16_t dmaicnt = 7;
	uint16_t dmaipad = (uint16_t)(adapter->ecan_base + CiRXD);
	uint16_t dmaistal = (uint16_t)adapter->buffer;
	uint16_t dmaistah = 0;

	writew(dmaicon, bp + DMAiCON);

	dmaicon |=  DMAiCON_CHEN_MASK;
	dmaicon |= 0x2 << DMAiCON_AMODE_SHIFT;
	dmaireq |= adapter->rx_irq;

	writew(dmaicnt, bp + DMAiCNT);
	writew(dmaipad, bp + DMAiPAD);
	writew(dmaistal, bp + DMAiSTAL);
	writew(dmaistah, bp + DMAiSTAH);
	writew(dmaireq, bp + DMAiREQ);
	writew(dmaicon, bp + DMAiCON);

	return 0;
}

int dma_setup_tx(struct ecan_adapter *adapter)
{
	void *bp = adapter->dma_base + 16 * adapter->dma_tx_channel;
	uint16_t dmaicon = 0;
	uint16_t dmaireq = 0;
	uint16_t dmaicnt = 7;
	uint16_t dmaipad = (uint16_t)(adapter->ecan_base + CiTXD);
	uint16_t dmaistal = (uint16_t)adapter->buffer;
	uint16_t dmaistah = 0;

	writew(dmaicon, bp + DMAiCON);

	dmaicon |= DMAiCON_DIR_MASK | DMAiCON_CHEN_MASK;
	dmaicon |= 0x2 << DMAiCON_AMODE_SHIFT;
	dmaireq |= adapter->tx_irq;

	writew(dmaicnt, bp + DMAiCNT);
	writew(dmaipad, bp + DMAiPAD);
	writew(dmaistal, bp + DMAiSTAL);
	writew(dmaistah, bp + DMAiSTAH);
	writew(dmaireq, bp + DMAiREQ);
	writew(dmaicon, bp + DMAiCON);

	return 0;
}

int ecan_init(struct ecan_adapter *adapter, struct ecan_baud_cfg *cicfg)
{
	void *bp = adapter->ecan_base;
	
	uint16_t cictrl1;
	uint16_t cifctrl = 0xC01F; // just for testing
	uint16_t citrmncon = 0;

	// set configuration mode
	cictrl1 = readw(bp + CiCTRL1);
	cictrl1 &= ~CiCTRL1_REQOP_MASK;
	cictrl1 |= 4 << CiCTRL1_REQOP_SHIFT;
	writew(cictrl1, bp + CiCTRL1);

	// wait for the mode change to take effect
	while (((readw(bp + CiCTRL1) & CiCTRL1_OPMODE_MASK) >> CiCTRL1_OPMODE_SHIFT) != 4);

	// make sure WIN=0
	cictrl1 = readw(bp + CiCTRL1);
	cictrl1 &= ~CiCTRL1_WIN_MASK;

	// select clock source (when at 30mips)
	cictrl1 |= CiCTRL1_CANCKS_MASK;
	writew(cictrl1, bp + CiCTRL1);

	// write the baud rate configuration
	writew(cicfg->cicfg1, bp + CiCFG1);
	writew(cicfg->cicfg2, bp + CiCFG2);

	writew(cifctrl, bp + CiFCTRL);

	// set buffer 0 as a transmit buffer
	citrmncon |= CiTRmnCON_TXEN_MASK;
	writew(citrmncon, bp + CiTRmnCON);

	// set normal mode
	cictrl1 = readw(bp + CiCTRL1);
	cictrl1 &= ~CiCTRL1_REQOP_MASK;
	//cictrl1 |= 2 << CiCTRL1_REQOP_SHIFT; // set loopback for testing
	writew(cictrl1, bp + CiCTRL1);

	// wait for the mode change to take effect
	while (((readw(bp + CiCTRL1) & CiCTRL1_OPMODE_MASK) >> CiCTRL1_OPMODE_SHIFT) != 0);
	//while (((readw(bp + CiCTRL1) & CiCTRL1_OPMODE_MASK) >> CiCTRL1_OPMODE_SHIFT) != 2);

	dma_setup_tx(adapter);
	dma_setup_rx(adapter);
	return 0;
}

int ecan_write_baud_cfg(struct ecan_baud_cfg *cicfg)
{
	uint16_t cicfg1 = 0;
	uint16_t cicfg2 = 0;
	
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
	
	cicfg1 |= 2 << CiCFG1_SJW_SHIFT;
	cicfg1 |= (23 << CiCFG1_BRP_SHIFT) & CiCFG1_BRP_MASK;
	// PRSEG + 1 = propagation segment bits
	cicfg2 |= 2 << CiCFG2_PRSEG_SHIFT;
	// SEG1PH + 1 = phase segment 1 bits
	cicfg2 |= 2 << CiCFG2_SEG1PH_SHIFT;
	// SEG2PH + 1 = phase segment 2 bits
	cicfg2 |= 2 << CiCFG2_SEG2PH_SHIFT;
	// yes, I want to set phase segment 2
	cicfg2 |= CiCFG2_SEG2PHTS_MASK;

	cicfg->cicfg1 = cicfg1;
	cicfg->cicfg2 = cicfg2;

	return 0;
}

int ecan_set_filter(struct ecan_adapter *adapter, int n, uint16_t id, int m)
{
	int shift;

	void *bp; // ecan module base pointer

	// local register copies
	uint16_t sid_reg; // filter identifier register
	uint16_t msksel_reg; // mask selection register
	uint16_t fbp_reg; // filter buffer pointer register
	uint16_t fen_reg; // filter enable register pointer
	uint16_t ctrl; // can control register

	// register pointers
	void *sid_reg_ptr;
	void *msksel_reg_ptr;
	void *fbp_reg_ptr;
	void *fen_reg_ptr;

	// find pointer addresses
	bp = adapter->ecan_base;
	sid_reg_ptr = bp + CiRXFnID + (n << 2);
	msksel_reg_ptr = bp + CiFMSKSELn + ((n >> 3) << 1);
	fbp_reg_ptr = bp + CiBUFPNTn + ((n >> 2) << 1);
	fen_reg_ptr = bp + CiFEN1;

	// set WIN=1
	ctrl = readw(bp + CiCTRL1);
	ctrl |= CiCTRL1_WIN_MASK;
	writew(ctrl, bp + CiCTRL1);

	// read, modify, write each register
	
	writew(id << CiRXnSID_SID_SHIFT, sid_reg_ptr);	
	writew(0x0, sid_reg_ptr+2);	// zero the EID register

	msksel_reg = readw(msksel_reg_ptr);
	shift = (n & 0x7) << 1;
	msksel_reg = (msksel_reg & ~(0x3U << shift)) | (m << shift);
	writew(msksel_reg, msksel_reg_ptr);

	fbp_reg = readw(fbp_reg_ptr);
	shift = (n & 0x3) << 2;
	// no need to zero the field, since we are always setting it to 0xF
	fbp_reg |= (0xF << shift);
	writew(fbp_reg, fbp_reg_ptr);

	fen_reg = readw(fen_reg_ptr);
	fen_reg |= BIT(n);
	writew(fen_reg, fen_reg_ptr);

	// set WIN=0
	ctrl = readw(bp + CiCTRL1);
	ctrl &= ~CiCTRL1_WIN_MASK;
	writew(ctrl, bp + CiCTRL1);
	
	return 0;
}

int ecan_set_mask(struct ecan_adapter *adapter, int m, uint16_t id_mask)
{
	void *bp; // ecan module base pointer

	// register pointer
	void *sid_reg_ptr;

	uint16_t ctrl;

	// find pointer address
	bp = adapter->ecan_base;
	sid_reg_ptr = bp + CiRXMnID + (m << 2);

	// set WIN=1
	ctrl = readw(bp + CiCTRL1);
	ctrl |= CiCTRL1_WIN_MASK;
	writew(ctrl, bp + CiCTRL1);

	// write the mask
	writew(id_mask << CiRXnSID_SID_SHIFT, sid_reg_ptr);	
	writew(0x0, sid_reg_ptr+2);	// zero the EID register

	// set WIN=0
	ctrl = readw(bp + CiCTRL1);
	ctrl &= ~CiCTRL1_WIN_MASK;
	writew(ctrl, bp + CiCTRL1);

	return 0;
}

int ecan_broadcast(struct ecan_adapter *adapter, struct ecan_message *m)
{
	int i;
	uint16_t citr01con;
	void *bp = adapter->ecan_base;

	// non-blocking check that the buffer is ready
	if (readw(bp + CiTRmnCON) & CiTRmnCON_TXREQ_MASK) {
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
	adapter->buffer[0][2] |= m->length & 0xF;

	// write the data
	for (i = 0; i < m->length; ++i) {
		adapter->buffer[0][i + 3] = m->data[i];
	}

	citr01con = readw(bp + CiTRmnCON);
	citr01con |= CiTRmnCON_TXREQ_MASK;
	writew(citr01con, bp + CiTRmnCON);

	return 1;
}


int ecan_read(struct ecan_adapter *adapter, struct ecan_message *m)
{
	int i;
	uint16_t cififo;
	uint16_t fnrb;
	void *bp = adapter->ecan_base;

	cififo = readw(bp + CiFIFO);
	fnrb = (cififo & CiFIFO_FNRB_MASK) >> CiFIFO_FNRB_SHIFT;
	if (read_register_bitset(bp + CiRXFULn, fnrb)) {
		// read the identifier
		m->sid = (adapter->buffer[0][0] & BUFFER_SID_MASK) >> BUFFER_SID_SHIFT;
		// read the data length code
		m->length = (uint8_t)(adapter->buffer[0][2] & 0xF);

		// read the data
		for (i = 0; i < m->length; ++i) {
			m->data[i] = adapter->buffer[0][i + 3];
		}

		// TODO: handle overflow condition (for logging statistics, if nothing else)
		clear_register_bitset(bp + CiRXFULn, fnrb);

		return 1;
	}

	return 0;
}
