/*
	John Talbot
	July 25, 2016
	
	GardenPiUARTBytes.h defines all communication bytes
	to be used in GardenPi programs
*/

#ifndef _GARDENPI_UART_BYTES_ 
#define _GARDENPI_UART_BYTES_ 	1

// Reserved Communication Bytes
#define NULL_BYTE				0x00
#define RASPI_INIT_TO_AVR		0x01
#define AVR_INIT_TO_RASPI		0x02
#define RASPI_REQ_AVR			0X04
#define AVR_REQ_RASPI			0x05
#define RASPI_READY				0x06
#define AVR_READY				0x07
#define RASPI_REC_MSG_CONFIRM	0x08
#define AVR_REC_MSG_CONFIRM		0x09
#define END_MSG					0x0A

// General Communication Bytes
#define OPTION_1				0x10
#define OPTION_2				0x11
#define OPTION_3				0x12
#define OPTION_4				0x13
#define OPTION_5				0x14
#define OPTION_6				0x15
#define OPTION_7				0x16
#define OPTION_8				0x17
#define OPTION_9				0x18
#define OPTION_10				0x19
#define OPTION_11				0x1A
#define OPTION_12				0x1B
#define OPTION_13				0x1C
#define OPTION_14				0x1D
#define OPTION_15				0x1E
#define OPTION_16				0x1F

// Sensor Request Messages
#define WATER_TANK_LVL			0x20
#define SOIL_MOISTURE			0x30

// Error Messages
#define UART_COMM_ERROR			0xF0
#define HCSR04_TIMEOUT			0xF1

#endif