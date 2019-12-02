/*** uart.c ***/

#include "uart.h"
//#include "protocol.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

volatile uint8_t blockReceivingUSARTchar_Flag = 0;
volatile uint8_t uartStringReadyToTransformFlag;
volatile uint8_t uartStringReadyToRead = 1;
volatile unsigned char uartReceivedChar; 
volatile uint8_t charIsOk;
volatile uint8_t receivedDataCounter;
volatile unsigned char tempUartString[8];
volatile unsigned char dataUartString[6]; //pitch, roll, yaw, long, lateral, vertical
volatile unsigned char startUartString[6] = {177,177,177,177,177,177};

void init_uart(void){
	memcpy(dataUartString, startUartString, sizeof(dataUartString));
	//Set Baudrate
	UBRR0H = (unsigned char)(UBRR_VAL>>8);
	UBRR0L = (unsigned char)(UBRR_VAL);
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);// | (1<<RXCIE0);      // tx & rx enable
	UCSR0B |= (1<<RXCIE0); //interrupt enable
	UCSR0A |= (1<<RXC0);
	/* Asynchronous USART, Data bits = 8, Parity = none, Stop bits = 1 */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);    // Asynchron 8N1
	
	/*  Override general io pins for usart rx/tx */
	USART_PORT &= ~_BV(USART_RX);
	USART_PORT |= _BV(USART_TX);
}


void uart_putc(unsigned char c)
{
	//unsigned char tmphead;	
	
	// wait for UART to get ready
	while (!(UCSR0A & (1<<UDRE0))){
		/* wait until sending is possible */
	}
	// save into buffer -> send
	UDR0 = c;
}

// Wait until a byte has been received and return received data
// char USART0_Receive(void)
// {
// 	// wait for data from USART to be received
// 	while (!(UCSR0A & (1 << RXC0))) {
// 		
// 	}
// 	
// 	// get and return buffer data
// 	char rChar = UDR0;
// 	
// 	return rChar;
// }

//This function is used to read the available data
//from USART. This function will wait until data is
//available.
char USARTReadChar(){
	//Wait until a data is available

	while(!(UCSR0A & (1<<RXC0))){
		//Do nothing
	}

	//Now USART has got data from host
	//and is available is buffer
	return UDR0;
}


char readCharIfAvailible(){
	//Wait until a data is available

	if(!(UCSR0A & (1<<RXC0))){	
		return UDR0;
	}else{
		return (int) NULL;
	}

	//Now USART has got data from host
	//and is available is buffer	
}

// uint8_t USART_ReceiveByte(){
//	while((UCSRA &(1<<RXC)) == 0);
//	return UDR;
//}

void USART0_SetupInterrupts(void)
{
	// USART
	UCSR0B |= (1 << RXCIE0) | (1 << TXEN0) | (1 << RXEN0);
	
	// Puffer leeren
	do {
		UDR0;
	} while(UCSR0A & (1 << RXC0));
	// **********************
}

/**
 * Interrupt routine for Usart0
 * Interrupt routine for Usart0 - If something comes over Bluetooth then this event is triggered
 * @param Interrupt event
 * @return none
 */
ISR(USART0_RX_vect) {
		
	if (blockReceivingUSARTchar_Flag == 0) {
		//Main-workload: read char from register
		uartReceivedChar = USARTReadChar(); //Check Datasheet: is a new char throwing away if no new one is accepted?
		
		readReceivedChar(); 
		if (receivedDataCounter >= 7){ //String is ready to be transformed
			//PORTC = ~PORTC;
			uartStringReadyToTransformFlag = 1; //String can now be checked in switch routine
			blockReceivingUSARTchar_Flag = 1;
			transformUartString();
		}//end:
	}//end:
}

void readReceivedChar()
{
	charIsOk = checkReceivedChar(uartReceivedChar, receivedDataCounter);
	
	if (charIsOk == 1)
	{
		tempUartString[receivedDataCounter] = uartReceivedChar;
		receivedDataCounter++;
	}
	else 
	{
		receivedDataCounter = 0;
	}
}

uint8_t checkReceivedChar(char tempChar, int tempCounter)
{
	if (tempCounter == 0)
	{
		if (tempChar=='S')
		{
			return 1;
		}
		else 
		{ 
			return 0;
		}
	} else if ((tempCounter>0)&&(tempCounter<7))
	{
		return 1;
	} else if (tempCounter==7)
	{
		if (tempChar=='E')
		{
			return 1;
		} else 
		{
			return 0;
		}
	} else {return 0;}	
}

void transformUartString()
{
	if (uartStringReadyToTransformFlag == 1)
	{	
		uartStringReadyToRead = 0;
		for (int i=1; i<7; i++)
		{
			dataUartString[i-1] = tempUartString[i];
		}
		receivedDataCounter = 0;
		uartStringReadyToRead = 1;
		uartStringReadyToTransformFlag = 0;
		blockReceivingUSARTchar_Flag = 0;
	}
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
	int i, j;
	char c;
	
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* itoa:  convert n to characters in s */
void intToString(int n, char s[])
{
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
	void longToString(unsigned int long n, char s[])
	{
	unsigned long int i, sign;
	
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
	
	
void sendIntUart(int n){
	
	uint8_t maxAmountNumbers=10;
	
	char valueChar[10];
	intToString(n, valueChar); // defined in Header file
	
	for(int i=0;i<maxAmountNumbers;i++){
		if (valueChar[i] ==',' || valueChar[i] =='.' ||valueChar[i] =='1' || valueChar[i] =='2'||valueChar[i] =='3'||valueChar[i] =='4'||valueChar[i] =='5'||valueChar[i] =='6'||valueChar[i] =='7'||valueChar[i] =='8'||valueChar[i] =='9'||valueChar[i] =='0'||valueChar[i] =='-'){
		uart_putc(valueChar[i]);
		}
		else {
			i=maxAmountNumbers;
		}//end else
	}//for (int i=...
}

void sendLongUart(unsigned long int n){
	
	uint8_t maxAmountNumbers=12;
	
	char valueChar[12];
	longToString(n, valueChar); // defined in Header file
	
	for(int i=0;i<maxAmountNumbers;i++){
		if (valueChar[i] ==',' || valueChar[i] =='.' || valueChar[i] =='1' || valueChar[i] =='2'||valueChar[i] =='3'||valueChar[i] =='4'||valueChar[i] =='5'||valueChar[i] =='6'||valueChar[i] =='7'||valueChar[i] =='8'||valueChar[i] =='9'||valueChar[i] =='0'||valueChar[i] =='-'){
			uart_putc(valueChar[i]);
		}
		else {
			i=maxAmountNumbers;
		}//end else
	}//for (int i=...
}




//This function prints a string
void sendString(char tempStringChar[]){
	
	//uint8_t maxAmountLetters=255;//strlen(stringChar);//255;
	
//	for(int i=0;i<maxAmountLetters;i++){
		
	for(int i=0;i<my_strlen(tempStringChar);i++){
		//if (tempStringChar[i]!=NULL){//test if string is finished
			uart_putc(tempStringChar[i]);
		//}
		//else {i=maxAmountLetters;}
	}
	//free(tempStringChar);
}
	
int my_strlen(char *str)
{
	int i;
	for (i=0; str[i];i++);
	return i;
		
}


void sendConvertDecInHex(int decimal){
	long int quotient;
	int i=1, j, temp;
	char hexadecimalNumber[10];
	
	quotient = decimal; //set the input as quotient
	
	while(quotient != 0){
		temp = quotient % 16;

		//To convert integer into character
		if( temp < 10){
			temp = temp + 48; //in the ASCII-range the numbers starts at 48 until 57
		}else{
			temp = temp + 55; //the character A starts in the ASCII-range at 65 (55 + 10 => A)
		}
		hexadecimalNumber[i++]= temp; //put the result in the char array
		quotient = quotient / 16;
	}
	for(j = i-1; j > 0; j--){
		uart_putc(hexadecimalNumber[j]);
	}
	//in case of '0' in the given decimal variable
	if (decimal == 0){
		uart_putc(32); //empty space	
		uart_putc(48); //ASCII for 0
	}
}

void sendHex(uint8_t hexvalue){
	char hex; 
	sprintf(hex, "Hex: %x\n",hexvalue);
	sendString(hex);
}

void sendConvertDecInBin(int decimal){
	long int quotient;
	int binaryNumber[10],i=1,j;

	quotient = decimal;

	while(quotient!=0){
		binaryNumber[i++]= (quotient % 2) + 48;
		quotient = quotient / 2;
	}
	for(j = i -1 ;j> 0;j--){
		uart_putc(binaryNumber[j]);
	}
	//in case of '0' in the given decimal variable
	if (decimal == 0){
		uart_putc(48); //ASCII for 0
	}
}

void errorMessage(char errorMessageText[]){
	sendString(errorMessageText);
	sendString("\n");
}

uint8_t volatile debugMessageFlag = 0;
uint8_t volatile systemMessageFlag = 1;

void debugMessage(char debugMessageText[]){
	if(debugMessageFlag==1){
		sendString(debugMessageText);
		sendString("\n");
	}
}

void systemMessage(char systemMessageText[]){
	if(systemMessageFlag==1){
		sendString(systemMessageText);
		sendString("\n");
	}
}