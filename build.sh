#!/bin/sh
xc16-gcc -mcpu=33EP32GP502 -T p33EP32GP502.gld -o test.elf test.c ecan.c
xc16-bin2hex test.elf
