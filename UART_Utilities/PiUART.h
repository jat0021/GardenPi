/* Functions to initialize, send, receive over USART

   initUSART requires BAUD to be defined in order to calculate
     the bit-rate multiplier.
 */

#ifndef _UART_H_
#define _UART_H_  1

#ifndef BAUD                          /* if not defined in Makefile... */
#define BAUD  9600                     /* set a safe default baud rate */
#endif

/* Takes the defined BAUD and F_CPU,
   calculates the bit-clock multiplier,
   and configures the hardware USART                   */
void initUART(void);

/* Blocking transmit and receive functions.
   When you call receiveByte() your program will hang until
   data comes through.  We'll improve on this later. */
uint8_t transmitByte(uint8_t data);
uint8_t receiveByte(void);

#endif