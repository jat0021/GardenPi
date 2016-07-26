/* 
John Talbot
July 20, 2016
This header defines all constants and functions to be used with
the GardenPi.c program
*/

#ifndef _GARDENPI_
#define _GARDENPI_	1

//---------------------------------------------------------------
// DEFINE CONSTANTS
//---------------------------------------------------------------
// Define CPU clock frequency
#ifndef F_CPU
#define F_CPU 		8000000UL
#endif

// Define trigger and echo pins for HS-SR04 sensors
	// Shared Echo pin
#define ECHO_PIN	PB0
#define ECHO_PORT	PORTB
#define ECHO_DDR	DDRB

	// Trigger for sensor 1
#define TRIG1_PIN	PB6
#define TRIG1_PORT 	PORTB
#define TRIG1_DDR	DDRB

	// Trigger for sensor 2
#define TRIG2_PIN 	PB7
#define TRIG2_PORT	PORTB
#define TRIG2_DDR	DDRB

// Define LED status pin
#define LED_PIN		PC0
#define LED_PORT	PORTC
#define LED_DDR		DDRC

//--------------------------------------------------------------
// FUNCTION DEFINITIONS
//--------------------------------------------------------------
// Function to initialize 16-bit timer for use with HC-SR04 sensor
static inline void initTimer1(void);

// Function to send data
static void sendData(uint16_t dataToSend);

// Function to trigger HC-SR04 sensor
static void triggerHCSR04(uint8_t trigPort, uint8_t trigPin);

#endif