/* 
John Talbot
July 20, 2016
This program measures distance using an HC-SR04 ultrasonic sensor and then transmits mesaurement over UART
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GardenPi.h"
#include "USART.h"

volatile uint8_t byteReceived;
volatile uint16_t beginCount, endCount;
volatile uint16_t elapsedCounts;
volatile uint16_t distance;

static inline void initTimer1(void){
	TCCR1B |= (1<<CS11) | (1<<ICES1);
	TIMSK1 |= (1<<ICIE1) | (1<<ICNC1);
}

static void sendData(uint16_t dataToSend){
	transmitByte(dataToSend >> 8);
	transmitByte(dataToSend);
	transmitByte(0x0A);
}

/* 
Function to trigger HC-SR04
	This function will trigger an HC-SR04 ultrasonic range sensor
	by generating a 15us HIGH pulse on the selected trigger pin
*/
static void triggerHCSR04(uint8_t trigPort, uint8_t trigPin){
	trigPort &= ~(1<<trigPin);
	_delay_us(15);
	trigPort |= (1<<trigPin);
	_delay_us(15);
	trigPort &= ~(1<<trigPin);
}

ISR(TIMER1_CAPT_vect){
	if(TCCR1B & (1<<ICES1)){
		beginCount = ICR1;
		TCCR1B &= ~(1<<ICES1);
	}
	else{
		endCount = ICR1;
		distance = (endCount - beginCount) / 58;
		sendData(distance);
		TCCR1B |= (1<<ICES1);
		TIMSK1 &= ~(1<<TOIE1);
	}
}

ISR(TIMER1_OVF_vect){
	sendData(SENSOR_TIMEOUT);
	TCNT1 = 0;
	TCCR1B |= (1<<ICES1);
	TIMSK1 &= ~(1<<TOIE1);	
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
