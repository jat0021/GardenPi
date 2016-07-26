#!/bin/bash
# Test avrdude programming Atmega328p using UART 

# Pull AVR reset pin low
gpio -g mode 25 out
gpio -g write 25 0

# Compile AVR C code before continuing
make

# Pull AVR reset pin high
gpio -g write 25 1
sleep 0.05

# flash AVR chip with program
make flash
sleep 0.25

# Change reset pin into HiZ input to safely keep AVR in active mode
gpio -g mode 25 in
gpio -g mode 25 up
