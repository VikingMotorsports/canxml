#!/bin/sh
xc16-gcc -g -Wall -mcpu=33EP32GP502 -T p33EP32GP502.gld -o test_io.elf oscconfig.c test_io.c
xc16-bin2hex test_io.elf
