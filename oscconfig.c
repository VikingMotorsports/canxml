#include <xc.h>

// Oscillator configuration details:
// 60Mips w/FRC:
// Fosc = 7.37 / 1 * 228 / (7 * 2) = 120.0257Mhz
// Fvco = 7.37 / 1 * 228 / 7 = 240.0514Mhz
// Fcy  = 120.0257 / 2 = ~60Mips
//
// 30Mips w/FRC:
// Fosc = 7.37 / 1 * 228 / (7 * 4) = 60.012857Mhz
// Fvco = 7.37 / 1 * 228 / 7 = 240.0514Mhz
// Fcy  = 60.012857 / 2 = ~30Mips

// configure PLL for 30Mips (60Mhz clock) operation with the
// 7.37Mhz FRC oscillator
void osccon_frc30mips() {
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
	
	// initiate clock switch to FRC w/PLL (NOSC=0b001)
	// using builtin functions to correctly unlock the register
	__builtin_write_OSCCONH(0x1); // NOSC
	__builtin_write_OSCCONL(OSCCON | 0x1); // OSWEN

	// wait for clock switch to complete
	while (OSCCONbits.COSC != 0x1);
	// wait for PLL to lock
	while (OSCCONbits.LOCK != 0x1);
}

// configure PLL for 60Mips (120Mhz clock) operation with the
// 7.37Mhz FRC oscillator
void osccon_frc60mips() {
	// Configure PLL for desired clock frequency
	// Fosc = Fin * M / (N1 * N2), Fcy = Fosc / 2
	// Fvco = Fin * M / N1, 120 < Fvco < 340
	
	//Fin = 7.37 / 2^(FRCDIV) = 7.37
	CLKDIVbits.FRCDIV = 0; // FRCDIV in {0..7} (3 bits)
	// M = PLLFBD + 2 = 228
	PLLFBD = 226; // PLLFBD in {0..511} (9 bits)
	// N1 = PLLPRE + 2 = 7
	CLKDIVbits.PLLPRE = 5; // PLLPRE in {0..31} (5 bits)
	// N2 = (PLLPOST + 1) * 2 = 2
	CLKDIVbits.PLLPOST = 0; // PLLPOST in {0, 1, 3} (2 bits)
	
	// initiate clock switch to FRC w/PLL (NOSC=0b001)
	// using builtin functions to correctly unlock the register
	__builtin_write_OSCCONH(0x1); // NOSC
	__builtin_write_OSCCONL(OSCCON | 0x1); // OSWEN

	// wait for clock switch to complete
	while (OSCCONbits.COSC != 0x1);
	// wait for PLL to lock
	while (OSCCONbits.LOCK != 0x1);
}
