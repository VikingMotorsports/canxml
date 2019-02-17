#ifndef TESTBUS_MESSAGES_H
#define TESTBUS_MESSAGES_H

#include <stdint.h>

#define CAN_Test_Message_SID 0x12
#define CAN_Test_Message_DLC 8

struct CAN_Test_Message_t {
    uint8_t SignalA;
    uint16_t SignalB;
    uint16_t SignalC;
    uint8_t SignalD;
    uint32_t SignalE;
};

#endif // TESTBUS_MESSAGES_H
