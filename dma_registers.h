#ifndef DMA_REGISTERS_H
#define DMA_REGISTERS_H

#include "mmio.h"

#define DMASTRIDE 0x10

#define DMAiCON  0x00
enum {
	DMAiCON_MODE_SHIFT = 0,
	DMAiCON_AMODE_SHIFT = 4,
	DMAiCON_NULLW_SHIFT = 11,
	DMAiCON_HALF_SHIFT = 12,
	DMAiCON_DIR_SHIFT = 13,
	DMAiCON_SIZE_SHIFT = 14,
	DMAiCON_CHEN_SHIFT = 15
};
#define DMAiCON_MODE_MASK  (0x3UL << DMAiCON_MODE_SHIFT)
#define DMAiCON_AMODE_MASK (0x3UL << DMAiCON_AMODE_SHIFT)
#define DMAiCON_NULLW_MASK BIT(DMAiCON_NULLW_SHIFT)
#define DMAiCON_HALF_MASK  BIT(DMAiCON_HALF_SHIFT)
#define DMAiCON_DIR_MASK   BIT(DMAiCON_DIR_SHIFT)
#define DMAiCON_SIZE_MASK  BIT(DMAiCON_SIZE_SHIFT)
#define DMAiCON_CHEN_MASK  BIT(DMAiCON_CHEN_SHIFT)

#define DMAiREQ  0x02
enum {
	DMAiREQ_IRQSEL_SHIFT = 0,
	DMAiREQ_FORCE_SHIFT = 15
};
#define DMAiREQ_IRQSEL_MASK (0xFFUL << DMAiREQ_IRQSEL_SHIFT)
#define DMAiREQ_FORCE_MASK BIT(DMAiREQ_FORCE_SHIFT)

#define DMAiSTAL 0x04
#define DMAiSTAH 0x06
#define DMAiSTBL 0x08
#define DMAiSTBH 0x0A
#define DMAiPAD  0x0C
#define DMAiCNT  0x0E

#endif // DMA_REGISTERS_H
