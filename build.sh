#!/bin/sh
xc16-gcc -Wall -mcpu=33EP32GP502 -T p33EP32GP502.gld -o test.elf ecan.c test.c
xc16-bin2hex test.elf
