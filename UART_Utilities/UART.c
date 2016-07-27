/*
    John Talbot
    July 25, 2016
    
    PiUART is a set of functions allowing the AVR and RasPi to 
    communicate over UART. For use with GardenPi code

    Code modified from USART.h and USART.c files from Make: Programming
    AVR by Elliot Williams for use with GardenPi package

    License included in UART_Utilities file
*/

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "GardenPiUARTBytes.h"

//---------------------------------------------------------------
// INITIALIZATION
//---------------------------------------------------------------
/*
    This function will initialize UART communications on the AVR
    using speeds calculated by setbaud.h and using 8 data bits 
    with 1 stop bit
*/
void initUART(void) {
    // Values defined in setbaud.h
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Use double speed Tx/Rx if flag is set in setbaud.h
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
  
    // Enable UART transmitter/receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Enable UART receive interrupt in register if flag enabled
    // in UART.h header file
    if(ENABLE_RX_INTERRUPT){
        UCSR0B |= (1 << RXCIE0);
    }
    else{
        UCSR0B &= ~(1 << RXCIE0);
    }

    // 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

//--------------------------------------------------------------
// LOW LEVEL Tx/Rx FUNCTIONS
//--------------------------------------------------------------
//This function will transmit one byte of data over UART
void transmitByte(uint8_t data) {
    // Loop until transmit data register is empty
    while(!(UCSR0A & (1 << UDRE0))){}

    // Write byte to transmit data register to send
    UDR0 = data;
}

// This function will receive one byte of data over UART
uint8_t receiveByte(void) {
    // Loop until receive data register has data
    while(!(UCSR0A & (1 << RXC0))){}

    // Read data from receive register
    return UDR0;
}

//--------------------------------------------------------------
// MID LEVEL MESSAGE FUNCTIONS
//--------------------------------------------------------------
// Handle UART communication error
void commError(void){
    transmitByte(UART_COMM_ERROR);
    transmitByte(END_MSG);

    // Flush receive buffer by temporarily disabling RXEN0
    UCSR0B &= ~(1 << RXEN0);
    _delay_us(25);
    UCSR0B |= (1 << RXEN0);
}

void transmitReady(void){
    transmitByte(AVR_READY);
    transmitByte(END_MSG);
}

void transmitRequest(void){
    transmitByte(AVR_REQ_RASPI);
    transmitByte(END_MSG);
}

void transmitConfirm(void){
    transmitByte(AVR_REC_MSG_CONFIRM);
    transmitByte(END_MSG);
}

void transmitInitialize(void){
    transmitByte(AVR_INIT_TO_RASPI);
    transmitByte(END_MSG);
}

//--------------------------------------------------------------
// HIGH LEVEL MESSAGE FUNCTIONS
//--------------------------------------------------------------
// This function will receive a full request message from a
// RasPi and return proper confirmation bytes
int * receiveMessage(void){
    // Hold inital byte received
    uint8_t i=0;
    uint8_t j=0;
    uint8_t n=0;

    // Array to hold received data
    static int msgArray[10];
    int dataByteIn[2];

    // Receive initial byte
    for(j=0; j<2; j++){
        dataByteIn[j] = receiveByte();
    }

    // If message is not properly terminated write error code
        // to entire array and call commError()
    if(dataByteIn[1] != END_MSG){
        for (i=0; i<4; i++){
            msgArray[i] = UART_COMM_ERROR;
        }
        commError();
    }

    // RasPi normal request message to AVR
    else if (dataByteIn[0] == RASPI_REQ_AVR){
        // Transmit AVR ready to receive message
        transmitReady();
        
        // Loop through until EOM reached and store in array
        do{
            msgArray[n] = receiveByte();
            n++;
        }while(msgArray[n-1] != END_MSG);
        transmitConfirm();
    }

    // RasPi initialize request to AVR
    else if(dataByteIn[0] == RASPI_INIT_TO_AVR){
	    // Transmit AVR initialized ready byte
        transmitInitialize();

        // Write initialized byte to array
        for(i=0; i<4; i++){
            msgArray[i] = AVR_INIT_TO_RASPI;
        }
    }

    // Improper initial communication byte, call commError()
    else{
        commError();
    }

    return msgArray;
}

// This function will transmit a full message to RasPi
uint8_t transmitMessage(volatile int sendData[4]){
    // Initialize loop counter
    uint8_t i;
    uint8_t badSendFlag = 0;

    // Clear global interrupt to stop receive interrupt
    cli();

    // Send AVR request RasPi byte
    transmitRequest();

    // Wait for response from RasPi
    // **** This has possibility of hanging program -- needs improvement!
    if (receiveByte() == RASPI_READY){
        // Remove EOM from buffer
        receiveByte();

        // Loop to send all of data array
        for(i=0; i<4; i++){
            transmitByte(sendData[i]);
        }

        // Transmit end of message byte
        transmitByte(END_MSG);

        // Check for good confirm byte
        if (receiveByte() == RASPI_REC_MSG_CONFIRM){
            // Remove EOM from buffer
            receiveByte();
        }
        else{
            // Set bad send flag
            badSendFlag = 2;
        }
    }

    // If improper response is received, call commError()
    else{
        // Set bad send flag
            badSendFlag = 1;
    }

    // If badSendFlag is set - call commError()
    if(badSendFlag != 0){
        commError();
    }

    // reenable global interrupts
    sei();

    // Return value of badSendFlag
    return badSendFlag;
}
