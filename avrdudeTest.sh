#!/bin/bash
# Test avrdude programming Atmega328p using UART 

gpio -g mode 25 out
gpio -g write 25 0
sleep 0.05
gpio -g write 25 1
sleep 0.05
avrdude -c arduino -p m328p -P /dev/ttyAMA0 -b 9600