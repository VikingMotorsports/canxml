#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

#define BIT(bitshift) (1UL << (bitshift))

static inline uint8_t readb(const volatile void *addr)
{
	return *(const volatile uint8_t *)addr;
}

static inline uint16_t readw(const volatile void *addr)
{
	return *(const volatile uint16_t *)addr;
}

static inline void writeb(const uint8_t val, volatile void *addr)
{
	*((volatile uint8_t *)addr) = val;
}

static inline void writew(const uint16_t val, volatile void *addr)
{
	*((volatile uint16_t *)addr) = val;
}

static inline int read_register_bitset(uint16_t *base, int index)
{
	return (readw(base + (index >> 4)) >> (index & 0xFF)) & 1;
}

static inline void clear_register_bitset(uint16_t *base, int index)
{
	uint16_t word = ~(1UL << (index & 0xFF));
	writew(word, base + (index >> 4));
}

#endif // MMIO_H
