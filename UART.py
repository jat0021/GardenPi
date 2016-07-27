#------------------------------------------------------------------
# HEADER
#------------------------------------------------------------------
# John Talbot
# July 26, 2016
# This module will define functions for use in the GardenPi project

#------------------------------------------------------------------
# IMPORT MODULES
#------------------------------------------------------------------
import 
#------------------------------------------------------------------
# SERIAL PORT PARAMETERS
#------------------------------------------------------------------
SERIAL_PORT = "/dev/ttyAMA0"
BAUD_RATE = 9600

#------------------------------------------------------------------
# MID LEVEL UART FUNCTIONS
#------------------------------------------------------------------
# Reply to request with ready byte
def transmitReady():
	sp.write(RASPI_READY)
	sp.write(END_MSG)

def transmitRequest():
	sp.write(RASPI_REQ_AVR)
	sp.write(END_MSG)

def transmitConfirm():
	sp.write(RASPI_REC_MSG_CONFIRM)
	sp.write(END_MSG)

def transmitInitialize():
	sp.write(RASPI_INIT_TO_AVR)
	sp.write(END_MSG)

def transmitPackage( dataPackage ):
	i = 0
	length = len(dataPackage)
	while(i < length):
		sp.write(dataPackage[i])
		i += 1;
	sp.write(END_MSG)

#------------------------------------------------------------------
# HIGH LEVEL UART FUNCTIONS
#------------------------------------------------------------------

