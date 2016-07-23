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
while(1)
	os.system('clear')
	sp.write(b'\x01')
	time.sleep(0.1)
	dataRead = sp.read(4)
	if dataRead == b'\xFF\xFF':
		print("Sensor Timeout Error")
	elif dataRead == b'\xDD\xDD':
		print("UART Data Error")
	else:
		print("Distance = %04d cm" % dataRead, end="")
		worksheet.update_cell(2,3, dataRead)

	time.sleep(1)


