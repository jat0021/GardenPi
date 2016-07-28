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
import gspread
import UART
import UART_Messages
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
UART.initUART()

#---------------------------------
# MAIN PROGRAM LOOP
#---------------------------------
# Compile data message polling information from HC-SR04 sensor 1
sensorRequest = UART_Messages.WATER_TANK_LVL
	+ UART_Messages.OPTION1
	+ UART_Messages.NULL_BYTE
	+ UART_Messages.NULL_BYTE

# Get sensor data
sensorData = UART.transmitMessage( sensorRequest )

# Write sensor data to google sheet
worksheet.update_cell(2,3, sensorData)




