#include <stdint.h>

#include "mmio.h"
#include "oscconfig.h"
#include "minimal.h"
#include "serial.h"
#include <xc.h>

// FICD
#pragma config ICS =      PGD1
#pragma config JTAGEN =   OFF

// FPOR
#pragma config WDTWIN =   WIN25
#pragma config ALTI2C1 =  OFF
#pragma config ALTI2C2 =  OFF

// FWDT
#pragma config FWDTEN =   OFF
#pragma config WINDIS =   OFF
#pragma config PLLKEN =   ON
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

int
main()
{
    struct CAN_Test_Message2_t tm = {
        .SignalA = 0,
        .SignalB = 0,
        .SignalC = 0,
        .SignalD = 0,
        .SignalE = 0,
        .SignalF = 0,
    }; 
    struct minimal_subscriptions_t ms;
    
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

    minimal_init(ECAN_125KBPS, ECAN_NORMAL);

    // start timer2
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.TCKPS = 0x3;
    T2CONbits.TON = 1;

    //IEC2bits.C1IE = 1;
    //C1INTEbits.RBIE = 1;

    LATBbits.LATB0 = 0;

    for (;;) {
        TMR2 = 0;
        while (TMR2 < 58594);
        minimal_check_subscriptions(&ms);
        LATBbits.LATB0 = ms.CAN_Test_Message2.SignalC & 0x1;
        tm.SignalC = !tm.SignalC;
        minimal_publish_CAN_Test_Message2(&tm);
    }

    return 0;
}

/*
void __attribute__ ( (interrupt, no_auto_psv) ) _C1Interrupt( void )
{
    IFS2bits.C1IF = 0;      // clear interrupt flag
    if( C1INTFbits.RBIF )
    {
        LATBbits.LATB0 = C1FIFObits.FNRB & 0x1;
        C1INTFbits.RBIF = 0;
    }
}
*/
