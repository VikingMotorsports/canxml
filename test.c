#include <xc.h>
#include <stdint.h>

#include "ecan.h"

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

ECAN_DECLARE_BUFFER(can1buffer)

int main()
{
	struct ecan_adapter can1;
	struct ecan_baud_cfg cfg;

	// Configure PLL for desired clock frequency
	// Fosc = Fin * M / (N1 * N2), Fcy = Fosc / 2
	// Fvco = Fin * M / N1, 120 < Fvco < 340
	
	//Fin = 7.37 / 2^(FRCDIV) = 7.37
	CLKDIVbits.FRCDIV = 0; // FRCDIV in {0..7} (3 bits)
	// M = PLLFBD + 2 = 228
	PLLFBD = 226; // PLLFBD in {0..511} (9 bits)
	// N1 = PLLPRE + 2 = 7
	CLKDIVbits.PLLPRE = 5; // PLLPRE in {0..31} (5 bits)
	// N2 = (PLLPOST + 1) * 2 = 4
	CLKDIVbits.PLLPOST = 1; // PLLPOST in {0, 1, 3} (2 bits)
	
	// 60Mips:
	// Fosc = 7.37 / 1 * 228 / (7 * 2) = 120.0257Mhz
	// Fvco = 7.37 / 1 * 228 / 7 = 240.0514Mhz
	// Fcy  = 120.0257 / 2 = ~60Mips
	//
	// 30Mips:
	// Fosc = 7.37 / 1 * 228 / (7 * 4) = 60.012857Mhz
	// Fvco = 7.37 / 1 * 228 / 7 = 240.0514Mhz
	// Fcy  = 60.012857 / 2 = ~30Mips

	// initiate clock switch to FRC w/o PLL (NOSC=0b000) (not using PLL temporarily)
	// NOSC = 0x001 for FRC w/PLL
	// using builtin functions to correctly unlock the register
	__builtin_write_OSCCONH(0x1); // NOSC
	__builtin_write_OSCCONL(OSCCON | 0x1); // OSWEN

	// wait for clock switch to complete
	while (OSCCONbits.COSC != 0x1);

	// wait for PLL to lock
	while (OSCCONbits.LOCK != 0x1);

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

	can1.ecan_base = (void *)CAN0_BASE_ADDRESS;
	can1.dma_base = (void *)DMA_BASE_ADDRESS;
	can1.tx_irq = 70;
	can1.rx_irq = 34;
	can1.dma_tx_channel = 0;
	can1.dma_rx_channel = 1;
	can1.buffer = can1buffer;

	ecan_write_baud_cfg(&cfg);
	ecan_init(&can1, &cfg);

	//while (DMA0PAD != (uint16_t)(&C1TXD));
	//LATBbits.LATB0 = 1;
	for (;;) {
		TMR2 = 0;
		while (TMR2 < 58594);
		ecan_broadcast(&can1, 0);
		while (C1TR01CONbits.TXREQ0 == 1);
		LATBbits.LATB0 = ~LATBbits.LATB0;
	}

	return 0;
}
