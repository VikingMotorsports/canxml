#ifndef TESTBUS_H
#define TESTBUS_H

#include <stdint.h>
#include "ecan.h"

struct CAN_Test_Message_t {
    uint8_t SignalA;
    uint16_t SignalB;
    uint16_t SignalC;
    uint8_t SignalD;
    uint32_t SignalE;
};

void testbus_pack_CAN_Test_Message(void *data, struct CAN_Test_Message_t *m);
void testbus_unpack_CAN_Test_Message(void *data, struct CAN_Test_Message_t *m);

#endif // TESTBUS_H
