#include <stdint.h>

#define BIT(bitshift) (1UL << (bitshift))

#define CiCTRL1 0x00
enum {
	CiCTRL1_WIN_SHIFT = 0,
	CiCTRL1_CANCAP_SHIFT = 3,
	CiCTRL1_OPMODE_SHIFT = 5,
	CiCTRL1_REQOP_SHIFT = 8,
	CiCTRL1_CANCKS_SHIFT = 11,
	CiCTRL1_ABAT_SHIFT = 12,
	CiCTRL1_CSIDL_SHIFT = 13
};
#define CiCTRL1_WIN_MASK    BIT(CiCTRL1_WIN_SHIFT)
#define CiCTRL1_CANCAP_MASK BIT(CiCTRL1_CANCAP_SHIFT)
#define CiCTRL1_OPMODE_MASK (0x7UL << CiCTRL1_OPMODE_SHIFT)
#define CiCTRL1_REQOP_MASK  (0x7UL << CiCTRL1_REQOP_SHIFT)
#define CiCTRL1_CANCKS_MASK BIT(CiCTRL1_CANCKS_SHIFT)
#define CiCTRL1_ABAT_MASK   BIT(CiCTRL1_ABAT_SHIFT)
#define CiCTRL1_CSIDL_MASK  BIT(CiCTRL1_CSIDL_SHIFT)

#define CiCTRL2 0x02
#define CiVEC   0x04
enum {
	CiVEC_ICODE_SHIFT = 0,
	CiVEC_FILHIT_SHIFT = 8
};
#define CiVEC_ICODE_MASK (0x7FUL << CiVEC_ICODE_SHIFT)
#define CiVEC_FILHIT_MASK (0x1FUL << CiVEC_FILHIT_SHIFT)

#define CiFCTRL 0x06
enum {
	CiFCTRL_FSA_SHIFT = 0,
	CiFCTRL_DMABS_SHIFT = 13
};
#define CiFCTRL_FSA_MASK (0x1FUL << CiFCTRL_FSA_SHIFT)
#define CiFCTRL_DMABS_MASK (0x7UL << CiFCTRL_DMABS_SHIFT)

#define CiFIFO  0x08
enum {
	CiFIFO_FNRB_SHIFT = 0,
	CiFIFO_FBP_SHIFT = 8
};
#define CiFIFO_FNRB_MASK (0x3FUL << CiFIFO_FNRB_SHIFT)
#define CiFIFO_FBP_MASK (0x3FUL << CiFIFO_FBP_SHIFT)

#define CiINTF  0x0A
enum {
	CiINTF_TBIF_SHIFT = 0,
	CiINTF_RBIF_SHIFT = 1,
	CiINTF_RBOVIF_SHIFT = 2,
	CiINTF_FIFOIF_SHIFT = 3,
	CiINTF_ERRIF_SHIFT = 5,
	CiINTF_WAKIF_SHIFT = 6,
	CiINTF_IVRIF_SHIFT = 7,
	CiINTF_EWARN_SHIFT = 8,
	CiINTF_RXWAR_SHIFT = 9,
	CiINTF_TXWAR_SHIFT = 10,
	CiINTF_RXBP_SHIFT = 11,
	CiINTF_TXBP_SHIFT = 12,
	CiINTF_TXBO_SHIFT = 13
};
#define CiINTF_TBIF_MASK   BIT(CiINTF_TBIF_SHIFT)
#define CiINTF_RBIF_MASK   BIT(CiINTF_RBIF_SHIFT)
#define CiINTF_RBOVIF_MASK BIT(CiINTF_RBOVIF_SHIFT)
#define CiINTF_FIFOIF_MASK BIT(CiINTF_FIFOIF_SHIFT)
#define CiINTF_ERRIF_MASK  BIT(CiINTF_ERRIF_SHIFT)
#define CiINTF_WAKIF_MASK  BIT(CiINTF_WAKIF_SHIFT)
#define CiINTF_IVRIF_MASK  BIT(CiINTF_IVRIF_SHIFT)
#define CiINTF_EWARN_MASK  BIT(CiINTF_EWARN_SHIFT)
#define CiINTF_RXWAR_MASK  BIT(CiINTF_RXWAR_SHIFT)
#define CiINTF_TXWAR_MASK  BIT(CiINTF_TXWAR_SHIFT)
#define CiINTF_RXBP_MASK   BIT(CiINTF_RXBP_SHIFT)
#define CiINTF_TXBP_MASK   BIT(CiINTF_TXBP_SHIFT)
#define CiINTF_TXBO_MASK   BIT(CiINTF_TXBO_SHIFT)

#define CiINTE  0x0C
enum {
	CiINTE_TBIE_SHIFT = 0,
	CiINTE_RBIE_SHIFT = 1,
	CiINTE_RBOVIE_SHIFT = 2,
	CiINTE_FIFOIE_SHIFT = 3,
	CiINTE_ERRIE_SHIFT = 5,
	CiINTE_WAKIE_SHIFT = 6,
	CiINTE_IVRIE_SHIFT = 7
};
#define CiINTE_TBIE_MASK   BIT(CiINTE_TBIE_SHIFT) 
#define CiINTE_RBIE_MASK   BIT(CiINTE_RBIE_SHIFT)
#define CiINTE_RBOVIE_MASK BIT(CiINTE_RBOVIE_SHIFT)
#define CiINTE_FIFOIE_MASK BIT(CiINTE_FIFOIE_SHIFT)
#define CiINTE_ERRIE_MASK  BIT(CiINTE_ERRIE_SHIFT)
#define CiINTE_WAKIE_MASK  BIT(CiINTE_WAKIE_SHIFT)
#define CiINTE_IVRIE_MASK  BIT(CiINTE_IVRIE_SHIFT)

#define CiEC    0x0E
enum {
	CiEC_RERRCNT_SHIFT = 0,
	CiEC_TERRCNT_SHIFT = 8
};
#define CiEC_RERRCNT_MASK (0xFFUL << CiRERRCNT_SHIFT)
#define CiEC_TERRCNT_MASK (0xFFUL << CiTERRCNT_SHIFT)

#define CiCFG1  0x10
enum {
	CiCFG1_BRP_SHIFT = 0,
	CiCFG1_SJW_SHIFT = 6
};
#define CiCFG1_BRP_MASK (0x3FUL << CiCFG1_BRP_SHIFT)
#define CiCFG1_SJW_MASK (0x3UL << CiCFG1_SJW_SHIFT)

#define CiCFG2  0x12
enum {
	CiCFG2_PRSEG_SHIFT = 0,
	CiCFG2_SEG1PH_SHIFT = 3,
	CiCFG2_SAM_SHIFT = 6,
	CiCFG2_SEG2PHTS_SHIFT = 7,
	CiCFG2_SEG2PH_SHIFT = 8,
	CiCFG2_WAKFIL_SHIFT = 14
};
#define CiCFG2_PRSEG_MASK    (0x7UL << CiCFG2_PRSEG_SHIFT)
#define CiCFG2_SEG1PH_MASK   (0x7UL << CiCFG2_SEG1PH_SHIFT)
#define CiCFG2_SAM_MASK      BIT(CiCFG2_SAM_MASK)
#define CiCFG2_SEG2PHTS_MASK BIT(CiCFG2_SEG2PHTS_SHIFT)
#define CiCFG2_SEG2PH_MASK   (0x7UL << CiCFG2_SEC2PH_SHIFT)
#define CiCFG2_WAKFIL_MASK   BIT(CiCFG2_WAKFIL_MASK)

#define CiFEN1  0x14
#define CiFMSKSELn 0x18

// WIN=0
#define CiRXFULn 0x20
#define CiRXOVFn 0x28
#define CiTRmnCON 0x30
enum {
	CiTRmnCON_TXPRI_SHIFT = 0,
	CiTRmnCON_RTREN_SHIFT = 2,
	CiTRmnCON_TXREQ_SHIFT = 3,
	CiTRmnCON_TXERR_SHIFT = 4,
	CiTRmnCON_TXLARB_SHIFT = 5,
	CiTRmnCON_TXABT_SHIFT = 6,
	CiTRmnCON_TXEN_SHIFT = 7,
	CiTRmnCON_N_SHIFT = 8
};
#define CiTRmnCON_TXPRI_MASK   (0x3UL << CiTRmnCON_TXPRI_SHIFT)
#define CiTRmnCON_RTREN_MASK  BIT(CiTRmnCON_RTREN_SHIFT) 
#define CiTRmnCON_TXREQ_MASK  BIT(CiTRmnCON_TXREQ_SHIFT)
#define CiTRmnCON_TXERR_MASK  BIT(CiTRmnCON_TXERR_SHIFT)
#define CiTRmnCON_TXLARB_MASK BIT(CiTRmnCON_TXLARB_SHIFT)
#define CiTRmnCON_TXABT_MASK  BIT(CiTRmnCON_TXABT_SHIFT)
#define CiTRmnCON_TXEN_MASK   BIT(CiTRmnCON_TXEN_SHIFT)

#define CiRXD 0x40
#define CiTXD 0x42

// WIN=1
#define CiBUFPNTn 0x20
#define CiRXMnID 0x30
#define CiRXFnID 0x40
enum {
	CiRXnSID_SID_SHIFT = 5,
	CiRXnSID_EXIDE_SHIFT = 3,
	CiRXnSID_EID_SHIFT = 0
};
#define CiRXnSID_SID_MASK   (0x7FFUL << CiRXnSID_SID_SHIFT)
#define CiRXnSID_EXIDE_MASK BIT(CiRXnSID_EXIDE_SHIFT)
#define CiRXnSID_EID_MASK   (0x3UL << CiRXnSID_EID_SHIFT)

#define BUFFER_SID_SHIFT 2
#define BUFFER_SID_MASK (0x7FFUL << BUFFER_SID_SHIFT)

// DMA

#define DMASTRIDE 0x10

#define DMAiCON  0x00
enum {
	DMAiCON_MODE_SHIFT = 0,
	DMAiCON_AMODE_SHIFT = 4,
	DMAiCON_NULLW_SHIFT = 11,
	DMAiCON_HALF_SHIFT = 12,
	DMAiCON_DIR_SHIFT = 13,
	DMAiCON_SIZE_SHIFT = 14,
	DMAiCON_CHEN_SHIFT = 15
};
#define DMAiCON_MODE_MASK  (0x3UL << DMAiCON_MODE_SHIFT)
#define DMAiCON_AMODE_MASK (0x3UL << DMAiCON_AMODE_SHIFT)
#define DMAiCON_NULLW_MASK BIT(DMAiCON_NULLW_SHIFT)
#define DMAiCON_HALF_MASK  BIT(DMAiCON_HALF_SHIFT)
#define DMAiCON_DIR_MASK   BIT(DMAiCON_DIR_SHIFT)
#define DMAiCON_SIZE_MASK  BIT(DMAiCON_SIZE_SHIFT)
#define DMAiCON_CHEN_MASK  BIT(DMAiCON_CHEN_SHIFT)

#define DMAiREQ  0x02
enum {
	DMAiREQ_IRQSEL_SHIFT = 0,
	DMAiREQ_FORCE_SHIFT = 15
};
#define DMAiREQ_IRQSEL_MASK (0xFFUL << DMAiREQ_IRQSEL_SHIFT)
#define DMAiREQ_FORCE_MASK BIT(DMAiREQ_FORCE_SHIFT)

#define DMAiSTAL 0x04
#define DMAiSTAH 0x06
#define DMAiSTBL 0x08
#define DMAiSTBH 0x0A
#define DMAiPAD  0x0C
#define DMAiCNT  0x0E

#define DMAPWC 0xF0
#define DMARQC 0xF2
#define DMAPPS 0xF4

#define DMALCA 0xF6
enum {
	DMALCA_LSTCH_SHIFT = 0
};
#define DMALCA_LSTCH_MASK (0xFUL << DMALCA_LSTCH_SHIFT)

#define DSADRL 0xF8
#define DSADRH 0xFA

#define ECAN_NUM_BUFFERS 8
#define ECAN_DECLARE_BUFFER(name) uint16_t name[ECAN_NUM_BUFFERS][8] \
		__attribute__((aligned(ECAN_NUM_BUFFERS * 16)));

struct ecan_message {
	uint16_t sid;
	uint16_t data[4];
	uint8_t length;
};

struct ecan_adapter {
	void *ecan_base;
	uint8_t tx_irq;
	uint8_t rx_irq;

	void *dma_base;
	uint8_t dma_tx_channel;
	uint8_t dma_rx_channel;

	// this is a bit bizzare, but it allows us to 
	// pass a pointer and still use 2d array syntax
	uint16_t (*buffer)[8];
};

struct ecan_baud_cfg {
	uint16_t cicfg1;
	uint16_t cicfg2;
};

static inline uint8_t readb(const volatile void *addr)
{
	return *(const volatile uint8_t *)addr;
}

static inline uint16_t readw(const volatile void *addr)
{
	return *(const volatile uint16_t *)addr;
}

static inline void writeb(const uint8_t val, volatile void *addr)
{
	*((volatile uint8_t *)addr) = val;
}

static inline void writew(const uint16_t val, volatile void *addr)
{
	*((volatile uint16_t *)addr) = val;
}

static inline int read_register_bitset(uint16_t *base, int index)
{
	return (readw(base + (index >> 4)) >> (index & 0x15)) & 1;
}

static inline void clear_register_bitset(uint16_t *base, int index)
{
	uint16_t word = readw(base + (index >> 4));
	word &= ~(1UL << (index & 0x15));
	writew(word, base + (index >> 4));
}

int ecan_init(struct ecan_adapter *adapter, struct ecan_baud_cfg *cxcfg);

int ecan_write_baud_cfg(struct ecan_baud_cfg *cxcfg);

int ecan_set_filter(struct ecan_adapter *adapter, int n, uint16_t id, int m);

int ecan_set_mask(struct ecan_adapter *adapter, int m, uint16_t id_mask);

// ecan_broadcast - non-blocking CAN message transmit.
// returns the number of messages transmitted (0 or 1)
int ecan_broadcast(struct ecan_adapter *adapter, struct ecan_message *m);

// ecan_read - non-blocking CAN message receive
// returns the number of messages received (0 or 1)
int ecan_read(struct ecan_adapter *adapter, struct ecan_message *m);
