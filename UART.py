#------------------------------------------------------------------
# HEADER
#------------------------------------------------------------------
# John Talbot
# July 26, 2016
# This module will define functions for use in the GardenPi project

#------------------------------------------------------------------
# IMPORT MODULES
#------------------------------------------------------------------
import UART_Messages
import serial

#------------------------------------------------------------------
# SERIAL PORT PARAMETERS
#------------------------------------------------------------------
SERIAL_PORT = "/dev/ttyAMA0"
BAUD_RATE = 9600
DEF_TIMEOUT = 30
sp = 0

#------------------------------------------------------------------
# INITIALIZATION FUNCTION
#------------------------------------------------------------------
def initUART(port=SERIAL_PORT, baud=BAUD_RATE, timeOutVar=DEF_TIMEOUT):
	global sp
	sp = serial.Serial(port, baud, timeout=timeOutVar)
	sp.flushInput()
	sp.flushOutput()

#	# Send initialization byte to AVR
#	transmitInitialize()
#
#	# Wait for handshake byte from AVR
#	handshakeByte = sp.readline()
#
#	# Errorproof initialization
#	if (handshakeByte != AVR_INIT_TO_RASPI):
#		raise Exception("Invalid 'AVR Initialization' return")
#	else:
#		print("UART Communications Initialized!")

#------------------------------------------------------------------
# MID LEVEL UART FUNCTIONS
#------------------------------------------------------------------
# Reply to request with ready byte
def transmitReady():
	sp.write(UART_Messages.RASPI_READY)
	sp.write(UART_Messages.END_MSG)

def transmitRequest():
	sp.write(UART_Messages.RASPI_REQ_AVR)
	sp.write(UART_Messages.END_MSG)

def transmitConfirm():
	sp.write(UART_Messages.RASPI_REC_MSG_CONFIRM)
	sp.write(UART_Messages.END_MSG)

def transmitInitialize():
	sp.write(UART_Messages.RASPI_INIT_TO_AVR)
	sp.write(UART_Messages.END_MSG)

def transmitPackage( dataPackage ):
	# Check for empty data package
	if (len(dataPackage) < 1):
		raise Exception("Empty Data Package")
	else:
		# Initialize counter and length
		i = 0
		length = len(dataPackage)

		# Loop to send all bytes in data package
		while(i < length):
			sp.write(dataPackage[i])
			i += 1;

		# Send EOM byte
		sp.write(UART_Messages.END_MSG)

#------------------------------------------------------------------
# HIGH LEVEL UART FUNCTIONS
#------------------------------------------------------------------
def transmitMessage( dataMessage ):
	# Send AVR request byte
	transmitRequest()

	# Wait for and check AVR return ready byte
	readyByte = sp.readline()[0]
	if (readyByte != UART_Messages.AVR_READY):
		raise Exception("Invalid 'AVR Ready' return")
	else:
		transmitPackage(dataMessage)

	# Wait for and check AVR return message received byte
	confirmByte = sp.readline()[0]
	if (confirmByte != UART_Messages.AVR_REC_MSG_CONFIRM):
		raise Exception("Invalid 'AVR Message Confirm' return")



