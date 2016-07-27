/*
	John Talbot
	July 25, 2016
	
	PiUART is a set of functions allowing the AVR and RasPi to 
	communicate over UART. For use with GardenPi code

	Code modified from USART.h and USART.c files from Make: Programming
	AVR by Elliot Williams for use with GardenPi package

	License included in UART_Utilities file
*/

#ifndef _UART_H_
#define _UART_H_  1

// Define default baud rate
#ifndef BAUD
#define BAUD  9600
#endif

// Enable/Disable UART receive interrupt (1=Enabled / 0=Disabled)
#define ENABLE_RX_INTERRUPT 1

// Define LED debugging constants
	// Debugging LED 1
#define LED_DEBUG1_PIN		PC1
#define LED_DEBUG1_PORT		PORTC
#define LED_DEBUG1_DDR		DDRC

	// Debugging LED 2
#define LED_DEBUG2_PIN		PC2
#define LED_DEBUG2_PORT		PORTC
#define LED_DEBUG2_DDR		DDRC

	// Debugging LED 3
#define LED_DEBUG3_PIN		PC3
#define LED_DEBUG3_PORT		PORTC
#define LED_DEBUG3_DDR		DDRC


//---------------------------------------------------------------
// INITIALIZATION FUNCTION
//---------------------------------------------------------------
/* Takes the defined BAUD and F_CPU,
   calculates the bit-clock multiplier,
   and configures the hardware UART
*/
void initUART(void);


//---------------------------------------------------------------
// LOW LEVEL Tx/Rx FUNCTIONS
//---------------------------------------------------------------
// Transmit one byte of data
void transmitByte(uint8_t data);

// Receive one byte of data
uint8_t receiveByte(void);


//--------------------------------------------------------------
// MID LEVEL MESSAGE FUNCTIONS
//--------------------------------------------------------------
// Handle UART communication error
void commError(void);

static void transmitReady(void);

static void transmitRequest(void);

static void transmitConfirm(void);

static void transmitInitialize(void);


//--------------------------------------------------------------
// HIGH LEVEL MESSAGE FUNCTIONS
//--------------------------------------------------------------
// This function will receive a full request message from a
// RasPi and return proper confirmation bytes
int * receiveMessage(void);

// This function will transmit a full message to RasPi
uint8_t transmitMessage(volatile int sendData[4]);

#endif