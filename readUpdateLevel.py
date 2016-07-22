#!/usr/bin/env python3

import gspread
from oauth2client.service_account import ServiceAccountCredentials

worksheetName = "rangeTest"


scope = ['https://spreadsheets.google.com/feeds']

credentials = ServiceAccountCredentials.from_json_keyfile_name('/home/jat0021/Development/Garden Watering-78f48eddaea0.json', scope)

gc = gspread.authorize(credentials)

worksheet = gc.open(worksheetName).sheet1

