#ifndef ECAN_H
#define ECAN_H

#include <stdint.h>

struct CiCTRL1bits {
	uint16_t WIN:1;
	uint16_t :2;
	uint16_t CANCAP:1;
	uint16_t :1;
	uint16_t OPMODE:3;
	uint16_t REQOP:3;
	uint16_t CANCKS:1;
	uint16_t ABAT:1;
	uint16_t CSIDL:1;
};

struct CiCTRL2bits {
	uint16_t DNCNT:5;
};

struct CiVECbits {
	uint16_t ICODE:7;
	uint16_t :1;
	uint16_t FILHIT:5;
};

struct CiFCTRLbits {
	uint16_t FSA:5;
	uint16_t :8;
	uint16_t DMABS:3;
};

struct CiFIFObits {
	uint16_t FNRB:6;
	uint16_t :2;
	uint16_t FBP:6;
};

struct CiINTFbits {
	uint16_t TBIF:1;
	uint16_t RBIF:1;
	uint16_t RBOVIF:1;
	uint16_t FIFOIF:1;
	uint16_t :1;
	uint16_t ERRIF:1;
	uint16_t WAKIF:1;
	uint16_t IVRIF:1;
	uint16_t EWARN:1;
	uint16_t RXWAR:1;
	uint16_t TXWAR:1;
	uint16_t RXBP:1;
	uint16_t TXBP:1;
	uint16_t TXBO:1;
};

struct CiINTEbits {
	uint16_t TBIE:1;
	uint16_t RBIE:1;
	uint16_t RBOVIE:1;
	uint16_t FIFOIE:1;
	uint16_t :1;
	uint16_t ERRIE:1;
	uint16_t WAKIE:1;
	uint16_t IVRIE:1;
};

struct CiECbits {
	uint16_t RERRCNT:8;
	uint16_t TERRCNT:8;
};

struct CiCFG1bits {
	uint16_t BRP:6;
	uint16_t SJW:2;
};

struct CiCFG2bits {
	uint16_t PRSEG:3;
	uint16_t SEG1PH:3;
	uint16_t SAM:1;
	uint16_t SEG2PHTS:1;
	uint16_t SEG2PH:3;
	uint16_t :3;
	uint16_t WAKFIL:1;
};

struct CiFEN1bits {
	uint16_t FLTEN0:1;
	uint16_t FLTEN1:1;
	uint16_t FLTEN2:1;
	uint16_t FLTEN3:1;
	uint16_t FLTEN4:1;
	uint16_t FLTEN5:1;
	uint16_t FLTEN6:1;
	uint16_t FLTEN7:1;
	uint16_t FLTEN8:1;
	uint16_t FLTEN9:1;
	uint16_t FLTEN10:1;
	uint16_t FLTEN11:1;
	uint16_t FLTEN12:1;
	uint16_t FLTEN13:1;
	uint16_t FLTEN14:1;
	uint16_t FLTEN15:1;
};

struct CiFMSKSEL1bits {
	uint16_t F0MSK:2;
	uint16_t F1MSK:2;
	uint16_t F2MSK:2;
	uint16_t F3MSK:2;
	uint16_t F4MSK:2;
	uint16_t F5MSK:2;
	uint16_t F6MSK:2;
	uint16_t F7MSK:2;
};

struct CiFMSKSEL2bits {
	uint16_t F8MSK:2;
	uint16_t F9MSK:2;
	uint16_t F10MSK:2;
	uint16_t F11MSK:2;
	uint16_t F12MSK:2;
	uint16_t F13MSK:2;
	uint16_t F14MSK:2;
	uint16_t F15MSK:2;
};

struct CiBUFPNT1bits {
	uint16_t F0BP:4;
	uint16_t F1BP:4;
	uint16_t F2BP:4;
	uint16_t F3BP:4;
};

struct CiRXFUL1bits {
	uint16_t RXFUL0:1;
	uint16_t RXFUL1:1;
	uint16_t RXFUL2:1;
	uint16_t RXFUL3:1;
	uint16_t RXFUL4:1;
	uint16_t RXFUL5:1;
	uint16_t RXFUL6:1;
	uint16_t RXFUL7:1;
	uint16_t RXFUL8:1;
	uint16_t RXFUL9:1;
	uint16_t RXFUL10:1;
	uint16_t RXFUL11:1;
	uint16_t RXFUL12:1;
	uint16_t RXFUL13:1;
	uint16_t RXFUL14:1;
	uint16_t RXFUL15:1;
};

struct CiBUFPNT2bits {
	uint16_t F4BP:4;
	uint16_t F5BP:4;
	uint16_t F6BP:4;
	uint16_t F7BP:4;
};

struct CiRXFUL2bits {
	uint16_t RXFUL16:1;
	uint16_t RXFUL17:1;
	uint16_t RXFUL18:1;
	uint16_t RXFUL19:1;
	uint16_t RXFUL20:1;
	uint16_t RXFUL21:1;
	uint16_t RXFUL22:1;
	uint16_t RXFUL23:1;
	uint16_t RXFUL24:1;
	uint16_t RXFUL25:1;
	uint16_t RXFUL26:1;
	uint16_t RXFUL27:1;
	uint16_t RXFUL28:1;
	uint16_t RXFUL29:1;
	uint16_t RXFUL30:1;
	uint16_t RXFUL31:1;
};

struct CiBUFPNT3bits {
	uint16_t F8BP:4;
	uint16_t F9BP:4;
	uint16_t F10BP:4;
	uint16_t F11BP:4;
};

struct CiBUFPNT4bits {
	uint16_t F12BP:4;
	uint16_t F13BP:4;
	uint16_t F14BP:4;
	uint16_t F15BP:4;
};

struct CiRXOVF1bits {
	uint16_t RXOVF0:1;
	uint16_t RXOVF1:1;
	uint16_t RXOVF2:1;
	uint16_t RXOVF3:1;
	uint16_t RXOVF4:1;
	uint16_t RXOVF5:1;
	uint16_t RXOVF6:1;
	uint16_t RXOVF7:1;
	uint16_t RXOVF8:1;
	uint16_t RXOVF9:1;
	uint16_t RXOVF10:1;
	uint16_t RXOVF11:1;
	uint16_t RXOVF12:1;
	uint16_t RXOVF13:1;
	uint16_t RXOVF14:1;
	uint16_t RXOVF15:1;
};

struct CiRXOVF2bits {
	uint16_t RXOVF16:1;
	uint16_t RXOVF17:1;
	uint16_t RXOVF18:1;
	uint16_t RXOVF19:1;
	uint16_t RXOVF20:1;
	uint16_t RXOVF21:1;
	uint16_t RXOVF22:1;
	uint16_t RXOVF23:1;
	uint16_t RXOVF24:1;
	uint16_t RXOVF25:1;
	uint16_t RXOVF26:1;
	uint16_t RXOVF27:1;
	uint16_t RXOVF28:1;
	uint16_t RXOVF29:1;
	uint16_t RXOVF30:1;
	uint16_t RXOVF31:1;
};

struct CiTR01CONbits {
	uint16_t TX0PRI:2;
	uint16_t RTREN0:1;
	uint16_t TXREQ0:1;
	uint16_t TXERR0:1;
	uint16_t TXLARB0:1;
	uint16_t TXABT0:1;
	uint16_t TXEN0:1;
	uint16_t TX1PRI:2;
	uint16_t RTREN1:1;
	uint16_t TXREQ1:1;
	uint16_t TXERR1:1;
	uint16_t TXLARB1:1;
	uint16_t TXABT1:1;
	uint16_t TXEN1:1;
};

struct CiTR23CONbits {
	uint16_t TX2PRI:2;
	uint16_t RTREN2:1;
	uint16_t TXREQ2:1;
	uint16_t TXERR2:1;
	uint16_t TXLARB2:1;
	uint16_t TXABT2:1;
	uint16_t TXEN2:1;
	uint16_t TX3PRI:2;
	uint16_t RTREN3:1;
	uint16_t TXREQ3:1;
	uint16_t TXERR3:1;
	uint16_t TXLARB3:1;
	uint16_t TXABT3:1;
	uint16_t TXEN3:1;
};

struct CiTR45CONbits {
	uint16_t TX4PRI:2;
	uint16_t RTREN4:1;
	uint16_t TXREQ4:1;
	uint16_t TXERR4:1;
	uint16_t TXLARB4:1;
	uint16_t TXABT4:1;
	uint16_t TXEN4:1;
	uint16_t TX5PRI:2;
	uint16_t RTREN5:1;
	uint16_t TXREQ5:1;
	uint16_t TXERR5:1;
	uint16_t TXLARB5:1;
	uint16_t TXABT5:1;
	uint16_t TXEN5:1;
};

struct CiTR67CONbits {
	uint16_t TX6PRI:2;
	uint16_t RTREN6:1;
	uint16_t TXREQ6:1;
	uint16_t TXERR6:1;
	uint16_t TXLARB6:1;
	uint16_t TXABT6:1;
	uint16_t TXEN6:1;
	uint16_t TX7PRI:2;
	uint16_t RTREN7:1;
	uint16_t TXREQ7:1;
	uint16_t TXERR7:1;
	uint16_t TXLARB7:1;
	uint16_t TXABT7:1;
	uint16_t TXEN7:1;
};

struct MSIDbits {
	uint16_t EID:2;
	uint16_t :1;
	uint16_t MIDE:1;
	uint16_t :1;
	uint16_t SID:11;
};

struct FSIDbits {
	uint16_t EID:2;
	uint16_t :1;
	uint16_t EXIDE:1;
	uint16_t :1;
	uint16_t SID:11;
};

struct CiRXMn {
	union {
		uint16_t word;
		struct MSIDbits fields;
	} SID;
	uint16_t EID;
};

struct CiRXFn {
	union {
		uint16_t word;
		struct FSIDbits fields;
	} SID;
	uint16_t EID;
};

struct ECAN {
	union {
		uint16_t word;
		struct CiCTRL1bits fields;
	} CiCTRL1;
	union {
		uint16_t word;
		struct CiCTRL2bits fields;
	} CiCTRL2;
	union {
		uint16_t word;
		struct CiVECbits fields;
	} CiVEC;
	union {
		uint16_t word;
		struct CiFCTRLbits fields;
	} CiFCTRL;
	union {
		uint16_t word;
		struct CiFIFObits fields;
	} CiFIFO;
	union {
		uint16_t word;
		struct CiINTFbits fields;
	} CiINTF;
	union {
		uint16_t word;
		struct CiINTEbits fields;
	} CiINTE;
	union {
		uint16_t word;
		struct CiECbits fields;
	} CiEC;
	union {
		uint16_t word;
		struct CiCFG1bits fields;
	} CiCFG1;
	union {
		uint16_t word;
		struct CiCFG2bits fields;
	} CiCFG2;
	union {
		uint16_t word;
		struct CiFEN1bits fields;
	} CiFEN1;
	uint16_t padding0;
	union {
		uint16_t CiFMSKSEL[2];
		struct {
			union {
				uint16_t word;
				struct CiFMSKSEL1bits fields;
			} CiFMSKSEL1;
			union {
				uint16_t word;
				struct CiFMSKSEL2bits fields;
			} CiFMSKSEL2;
		};
	};
	uint16_t padding1;
	uint16_t padding2;
	union {
		// WHEN WIN=0
		struct {
			union {
				uint16_t CiRXFUL[2];
				struct {
					union {
						uint16_t word;
						struct CiRXFUL1bits fields;
					} CiRXFUL1;
					union {
						uint16_t word;
						struct CiRXFUL2bits fields;
					} CiRXFUL2;
				};
			};
			union {
				uint16_t CiOVF[2];
				struct {
					union {
						uint16_t word;
						struct CiRXOVF1bits fields;
					} CiRXOVF1;
					union {
						uint16_t word;
						struct CiRXOVF2bits fields;
					} CiRXOVF2;
				};
			};
			union {
				uint16_t CiTRCON[4];
				struct {
					union {
						uint16_t word;
						struct CiTR01CONbits fields;
					} CiTR01CON;
					union {
						uint16_t word;
						struct CiTR23CONbits fields;
					} CiTR23CON;
					union {
						uint16_t word;
						struct CiTR45CONbits fields;
					} CiTR45CON;
					union {
						uint16_t word;
						struct CiTR67CONbits fields;
					} CiTR67CON;
				};
			};
			uint16_t CiRXD;
			uint16_t CiTXD;
		};
		// WHEN WIN=1
		struct {
			union {
				uint16_t CiBUFPNTn[4];
				struct {
					union {
						uint16_t word;
						struct CiBUFPNT1bits fields;
					} CiBUFPNT1;
					union {
						uint16_t word;
						struct CiBUFPNT2bits fields;
					} CiBUFPNT2;
					union {
						uint16_t word;
						struct CiBUFPNT3bits fields;
					} CiBUFPNT3;
					union {
						uint16_t word;
						struct CiBUFPNT4bits fields;
					} CiBUFPNT4;
					
				};
			};
			uint16_t padding3;
			uint16_t padding4;
			uint16_t padding5;
			uint16_t padding7;
			struct CiRXMn CiRXM[3];
			uint16_t padding8;
			uint16_t padding9;
			struct CiRXFn CiRXF[16];
		};
	};
};

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

#endif // ECAN_H
