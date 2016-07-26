/* 
John Talbot
July 20, 2016

This program measures distance using an HC-SR04 ultrasonic sensor and then transmits mesaurement over UART
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GardenPiUARTBytes.h"
#include "UART.h"
#include "GardenPi.h"

//--------------------------------------------------------------
// GLOBAL VARIABLE DEFINITIONS
//--------------------------------------------------------------
volatile uint8_t byteReceived, wtrLvlISRDone;
volatile int dataToSend[4];
volatile int *dataIn;
volatile uint16_t beginCount, endCount;
volatile uint16_t elapsedCounts;
volatile uint16_t distance;


//---------------------------------------------------------------
// INITIALIZATION FUNCTIONS
//---------------------------------------------------------------
// Initialize 16-bit timer for use with HC-SR04 sensor
static inline void initTimer1(void){
	TCCR1B |= (1<<CS11) | (1<<ICES1);
	TIMSK1 |= (1<<ICIE1) | (1<<ICNC1);
}

//---------------------------------------------------------------
// LOCAL FUNCTIONS
//---------------------------------------------------------------
/*Trigger HC-SR04
  This function will trigger an HC-SR04 ultrasonic range seNsor
  by generating a 15us HIGH pulse on the selected trigger pin */
static void triggerHCSR04(uint8_t trigPort, uint8_t trigPin){
	trigPort &= ~(1<<trigPin);
	_delay_us(15);
	trigPort |= (1<<trigPin);
	_delay_us(15);
	trigPort &= ~(1<<trigPin);
}

// Read water level in tank using HC-SR04 sensor
static void readWaterTankLvl(uint8_t sensorCode){
	// Good send flag
	uint8_t goodSend, i;

	// Clear timer1
	TCNT1 = 0;

	// Reset water level interrupt done flag
	wtrLvlISRDone = 0;

	// Trigger correct HC-SR04 sensor
	switch(sensorCode){
		case TANK1:
			triggerHCSR04(TRIG1_PORT, TRIG1_PIN);
			break;
		case TANK2:
			triggerHCSR04(TRIG2_PORT, TRIG2_PIN);
			break;
		default:
			commError();
	}	

	// Enable timer1 overflow interrupt
	TIMSK1 |= (1<<TOIE1);

	// Loop until water level interrupt flag is set
	while(~wtrLvlISRDone){}

	// Calculate distance from recorded timer values
	distance = (endCount - beginCount) / 58;

	// Populate data out array
	dataToSend[0] = WATER_TANK_LVL;
	dataToSend[1] = sensorCode;
	dataToSend[2] = (distance >> 8);
	dataToSend[3] = distance;

	// Send message to RasPi containing sensor data
	goodSend = transmitMessage(dataToSend);

	// Blink LED to indicate send error
	if(~goodSend){
		for(i=0; i<5; i++){
			LED_PORT ^= (1 << LED_PIN);
			_delay_ms(100);
		}
	}
}

//---------------------------------------------------------------
// INTERRUPT ROUTINES
//---------------------------------------------------------------
// Timer 1 capture interrupt to measure distance with HC-SR04 sensor
ISR(TIMER1_CAPT_vect){
	// On rising capture edge
	if(TCCR1B & (1<<ICES1)){
		// Record timer value as beginCount
		beginCount = ICR1;

		// Switch triggering edge to falling
		TCCR1B &= ~(1<<ICES1);
	}
	// On falling capture edge
	else{
		// Record timer value as endCount
		endCount = ICR1;

		// Switch triggering edge to rising
		TCCR1B |= (1<<ICES1);

		// Disable timer1 overflow interrupt
		TIMSK1 &= ~(1<<TOIE1);

		// Set water level interrupt done flag
		wtrLvlISRDone = 1;
	}
}

// Timer 1 overflow interrupt to errorproof against HC-SR04 timeout
ISR(TIMER1_OVF_vect){
	// Clear timer1
	TCNT1 = 0;
	
	// Switch triggering edge to rising
	TCCR1B |= (1<<ICES1);

	// Disable timer1 overflow interrupt
	TIMSK1 &= ~(1<<TOIE1);

	// Set water level interrupt done flag
	wtrLvlISRDone = 1;
}

// UART receive interrupt 
ISR(USART_RX_vect){
	// Call UART utility to return pointer to incoming data
	dataIn = receiveMessage();

	// Switch to determine which function to run
	switch(dataIn[0]){

		// Read water tank level
		case WATER_TANK_LVL:
			readWaterTankLvl(dataIn[1]);
			break;

		default:
			commError();
	}
}


//--------------------------------------------------------------
// MAIN FUNCTION
//--------------------------------------------------------------
int main(){
	//Set data direction out for LED pin
	LED_DDR |= (1 << LED_PIN);

	//Set data direction out for SONIC_TRIG1 pin
	TRIG1_DDR |= (1 << TRIG1_PIN);

	// Initialize AVR hardware UART
	initUART();

	// Initialize timer1
	initTimer1();

	// Enable global interrupts
	sei();

	while(1){
		_delay_ms(750);
		LED_PORT ^= (1 << LED_PIN);
	}

	return(0);
}
