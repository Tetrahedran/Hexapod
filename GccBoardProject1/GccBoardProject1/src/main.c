#define F_CPU 2000000UL
#include <asf.h>

volatile uint16_t counter = 0;
volatile uint16_t counter2 = 0;
volatile uint8_t pwmlength = 0;
volatile uint16_t limit = 0;
volatile uint8_t rest = 0;

int main (void)
{
	DDRB = 0xff;
	PORTB = 0x00;
	//DDRD = 0xff;
	DDRA = 0x00;
	PINA = 0x00;
	DDRC = 0xff;
	TCCR2A = (1<<WGM01);
	TCCR2B = (1<<CS20);
	TIMSK2 |= (1<<OCIE2A);
	OCR2A = 255;
	TCCR0A = (1<<WGM01);
	TCCR0B = (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 255;
	sei();
	while (1) {
		pwmlength = ~PINA;
		uint16_t length = 100 + (100 * pwmlength / 255);
		uint16_t number = 160 * length;
		limit = number / 256;
		rest = number % 256;
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
		PORTB = ~PORTB;
		//PORTD = ~PORTD;
	}
}

ISR( TIMER0_COMPA_vect )
{
	counter++;
	OCR0A = 255;
	if (counter	> limit)
	{
		OCR0A = rest;
		counter = 0;
		PORTB = ~PORTB;
		//PORTD = ~PORTD;
		TCCR0B = (0<<CS00);
	}
}
