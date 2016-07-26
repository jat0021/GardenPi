/* 
John Talbot
July 20, 2016

This header defines all constants and functions to be used with
the GardenPi.c program
*/

#ifndef _GARDENPI_H_
#define _GARDENPI_H_	1

//---------------------------------------------------------------
// DEFINE CONSTANTS
//---------------------------------------------------------------
// Define CPU clock frequency if not already defined
#ifndef F_CPU
#define F_CPU 		8000000UL
#endif

// Define trigger and echo pins for HS-SR04 sensors
	// Shared Echo pin
#define ECHO_PIN	PB0
#define ECHO_PORT	PORTB
#define ECHO_DDR	DDRB

	// Trigger for sensor 1
#define TANK1 		0x10
#define TRIG1_PIN	PB6
#define TRIG1_PORT 	PORTB
#define TRIG1_DDR	DDRB

	// Trigger for sensor 2
#define TANK2		0x11
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
// Initialize 16-bit timer for use with HC-SR04 sensor
static inline void initTimer1(void);

// Trigger HC-SR04 sensor
static void triggerHCSR04(uint8_t trigPort, uint8_t trigPin);

// Read water level in tank
static void readWaterTankLvl(void);

#endif