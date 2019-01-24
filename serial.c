#include <stdint.h>
#include <xc.h>
#include "defs.h"
#include "serial.h"

void
serial_init(long baud,
            enum serial_stop_bits stop,
            enum serial_data_parity dbp)
{
    U1MODEbits.PDSEL = dbp;
    U1MODEbits.STSEL = stop;
    U1BRG = (FCY / baud / 16) - 1;
    U1STAbits.UTXEN = 1;
    U1MODEbits.UARTEN = 1;
}

void
serial_putc(const char c)
{
    while (U1STAbits.UTXBF == 1);
    U1TXREG = c;
}

void
serial_puts(const char *s)
{
    do {
        serial_putc(*s);
    } while (*s++);
}

void
serial_putn(long n, int base)
{
    static const char *digits = "0123456789ABCDEF";
    int i, negative;
    char lifo[32];

    negative = 0;

    if (n < 0) {
        negative = 1;
        n = -n;
    }

    for (i = 0; n > 0; ++i) {
        lifo[i] = digits[n%base];
        n /= base;
    }

    if (negative)
        lifo[i++] = '-';

    for (--i; i >= 0; --i)
        serial_putc(lifo[i]);
}
