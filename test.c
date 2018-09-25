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

int main()
{
	struct ecan_adapter can0;
	struct ecan_baud_cfg cfg;
	ECAN_DECLARE_BUFFER(can0buffer)

	// Configure PLL for desired clock frequency
	// Fosc = Fin * M / (N1 * N2), Fcy = Fosc / 2
	
	//Fin = 7.37 / 2^(FRCDIV) = 7.37
	CLKDIVbits.FRCDIV = 0; // FRCDIV in {0..7} (3 bits)
	// M = PLLFBD + 2 = 50
	PLLFBD = 48; // PLLFBD in {0..511} (9 bits)
	// N1 = PLLPOST * 2 + 2 = 4
	CLKDIVbits.PLLPOST = 1; // PLLPOST in {0, 1, 3} (2 bits)
	// N2 = PLLPRE + 2 = 2
	CLKDIVbits.PLLPRE = 0; // PLLPRE in {0..31} (5 bits)

	// Fosc = 7.37 / 1 * 50 / (4 * 2) = 46.0625Mhz
	// Fcy  = 46.0625 / 2 = 23Mips

	// initiate clock switch to FRC w/o PLL (NOSC=0b000) (not using PLL temporarily)
	// NOSC = 0x001 for FRC w/PLL
	// using builtin functions to correctly unlock the register
	__builtin_write_OSCCONH(0x0); // NOSC
	__builtin_write_OSCCONL(OSCCON | 0x1); // OSWEN

	// wait for clock switch to complete
	//while (OSCCONbits.COSC != 0x1);

	// wait for PLL to lock
	//while (OSCCONbits.LOCK != 0x1);

	// configure I/O pins
	TRISBbits.TRISB0 = 0; // RB0 output
	ANSELBbits.ANSB0 = 0; // RB0 digital
	ODCBbits.ODCB0 = 0; // normal digital output

    // start timer2
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.TCKPS = 0x2;
    T2CONbits.TON = 1;

	can0.ecan_base = (void *)CAN0_BASE_ADDRESS;
	can0.dma_base = (void *)DMA_BASE_ADDRESS;
	can0.tx_irq = 70;
	can0.rx_irq = 34;
	can0.dma_tx_channel = 0;
	can0.dma_rx_channel = 1;
	can0.buffer = &can0buffer[0][0];

	ecan_write_baud_cfg(&cfg);
	ecan_init(&can0, &cfg);
	
	for (;;) {
        TMR2 = 0;
        while (TMR2 < 28790);
		LATBbits.LATB0 = ~LATBbits.LATB0;
	}

	return 0;
}
