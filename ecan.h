#ifndef ECAN_H
#define ECAN_H

#include <stdint.h>

#define ECAN_NUM_BUFFERS 16
#define ECAN_DECLARE_BUFFER(name) uint16_t name[ECAN_NUM_BUFFERS][8] \
        __attribute__((aligned(ECAN_NUM_BUFFERS * 16)));

enum ecan_speed {
    ECAN_10KBPS,
    ECAN_20KBPS,
    ECAN_50KBPS,
    ECAN_100KBPS,
    ECAN_125KBPS,
    ECAN_250KBPS,
    ECAN_500KBPS,
    ECAN_1000KBPS
};

enum ecan_mode {
    ECAN_NORMAL = 0,
    ECAN_DISABLE = 1,
    ECAN_LOOPBACK = 2,
    ECAN_LISTEN_ONLY = 3,
    ECAN_CONFIG = 4,
    ECAN_LISTEN_ALL = 7
};

#endif // ECAN_H
