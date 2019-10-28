#define F_CPU 16000000UL
#include <asf.h>
#include "vector.h"
#include "quaternion.h"

volatile uint16_t counter = 0;
volatile uint16_t counter2 = 0;
volatile uint8_t pwmlength = 0;
volatile uint16_t limit = 0;
volatile uint8_t rest = 0;

volatile uint16_t limit2 = 0;
volatile uint8_t rest2 = 0;

int main (void)
{
	//Power Source
	DDRB = 0xff;
	PORTB = 0xff;
	
	//Taster Input
	DDRA = 0x00;
	PINA = 0x00;
	
	//PWM Output
	DDRC = 0xff;
	
	//Initialisierung 50ms timer
	TCCR2A = (1<<WGM01);
	TCCR2B = (1<<CS20);
	TIMSK2 |= (1<<OCIE0A);
	OCR2A = 255;
	
	//Initialisierung variabler PWM-Timer
	TCCR0A = (1<<WGM01);
	TCCR0B = (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 255;
	sei();
	
	while (1) {
		uint16_t length = 100 + (100 * pwmlength / 255);
		uint16_t number = 160 * length;
		limit = number / 256;
		rest = number % 256;
		
		limit2 = number * 2 / 256;
		rest2 = number * 2 % 256;
		
		pwmlength = ~PINA;
	}
	return 0;
}

ISR( TIMER2_COMPA_vect )
{
	counter2++;
	OCR2A = 255;
	if (counter2 >= 1250)
	{		
		TCCR0B = (1<<CS00);
		counter2 = 0;
		PORTC = 0xFF;
	}
}

ISR( TIMER0_COMPA_vect )
{
	counter++;
	OCR0A = 255;
	
	//timer resetten aus if's auslagern und nur einmal ausführen nachdem alle ifs durchlaufen sind
	if (counter	> limit)
	{
		OCR0A = rest;
		counter = 0;
		PORTC ^= 1 << 0;
	}
	if (counter	> limit2)
	{
		OCR0A = rest2;
		counter = 0;
		PORTC ^= 1 << 1;
	}
	//sobald alle auf 0
	TCCR0B = (0<<CS00);
}
