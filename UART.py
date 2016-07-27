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
import time
import serial
import struct
import os

#------------------------------------------------------------------
# SERIAL PORT PARAMETERS
#------------------------------------------------------------------
SERIAL_PORT = "/dev/ttyAMA0"
BAUD_RATE = 9600
DEF_TIMEOUT = 30
PACKAGE_LEN = 4
sp = 0

#------------------------------------------------------------------
# INITIALIZATION FUNCTION
#------------------------------------------------------------------
def initUART(port=SERIAL_PORT, baud=BAUD_RATE, timeOutVar=DEF_TIMEOUT):
	# Echo message to user
	print("Initializing UART Communications... Please Wait")
	time.sleep(3)

	# Open serial port and map to global variable sp
	global sp
	sp = serial.Serial(port, baud, timeout=timeOutVar)
	sp.flushInput()
	sp.flushOutput()

	# Send initialization byte to AVR
	transmitInitialize()

	# Wait for handshake byte from AVR
	handshakeByte = sp.readline()[:-1]

	# Errorproof initialization
	if (handshakeByte != UART_Messages.AVR_INIT_TO_RASPI):
		os.system('clear')
		print(handshakeByte)
		raise Exception("Invalid 'AVR Initialization' return")
	else:
		os.system('clear')
		print("UART Communications Initialized!")
		time.sleep(3)
		os.system('clear')

#------------------------------------------------------------------
# MID LEVEL UART FUNCTIONS
#------------------------------------------------------------------
# Read in package, strip EOM character, and encode into hex
def readPackage():
	# Initialize array to return data bytes
	dataOut = [];

	# Read in package and strip newline
	packageIn = sp.readline()[:-1]

	# Check package length - If != 4 throw exception
	packageLength = len(packageIn)
	if (packageLength != PACKAGE_LEN):
		raise Exception("Incorrect Package Length")
	else:
		dataOut.append(hex(packageIn[0]))
		dataOut.append(hex(packageIn[1]))
		dataOut.append(convertData(packageIn[2:4]))
		return dataOut

# Check data message for errors - If none convert to int
def convertData(dataIn):
	# Data message should be 2 bytes long
	msgLen = len(dataIn)
	if (msgLen != 2):
		raise Exception("Incorrect Data Length for convertData()")
	elif (dataIn[0] >= UART_Messages.UART_COMM_ERROR):
		parseError(dataIn[0])
	else:
		dataRet = struct.unpack('>H',dataIn)
		return dataRet

# Parse error code
def parseError(errorCode):
	if (errorCode == UART_Messages.UART_COMM_ERROR):
		raise Exception("Communications Error with AVR")
	elif (errorCode == UART_Messages.HCSR04_TIMEOUT):
		raise Exception("HC-SR04 Sensor Timeout Error")
	else:
		raise Exception("Unrecognized Error Code")

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
	readyByte = sp.readline()[:-1]
	if (readyByte != UART_Messages.AVR_READY):
		raise Exception("Invalid 'AVR Ready' return")
	else:
		transmitPackage(dataMessage)

	# Wait for and check AVR return message received byte
	confirmByte = sp.readline()[:-1]
	if (confirmByte != UART_Messages.AVR_REC_MSG_CONFIRM):
		raise Exception("Invalid 'AVR Message Confirm' return")



