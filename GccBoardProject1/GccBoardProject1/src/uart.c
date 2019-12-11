/*** uart.c ***/

#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include <string.h>
#include <math.h>


/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif

 /* ATmega with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART0_STATUS      UCSR0A
 #define UART0_CONTROL     UCSR0B
 #define UART0_CONTROLC    UCSR0C
 #define UART0_DATA        UDR0
 #define UART0_UDRIE       UDRIE0
 #define UART0_UBRRL       UBRR0L
 #define UART0_UBRRH       UBRR0H
 #define UART0_BIT_U2X     U2X0
 #define UART0_BIT_RXCIE   RXCIE0
 #define UART0_BIT_RXEN    RXEN0
 #define UART0_BIT_TXEN    TXEN0
 #define UART0_BIT_UCSZ0   UCSZ00
 #define UART0_BIT_UCSZ1   UCSZ01

/*
 *  module global variables
 */
static volatile unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static volatile unsigned char UART_LastRxError;


ISR (UART0_RECEIVE_INTERRUPT)	
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;
 
 
    /* read UART status register and UART data register */
    usr  = UART0_STATUS;	//UCSR0A
    data = UART0_DATA;		//UDR0
    
    /* get FEn (Frame Error) DORn (Data OverRun) UPEn (USART Parity Error) bits */
#if defined(FE) && defined(DOR) && defined(UPE)
    lastRxError = usr & (_BV(FE)|_BV(DOR)|_BV(UPE) );
#elif defined(FE0) && defined(DOR0) && defined(UPE0) //this is the register for ATMEGA644PA - UART0
    lastRxError = usr & (_BV(FE0)|_BV(DOR0)|_BV(UPE0) );
#elif defined(FE1) && defined(DOR1) && defined(UPE1)
    lastRxError = usr & (_BV(FE1)|_BV(DOR1)|_BV(UPE1) );
#elif defined(FE) && defined(DOR)
    lastRxError = usr & (_BV(FE)|_BV(DOR) );
#endif

    /* calculate buffer index */ 
    tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK; //UART_RX_BUFFER_SIZE - 1 => 31
    
    if ( tmphead == UART_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    }else{
        /* store new index */
        UART_RxHead = tmphead;
        /* store received data in buffer */
        UART_RxBuf[tmphead] = data;
    }
    UART_LastRxError |= lastRxError;   
	//in main.c/generalCatchEvents() ?C polls for characters
}


ISR (UART0_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
{
    unsigned char tmptail;

    
    if ( UART_TxHead != UART_TxTail) {
        /* calculate and store new buffer index */
        tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmptail;
        /* get one byte from buffer and write it to UART */
        UART0_DATA = UART_TxBuf[tmptail];  /* start transmission */
    }else{
        /* tx buffer empty, disable UDRE interrupt */
        UART0_CONTROL &= ~_BV(UART0_UDRIE);
    }
}


/*************************************************************************
Function: uart_init()
Purpose:  initialize UART and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart_init(unsigned int baudrate)
{
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;

#ifdef UART_TEST
#ifndef UART0_BIT_U2X
#warning "UART0_BIT_U2X not defined"
#endif
#ifndef UART0_UBRRH
#warning "UART0_UBRRH not defined"
#endif
#ifndef UART0_CONTROLC
#warning "UART0_CONTROLC not defined"
#endif
#if defined(URSEL) || defined(URSEL0)
#ifndef UART0_BIT_URSEL
#warning "UART0_BIT_URSEL not defined"
#endif
#endif
#endif

    /* Set baud rate */
    if ( baudrate & 0x8000 )
    {
        #if UART0_BIT_U2X
        UART0_STATUS = (1<<UART0_BIT_U2X);  //Enable 2x speed 
        #endif
    } 
    #if defined(UART0_UBRRH)
    UART0_UBRRH = (unsigned char)((baudrate>>8)&0x80) ;
    #endif    
    UART0_UBRRL = (unsigned char) (baudrate&0x00FF);
      
    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(UART0_BIT_RXCIE)|(1<<UART0_BIT_RXEN)|(1<<UART0_BIT_TXEN);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    #ifdef UART0_CONTROLC
    #ifdef UART0_BIT_URSEL
    UART0_CONTROLC = (1<<UART0_BIT_URSEL)|(1<<UART0_BIT_UCSZ1)|(1<<UART0_BIT_UCSZ0);
    #else
    UART0_CONTROLC = (1<<UART0_BIT_UCSZ1)|(1<<UART0_BIT_UCSZ0);
    #endif 
    #endif

}/* uart_init */


/*************************************************************************
Function: uart_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart_getc(void)
{    
    unsigned char tmptail;
    unsigned char data;
    unsigned char lastRxError;


    if ( UART_RxHead == UART_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate buffer index */
    tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
    
    /* get data from receive buffer */
    data = UART_RxBuf[tmptail];
    lastRxError = UART_LastRxError;
    
    /* store buffer index */
    UART_RxTail = tmptail; 
    
    UART_LastRxError = 0;
    return (lastRxError << 8) + data;

}/* uart_getc */


/*************************************************************************
Function: uart_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART_TxBuf[tmphead] = data;
    UART_TxHead = tmphead;

    /* enable UDRE interrupt */
    UART0_CONTROL    |= _BV(UART0_UDRIE);

}/* uart_putc */


/*************************************************************************
Function: uart_puts()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart_puts(const char *s )
{
    while (*s) 
      uart_putc(*s++);

}/* uart_puts */


//This function prints a string
void sendString(char tempStringChar[]){
	uart_puts(tempStringChar);
}

/*************************************************************************
Function: uart_puts_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void uart_puts_p(const char *progmem_s )
{
    register char c;
    
    while ( (c = pgm_read_byte(progmem_s++)) ) 
      uart_putc(c);

}/* uart_puts_p */

/* reverse:  reverse string s in place */
void reverse(char s[]){
	int i, j;
	char c;
	
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* itoa:  convert n to characters in s */
void intToString(int n, char s[]) {
	
	int i, sign;
	
	if ((sign = n) < 0)  /* record sign */
	n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
		if (sign < 0)
		s[i++] = '-';
		s[i] = '\0';
		reverse(s);
}


/* itoa:  convert n to characters in s */
void longToString(unsigned int long n, char s[]){
	unsigned long int i, sign;
	
	if ((sign = n) < 0){	// record sign
		n = -n;				// make n positive
	}
	i = 0;
	do {// generate digits in reverse order
		s[i++] = n % 10 + '0';	//get next digit
	} while ((n /= 10) > 0);	//delete it
	
	if (sign < 0){
		s[i++] = '-';
	}
	s[i] = '\0';
	reverse(s);
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint){
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of fraction part up to given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d){
	
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverseOverLen(str, i);
	str[i] = '\0';
	return i;
}

// reverses a string 'str' of length 'len'
void reverseOverLen(char *str, int len){
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

uint8_t dynamicBufferSize(void) 
{
	unsigned char tempHead = 0;
	tempHead = (UART_RxHead - UART_RxTail) & UART_RX_BUFFER_MASK;
	if (tempHead > 0)
	{
		PORTC = tempHead;
	}
	return tempHead;
}

uint8_t uartConsistencyCheck(void)
{
	uint8_t tailElement = (UART_RxTail + 1) & UART_RX_BUFFER_MASK; 
	if (dynamicBufferSize()<8)
	{
		return 0;
	}
	else if (UART_RxBuf[tailElement] == 'S')
	{
		for (int i = 1; i < 8; i++)
		{
			if ((tailElement + i) & UART_RX_BUFFER_MASK == UART_RxHead)
			{
				return 0;
			} else
			{
				char c = UART_RxBuf[(tailElement + i) & UART_RX_BUFFER_MASK];
				if (c == 'E' && i == 7)
				{
					return 1;
				}
			}
		}
		UART_RxTail = tailElement;
		return uartConsistencyCheck();
	} else
	{
		for (int i = 1; ((tailElement + i) & UART_RX_BUFFER_MASK) != ((UART_RxHead + 1) & UART_RX_BUFFER_MASK); i++)
		{
			char c = UART_RxBuf[(tailElement + i) & UART_RX_BUFFER_MASK];
			if (c == 'S')
			{
				UART_RxTail = (tailElement + i - 1) & UART_RX_BUFFER_MASK;
				return uartConsistencyCheck();
			}
		}
		UART_RxTail = UART_RxHead;
		return 0;
	}		
	return 0;
}

void uartGetData(uint8_t data[])
{
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 || i == 7)
		{
			uart_getc();
		} else
		{			
			data[i - 1] = uart_getc();
		}
	}
}



void clearReceiveBufferIfNecessary(void) 
{
	if (dynamicBufferSize() > UART_RX_BUFFER_SIZE / 2)
	{
		UART_RxTail = (UART_RxHead - 15) & UART_RX_BUFFER_MASK;
	}
}

/* Datasheet 21.8.7./p.235 Flushing the Receive Buffer:
	The receiver buffer FIFO will be flushed when the Receiver is disabled, i.e., the buffer will be emptied of
	its contents. Unread data will be lost. If the buffer has to be flushed during normal operation, due to for
	instance an error condition, read the UDRn I/O location until the RXCn Flag is cleared.
*/

void UART_Flush( void ){ 
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}