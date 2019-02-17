<%
assert(len(node.subscribes) <= 16)
assert(len(node.publishes) <= 8)
%>\
#include "mmio.h"
#include "${bus_name}.h"
#include "${bus_name}_pack.h"
#include <xc.h>

ECAN_DECLARE_BUFFER(${bus_name}_buffer)

static int
${bus_name}_dma_setup_rx()
{
    // Disable channel and start configuration
    DMA0CON = 0 | (0x2 << 4);

    // Configure channel
    DMA0REQ = 34;
    DMA0CNT = 7;
    DMA0PAD = (uint16_t)(&C1RXD);
    DMA0STAL = (uint16_t)${bus_name}_buffer;
    DMA0STAH = 0;

    // Enable channel 
    DMA0CONbits.CHEN = 1;

    return 0;
}

static int
${bus_name}_dma_setup_tx()
{
    // Disable channel and start configuration
    DMA1CON = 0 | (0x2 << 4) | (1 << 13);

    // Configure channel
    DMA1REQ = 70;
    DMA1CNT = 7;
    DMA1PAD = (uint16_t)(&C1TXD);
    DMA1STAL = (uint16_t)${bus_name}_buffer;
    DMA1STAH = 0;

    // Enable channel 
    DMA1CONbits.CHEN = 1;

    return 0;
}

static void
${bus_name}_set_filters()
{
    C1RXM0SID = 0x0000;
    C1RXM0EID = 0;
% for message in node.subscribes:

    C1FEN1bits.FLTEN${loop.index} = 1;
    C1RXF${loop.index}SID = ${message.name}_SID;
    C1RXF${loop.index}EID = 0;
    C1FMSKSEL${1+loop.index//8}bits.F${loop.index}MSK = 0;
    C1BUFPNT${1+loop.index//4}bits.F${loop.index}BP = 0xF;
% endfor
}

static int
${bus_name}_set_speed(enum ecan_speed cfg)
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
${bus_name}_init(enum ecan_speed speed, enum ecan_mode mode)
{
    // set configuration mode
    C1CTRL1bits.REQOP = ECAN_CONFIG;
    // wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != ECAN_CONFIG);

    // Configure DMA channels to match
    ${bus_name}_dma_setup_tx();
    ${bus_name}_dma_setup_rx();

    ${bus_name}_set_speed(speed);
    ${bus_name}_set_filters();

    // Make sure WIN = 0
    C1CTRL1bits.WIN = 0;

    // Select clock source (2*Fp)
    C1CTRL1bits.CANCKS = 1;

    C1FCTRL = 0xC008;

    // enable transmit buffers
% for message in node.publishes:
<%
m = (loop.index//2)*2
n = m + 1
%>\
    C1TR${m}${n}CONbits.TXEN${loop.index} = 1;
% endfor

    // Set normal mode
    C1CTRL1bits.REQOP = mode;
    // wait for the mode change to take effect
    while (C1CTRL1bits.OPMODE != mode);

    return 0;
}

% for message in node.publishes:
<%
m = (loop.index//2)*2
n = m + 1
%>\
#define ${message.name}_INDEX ${loop.index}
#define ${message.name}_WORD0 ((0x7FF & ${message.name}_SID) << 2)
#define ${message.name}_WORD2 (${message.name}_DLC & 0xF)

int
${bus_name}_publish_${message.name}(struct ${message.name}_t *m)
{
    // check that buffer 0 is ready
    if (C1TR${m}${n}CONbits.TXREQ${loop.index}) {
        // TODO: handle this.
        return 0;
    }

    // clear the non-data words
    ${bus_name}_buffer[${message.name}_INDEX][0] = ${message.name}_WORD0;
    ${bus_name}_buffer[${message.name}_INDEX][1] = 0x0;
    ${bus_name}_buffer[${message.name}_INDEX][2] = ${message.name}_WORD2;
    ${bus_name}_buffer[${message.name}_INDEX][7] = 0x0;

    // write the data
    uint16_t *data = &${bus_name}_buffer[${message.name}_INDEX][3];
    ${bus_name}_pack_${message.name}(data, m);

    // mark buffer for transmission
    C1TR${m}${n}CONbits.TXREQ${loop.index} = 1;

    return 1;
}
% endfor

% for message in node.subscribes:
#define ${message.name}_FILTER ${loop.index}
% endfor

int
${bus_name}_check_subscriptions(struct ${bus_name}_subscriptions_t *subs)
{
    int flthit;
    uint16_t fnrb;
    uint16_t *data;

    fnrb = C1FIFObits.FNRB;
    if (read_register_bitset(&C1RXFUL1, fnrb)) {
        flthit = ${bus_name}_buffer[fnrb][7] >> 8;
        data = &${bus_name}_buffer[fnrb][3];

        switch (flthit) {
% for message in node.subscribes:
            case ${message.name}_FILTER:
                ${bus_name}_unpack_${message.name}(data, &subs->${message.name});
                break;
% endfor
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
