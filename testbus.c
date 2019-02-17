#include "mmio.h"
#include "testbus.h"
#include "testbus_pack.h"
#include <xc.h>

ECAN_DECLARE_BUFFER(testbus_buffer)

static int
testbus_dma_setup_rx()
{
    // Disable channel and start configuration
    DMA0CON = 0 | (0x2 << 4);

    // Configure channel
    DMA0REQ = 34;
    DMA0CNT = 7;
    DMA0PAD = (uint16_t)(&C1RXD);
    DMA0STAL = (uint16_t)testbus_buffer;
    DMA0STAH = 0;

    // Enable channel 
    DMA0CONbits.CHEN = 1;

    return 0;
}

static int
testbus_dma_setup_tx()
{
    // Disable channel and start configuration
    DMA1CON = 0 | (0x2 << 4) | (1 << 13);

    // Configure channel
    DMA1REQ = 70;
    DMA1CNT = 7;
    DMA1PAD = (uint16_t)(&C1TXD);
    DMA1STAL = (uint16_t)testbus_buffer;
    DMA1STAH = 0;

    // Enable channel 
    DMA1CONbits.CHEN = 1;

    return 0;
}

static void
testbus_set_filters()
{
    C1RXM0SID = 0x0000;
    C1RXM0EID = 0;

    C1FEN1bits.FLTEN0 = 1;
    C1RXF0SID = CAN_Test_Message_SID;
    C1RXF0EID = 0;
    C1FMSKSEL1bits.F0MSK = 0;
    C1BUFPNT1bits.F0BP = 0xF;
}

static int
testbus_set_speed(enum ecan_speed cfg)
{
    // using 10 time quanta per bit
    // synchronization segment is 1TQ
    // phase segment 2 is 3TQ
    // propagation segment is 3TQ
    // phase segment 1 is 3TQ
    // Fp = 30000000
    // Fcan = 60000000
    // Ftq = 125000 * 10 = 1250000
    // Fbrp = 1250000 * 2 = 2500000
    // Fp / Fbrp = 600/25 = 24
    // BRP = Fp / Fbrp - 1 = 24 - 1 = 23
    
    if (cfg == ECAN_125KBPS) {
        C1CFG1bits.SJW = 2;
        C1CFG1bits.BRP = 23;
        // PRSEG + 1 = propagation segment bits
        C1CFG2bits.PRSEG = 2;
        // SEG1PH + 1 = phase segment 1 bits
        C1CFG2bits.SEG1PH = 2;
        // SEG2PH + 1 = phase segment 2 bits
        C1CFG2bits.SEG2PH = 2;
        // yes, I want to set phase segment 2
        C1CFG2bits.SEG2PHTS = 1;

        return 0;
    }

    return -1;
}

int
testbus_init(enum ecan_speed speed, enum ecan_mode mode)
{
    // set configuration mode
    C1CTRL1bits.REQOP = ECAN_CONFIG;
    // wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != ECAN_CONFIG);

    // Configure DMA channels to match
    testbus_dma_setup_tx();
    testbus_dma_setup_rx();

    testbus_set_speed(speed);
    testbus_set_filters();

    // Make sure WIN = 0
    C1CTRL1bits.WIN = 0;

    // Select clock source (2*Fp)
    C1CTRL1bits.CANCKS = 1;

    C1FCTRL = 0xC001;

    // set buffer 0 as a transmit buffer
    C1TR01CONbits.TXEN0 = 1;

    // Set normal mode
    C1CTRL1bits.REQOP = mode;
    // wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != mode);

    return 0;
}

#define CAN_Test_Message_INDEX 0
#define CAN_Test_Message_WORD0 ((0x7FF & CAN_Test_Message_SID) << 2)
#define CAN_Test_Message_WORD2 (CAN_Test_Message_DLC & 0xF)

int
testbus_publish_CAN_Test_Message(struct CAN_Test_Message_t *m)
{
    // check that buffer 0 is ready
    if (C1TR01CONbits.TXREQ0) {
        // TODO: handle this.
        return 0;
    }

    // clear the non-data words
    testbus_buffer[CAN_Test_Message_INDEX][0] = CAN_Test_Message_WORD0;
    testbus_buffer[CAN_Test_Message_INDEX][1] = 0x0;
    testbus_buffer[CAN_Test_Message_INDEX][2] = CAN_Test_Message_WORD2;
    testbus_buffer[CAN_Test_Message_INDEX][7] = 0x0;

    // write the data
    uint16_t *data = &testbus_buffer[CAN_Test_Message_INDEX][3];
    testbus_pack_CAN_Test_Message(data, m);

    // mark buffer for transmission
    C1TR01CONbits.TXREQ0 = 1;

    return 1;
}

#define CAN_Test_Message_FILTER 8

int
testbus_check_subscriptions(struct testbus_subscriptions_t *subs)
{
    int flthit;
    uint16_t fnrb;
    uint16_t *data;

    fnrb = C1FIFObits.FNRB;
    if (read_register_bitset(&C1RXFUL1, fnrb)) {
        flthit = testbus_buffer[fnrb][7] >> 8;
        data = &testbus_buffer[fnrb][3];

        switch (flthit) {
            case CAN_Test_Message_FILTER:
                testbus_unpack_CAN_Test_Message(data, &subs->CAN_Test_Message);
                break;
            default:
                break;
        }

        // TODO: handle overflow.
        read_register_bitset(&C1RXOVF1, fnrb);

        // Clear RXFUL bit (bumps FNRB register)
        clear_register_bitset(&C1RXFUL1, fnrb);

        return 1;
    }
    
    return 0;
}
