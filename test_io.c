#include <stdint.h>

#include "mmio.h"
#include "oscconfig.h"
#include <xc.h>


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


int main()
{
	osccon_frc30mips();

	// configure I/O pins
	TRISBbits.TRISB0 = 0; // RB0 output
	ANSELBbits.ANSB0 = 0; // RB0 digital
	ODCBbits.ODCB0 = 0; // normal digital output

	// start timer2
	T2CONbits.TCS = 0;
	T2CONbits.T32 = 0;
	T2CONbits.TGATE = 0;
	T2CONbits.TSIDL = 0;
	T2CONbits.TCKPS = 0x3;
	T2CONbits.TON = 1;

	for (;;) {
		TMR2 = 0;
		while (TMR2 < 58594);
		//LATBbits.LATB0 = ~LATBbits.LATB0;
		atomic_xor(0x1, (volatile uint16_t *)(&LATBbits));
	}

	return 0;
}
