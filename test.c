#include <stdint.h>

#include "mmio.h"
#include "oscconfig.h"
#include "ecan.h"
#include <xc.h>

#define CAN0_BASE_ADDRESS 0x400
#define DMA_BASE_ADDRESS  0xB00

// FICD
#pragma config ICS =      PGD1
#pragma config JTAGEN =   OFF

// FPOR
#pragma config WDTWIN =   WIN25
#pragma config ALTI2C1 =  OFF
#pragma config ALTI2C2 =  OFF

// FWDT
#pragma config FWDTEN =	  OFF
#pragma config WINDIS =	  OFF
#pragma config PLLKEN =	  ON
#pragma config WDTPRE =   PR128
#pragma config WDTPOST =  PS32768

// FOSC
#pragma config FCKSM =    CSECMD
#pragma config IOL1WAY =  OFF
#pragma config OSCIOFNC = OFF
#pragma config POSCMD =   NONE

// FOSCSEL
#pragma config IESO =     OFF
#pragma config FNOSC =    FRC

// FGS
#pragma config GCP =      OFF
#pragma config GWRP =     OFF

#define ct_assert(e) ((void)sizeof(char[1 - 2*!(e)]))

ECAN_DECLARE_BUFFER(can1buffer)

int main()
{
	struct ecan_adapter can1;
	struct ecan_baud_cfg cfg;
	struct ecan_message test_tx_message;
	struct ecan_message test_rx_message;

	osccon_frc30mips();

	// configure I/O pins
	TRISBbits.TRISB0 = 0; // RB0 output
	ANSELBbits.ANSB0 = 0; // RB0 digital
	ODCBbits.ODCB0 = 0; // normal digital output

	// CAN1 RX is on RPI46/RB14
	// CAN1 TX is on RP43/RB11
	__builtin_write_OSCCONL(OSCCON & ~BIT(6));
	RPINR26bits.C1RXR = 46;
	RPOR4bits.RP43R = 0xE;
	__builtin_write_OSCCONL(OSCCON | BIT(6));

	TRISBbits.TRISB14 = 1;
	TRISBbits.TRISB11 = 0;
	ODCBbits.ODCB14 = 0; // normal digital output
	ODCBbits.ODCB11 = 0; // normal digital output

	// start timer2
	T2CONbits.TCS = 0;
	T2CONbits.T32 = 0;
	T2CONbits.TGATE = 0;
	T2CONbits.TSIDL = 0;
	T2CONbits.TCKPS = 0x3;
	T2CONbits.TON = 1;

	// initialize the CAN adapter
	can1.ecan_base = (void *)CAN0_BASE_ADDRESS;
	can1.dma_base = (void *)DMA_BASE_ADDRESS;
	can1.tx_irq = 70;
	can1.rx_irq = 34;
	can1.dma_tx_channel = 0;
	can1.dma_rx_channel = 1;
	can1.buffer = can1buffer;

	ecan_write_baud_cfg(&cfg);
	ecan_init(&can1, &cfg);
	ecan_set_mask(&can1, 0, 0x0000);
	ecan_set_filter(&can1, 0, 0, 0);
	
	test_tx_message.sid = 0x123;
	test_tx_message.data_words[0] = 0x0123;
	test_tx_message.data_words[1] = 0x4567;
	test_tx_message.data_words[2] = 0x89ab;
	test_tx_message.data_words[3] = 0xcdef;
	test_tx_message.dlc = 8;

	for (;;) {
		TMR2 = 0;
		//while (TMR2 < 58594);
		while (ecan_read(&can1, &test_rx_message) == 0);
		LATBbits.LATB0 = ~LATBbits.LATB0;
	}

	return 0;
}
