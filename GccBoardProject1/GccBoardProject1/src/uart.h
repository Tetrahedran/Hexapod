/*** uart.h ***/
#ifndef uart_h
#define uart_h

#include <avr/io.h>
//#include "globals.h"

#define BAUD 9600UL          // Baudrate

#ifndef F_CPU
#warning "F_CPU is still not defined, now is set with 20000000UL"
#define F_CPU 16000000UL    // Systemclock in Hz
//#define F_CPU 3686400UL    // Systemclock in Hz
// attention on definition as unsigned long
// without UL an error occurs during the calculation.
#endif
 
// Calculation
//#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // smart rounding
#define UBRR_VAL ((F_CPU)/(BAUD*16)-1)   // smart rounding

#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Real Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)      
//error in promille, 1000 = no error

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
#error systematic error in baudrate is >1% and therefore to high
#endif

#define USART_PORT DDRD   // usart port
#define USART_RX   DDD0   // usart rx pin
#define USART_TX   DDD1   // usart tx pin

extern volatile uint8_t debugMessageFlag; //0
extern volatile uint8_t systemMessageFlag; //1
extern volatile uint8_t blockReceivingUSARTchar_Flag;

void init_uart(void);
void uart_putc(unsigned char c);
char USART0_Receive(void);
void USART0_SetupInterrupts(void);
void sendString (char stringChar[]);
void sendIntUart(int n);
int my_strlen(char *str);
void sendConvertDecInHex(int decimal);
void sendConvertDecInBin(int decimal);


void errorMessage(char errorMessageText[]);
void debugMessage(char debugMessageText[]);
void systemMessage(char systemMessageText[]);


//intToString and Reverse
#endif // uart_h