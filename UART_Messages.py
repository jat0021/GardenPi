#------------------------------------------------------------------
# HEADER
#------------------------------------------------------------------
# John Talbot
# July 26, 2016
# This module will define messages and functions for use in the
# GardenPi project

#------------------------------------------------------------------
# MESSAGE BYTE CONSTANTS
#------------------------------------------------------------------
# Reserved Communication Bytes
NULL_BYTE=b'\x00'
RASPI_INIT_TO_AVR=b'\x01'
AVR_INIT_TO_RASPI=b'\x02'
RASPI_REQ_AVR=b'\x04'
AVR_REQ_RASPI=b'\x05'
RASPI_READY=b'\x06'
AVR_READY=b'\x07'
RASPI_REC_MSG_CONFIRM=b'\x08'
AVR_REC_MSG_CONFIRM=b'\x09'
END_MSG=b'\x0A'

# General Communication Bytes
OPTION_1=b'\x10'
OPTION_2=b'\x11'
OPTION_3=b'\x12'
OPTION_4=b'\x13'
OPTION_5=b'\x14'
OPTION_6=b'\x15'
OPTION_7=b'\x16'
OPTION_8=b'\x17'
OPTION_9=b'\x18'
OPTION_10=b'\x19'
OPTION_11=b'\x1A'
OPTION_12=b'\x1B'
OPTION_13=b'\x1C'
OPTION_14=b'\x1D'
OPTION_15=b'\x1E'
OPTION_16=b'\x1F'

# Sensor Request Messages
WATER_TANK_LVL=b'\x20'
SOIL_MOISTURE=b'\x30'

# Error Messages
UART_COMM_ERROR=b'\xF0'
HCSR04_TIMEOUT=b'\xF1'

