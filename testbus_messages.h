#ifndef TESTBUS_MESSAGES_H
#define TESTBUS_MESSAGES_H

#include <stdint.h>

struct CAN_Test_Message_t {
    uint8_t SignalA;
    uint16_t SignalB;
    uint16_t SignalC;
    uint8_t SignalD;
    uint32_t SignalE;
};

#endif // TESTBUS_MESSAGES_H
