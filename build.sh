#!/bin/sh
xc16-gcc -g -Wall -mcpu=33EP32GP502 -T p33EP32GP502.gld -o test.elf oscconfig.c serial.c testbus.c testbus_test_a.c test.c
xc16-bin2hex test.elf
