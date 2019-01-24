#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

enum serial_stop_bits {
    SERIAL_STOP_1 = 0,
    SERIAL_STOP_2 = 1
};

enum serial_data_parity {
    SERIAL_8N = 0,
    SERIAL_8E = 1,
    SERIAL_8O = 2,
    SERIAL_9N = 3

};

void serial_init(long baud, 
                 enum serial_stop_bits,
                 enum serial_data_parity);

void serial_putc(const char);
void serial_puts(const char *);
void serial_putn(long, int);

#endif // SERIAL_H
