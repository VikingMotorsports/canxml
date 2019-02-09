#ifndef TESTBUS_PACK_H
#define TESTBUS_PACK_H

#include <stdint.h>
#include "testbus_messages.h"

#define MASK(n) ((1UL<<(n))-1)

void
testbus_pack_CAN_Test_Message(void *data, struct CAN_Test_Message_t *m)
{
    *((uint8_t *)data + 0) = ((uint8_t)m->SignalA & MASK(5)) << 1;
    *((uint32_t *)data + 0) |= ((uint32_t)m->SignalA) << 6;
    *((uint16_t *)data + 1) |= ((uint16_t)m->SignalC & MASK(10)) << 6;
    *((uint8_t *)data + 4) = m->SignalD;
    *((uint32_t *)data + 1) |= ((uint32_t)m->SignalE & MASK(24)) << 8;
}

void
testbus_unpack_CAN_Test_Message(void *data, struct CAN_Test_Message_t *m)
{
    m->SignalA = (*((uint8_t *)data + 0) >> 1) & MASK(5);
    m->SignalB = (*((uint32_t *)data + 0) >> 6);
    m->SignalC = (*((uint16_t *)data + 1) >> 6) & MASK(10);
    m->SignalD = *((uint8_t *)data + 4);
    m->SignalE = (*((uint32_t *)data + 1) >> 8) & MASK(24);
}

#endif // TESTBUS_PACK_H
