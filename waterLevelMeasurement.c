/* 
John Talbot
July 20, 2016
This program measures distance using an HC-SR04 ultrasonic sensor and then transmits mesaurement over UART
*/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define TRIGGER_PIN		PD4
#define TRIGGER_PORT 	PORTD
#define TRIGGER_DDR		DDRD

#define ECHO_PIN		PD2
#define ECHO_PORT		PORTD
#define ECHO_DDR		DDRD

#define LED_PIN			PB0
#define LED_PORT		PORTB
#define LED_DDR			DDRB

#define DIST_THRESH 	5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

volatile uint16_t elapsedCounts;
volatile uint32_t distance=10;

static inline void initTimer1(void){
	TCCR1B |= (1<<CS11);
	TIMSK1 |= (1<<TOIE1);
}

ISR(INT0_vect){
	if (PIND & (1<<ECHO_PIN)) {
		TCNT1 = 0;
	}
	else{
		elapsedCounts = TCNT1;
		distance = elapsedCounts / 58;
	}
}

ISR(TIMER1_OVF_vect){
	LED_PORT |= (1<<LED_PIN);
	_delay_ms(100);
	LED_PORT &= ~(1<<LED_PIN);



	// Generate a 12us pulse to trigger the HR-SR04
	TRIGGER_PORT &= ~(1<<TRIGGER_PIN);
	_delay_us(15);
	TRIGGER_PORT |= (1<<TRIGGER_PIN);
	_delay_us(15);
	TRIGGER_PORT &= ~(1<<TRIGGER_PIN);

	TCNT1 = 0;
}



int main(){
	LED_DDR |= (1<<LED_PIN);			//Set data direction out for LED pin
	TRIGGER_DDR |= (1<<TRIGGER_PIN);	//Set data direction out for TRIGGER pin

	initTimer1();
	sei();

	while(1){
		/*
		if (distance < DIST_THRESH){
			LED_PORT |= (1<<LED_PIN);
		}
		else {
			LED_PORT &= ~(1<<LED_PIN);
		}
		*/
	}
}