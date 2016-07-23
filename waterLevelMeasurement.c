/* 
John Talbot
July 20, 2016
This program measures distance using an HC-SR04 ultrasonic sensor and then transmits mesaurement over UART
*/

#ifndef F_CPU
#define F_CPU 				8000000UL
#endif

#define TRIGGER_PIN			PD4
#define TRIGGER_PORT 		PORTD
#define TRIGGER_DDR			DDRD

#define ECHO_PIN			PB0
#define ECHO_PORT			PORTB
#define ECHO_DDR			DDRB

#define LED_PIN				PC0
#define LED_PORT			PORTC
#define LED_DDR				DDRC

#define DIST_THRESH 		5

#define UART_TRIG_BYTE 		0x01
#define SENSOR_TIMEOUT		0xFFFFFFFF
#define UART_BAD_RECEIVE 	0xDDDDDDDD

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

volatile uint8_t byteReceived;
volatile uint16_t beginCount, endCount;
volatile uint32_t elapsedCounts;
volatile uint32_t distance;
volatile char goodReturn=1;

static inline void initTimer1(void){
	TCCR1B |= (1<<CS11) | (1<<ICES1);
	TIMSK1 |= (1<<ICIE1) | (1<<TOIE1) | (1<<ICNC1);
}

static void sendData(uint32_t dataToSend){
	printWord(dataToSend>>16);
	printWord(dataToSend);
	transmitByte(0x0D);
}

static inline void generatePulse(void){
	// Generate a 12us pulse to trigger the HR-SR04
	TRIGGER_PORT &= ~(1<<TRIGGER_PIN);
	_delay_us(15);
	TRIGGER_PORT |= (1<<TRIGGER_PIN);
	_delay_us(15);
	TRIGGER_PORT &= ~(1<<TRIGGER_PIN);
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
	}
}

ISR(TIMER1_OVF_vect){
	if(!goodReturn){
		sendData(SENSOR_TIMEOUT);
		TCNT1 = 0;
		TCCR1B |= (1<<ICES1);
	}
}

ISR(USART_RX_vect){
	byteReceived = receiveByte();
	if(byteReceived == UART_TRIG_BYTE){
		TCNT1 = 0;
		generatePulse();
	}
	else{
		sendData(UART_BAD_RECEIVE);
	}
}

int main(){
	LED_DDR |= (1<<LED_PIN);			//Set data direction out for LED pin
	TRIGGER_DDR |= (1<<TRIGGER_PIN);	//Set data direction out for TRIGGER pin

	initUSART();
	initTimer1();
	sei();

	while(1){
		_delay_ms(750);
		LED_PORT ^= (1<<LED_PIN);
	}

	return(0);
}
