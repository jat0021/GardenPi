/* 
John Talbot
July 20, 2016
This program measures distance using an HC-SR04 ultrasonic sensor and then transmits mesaurement over UART
*/

#ifndef F_CPU
#define F_CPU 				8000000UL
#endif

#define SONIC_TRIG1_PIN		PD4
#define SONIC_TRIG1_PORT 	PORTD
#define SONIC_TRIG1_DDR		DDRD

#define ECHO_PIN			PB0
#define ECHO_PORT			PORTB
#define ECHO_DDR			DDRB

#define LED_PIN				PC0
#define LED_PORT			PORTC
#define LED_DDR				DDRC

#define DIST_THRESH 		5

#define UART_TRIG_BYTE 		0x01
#define SENSOR_TIMEOUT		0xFFFF
#define UART_BAD_RECEIVE 	0xDDDD

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

volatile uint8_t byteReceived;
volatile uint16_t beginCount, endCount;
volatile uint16_t elapsedCounts;
volatile uint16_t distance;
volatile char goodReturn=1;

static inline void initTimer1(void){
	TCCR1B |= (1<<CS11) | (1<<ICES1);
	TIMSK1 |= (1<<ICIE1) | (1<<ICNC1);
}

static void sendData(uint16_t dataToSend){
	transmitByte(dataToSend >> 8);
	transmitByte(dataToSend);
	transmitByte(0x0A);
}

static inline void generatePulse(void){
	// Generate a 12us pulse to SONIC_TRIG1 the HR-SR04
	SONIC_TRIG1_PORT &= ~(1<<SONIC_TRIG1_PIN);
	_delay_us(15);
	SONIC_TRIG1_PORT |= (1<<SONIC_TRIG1_PIN);
	_delay_us(15);
	SONIC_TRIG1_PORT &= ~(1<<SONIC_TRIG1_PIN);
}

ISR(TIMER1_CAPT_vect){
	if(TCCR1B & (1<<ICES1)){
		beginCount = ICR1;
		TCCR1B &= ~(1<<ICES1);
		goodReturn = 0;
	}
	else{
		endCount = ICR1;
		distance = (endCount - beginCount) / 58;
		goodReturn = 1;
		sendData(distance);
		TCCR1B |= (1<<ICES1);
		TIMSK1 &= ~(1<<TOIE1);
	}
}

ISR(TIMER1_OVF_vect){
	if(!goodReturn){
		sendData(SENSOR_TIMEOUT);
		TCNT1 = 0;
		TCCR1B |= (1<<ICES1);
		TIMSK1 &= ~(1<<TOIE1);
	}
}

ISR(USART_RX_vect){
	byteReceived = receiveByte();
	if(byteReceived == UART_TRIG_BYTE){
		TCNT1 = 0;
		generatePulse();
		TIMSK1 |= (1<<TOIE1);
	}
	else{
		sendData(UART_BAD_RECEIVE);
	}
}

int main(){
	LED_DDR |= (1<<LED_PIN);			//Set data direction out for LED pin
	SONIC_TRIG1_DDR |= (1<<SONIC_TRIG1_PIN);	//Set data direction out for SONIC_TRIG1 pin

	initUSART();
	initTimer1();
	sei();

	while(1){
		_delay_ms(750);
		LED_PORT ^= (1<<LED_PIN);
	}

	return(0);
}
