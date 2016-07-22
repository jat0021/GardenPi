#!/bin/bash
# Test avrdude programming Atmega328p using UART 

gpio -g mode 25 out
gpio -g write 25 0
sleep 0.05
gpio -g write 25 1
sleep 0.05
make flash
sleep 0.25
gpio -g mode 25 in
gpio -g mode 25 up
