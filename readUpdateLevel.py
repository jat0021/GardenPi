#!/usr/bin/env python3

#-------------------------------------------------------------------
# HEADER
#-------------------------------------------------------------------
# John Talbot
# July 22, 2016
# This program will trigger an atmega over UART to read range. It
# will then populate a google sheet with the range data

#-----------------------------
# IMPORT PACKAGES
#-----------------------------
import os
import time
import serial
import gspread
from oauth2client.service_account import ServiceAccountCredentials

#------------------------------
# INITIALIZE GOOGLE SHEET DATA
#------------------------------
worksheetName = "rangeTest"

scope = ['https://spreadsheets.google.com/feeds']

credentials = ServiceAccountCredentials.from_json_keyfile_name('/home/jat0021/Development/Garden Watering-78f48eddaea0.json', scope)

gc = gspread.authorize(credentials)

worksheet = gc.open(worksheetName).sheet1

#--------------------------------
# INITIALIZE SERIAL PORT
#--------------------------------
SERIAL_PORT = "/dev/ttyAMA0"
BAUD_RATE = 9600

sp = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = 5)
sp.flushInput()
sp.flushOutput()

#---------------------------------
# MAIN PROGRAM LOOP
#---------------------------------
while(1):
	# Clear Screen
	os.system('clear')

	# Write sensor trigger byte to atmega
	sp.write(b'\x01')

	# Read sensor data from atmega
	dataRead = sp.read(5)
	if dataRead == b'65535':
		print("Sensor Timeout Error")
	elif dataRead == b'56797':
		print("UART Data Error")
	else:
		sensorStr = dataRead.decode()
		sensorData = int(sensorStr[1:5])
		print("Distance = %04d cm" % dataRead, end="")
		worksheet.update_cell(2,3, sensorData)

	time.sleep(1)


