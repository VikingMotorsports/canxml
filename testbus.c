#include <stdint.h>
#include "testbus.h"

void
testbus_pack_CAN_Test_Message(uint16_t *data, struct CAN_Test_Message_t *m)
{
    data[0] = 0 
            | ((m->SignalA & 0x1F) << 1)
            | ((m->SignalB & 0xFFFF) << 6);
    data[1] = 0
            | ((m->SignalB & 0xFFFF) >> 10)
            | ((m->SignalC & 0x3FF) << 6);
    data[2] = ((m->SignalD & 0xFF) << 0)
            | ((m->SignalE & 0xFFFFFF) << 8);
    data[3] = ((m->SignalE & 0xFFFFFF) >> 8);
}

void
testbus_unpack_CAN_Test_Message(uint16_t *data, struct CAN_Test_Message_t *m)
{
    m->SignalA = (data[0] >> 1) & 0x1F;
    m->SignalB = ((data[0] >> 6) | (data[1] << 10)) & 0xFFFF;
    m->SignalC = (data[1] >> 6) & 0x3FF;
    m->SignalD = (data[2] >> 0) & 0xFF;
    m->SignalE = ((uint32_t)(data[2] >> 8) | (((uint32_t)data[3]) << 8));
}

