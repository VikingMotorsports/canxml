#ifndef TESTBUS_H
#define TESTBUS_H

#include <stdint.h>
#include "ecan.h"

struct CAN_Test_Message_t {
    int a;
};

void testbus_pack_CAN_Test_Message(uint16_t *data, struct CAN_Test_Message_t *m);
int testbus_unpack_CAN_Test_Message(uint16_t *data, struct CAN_Test_Message_t *m);

#endif // TESTBUS_H
