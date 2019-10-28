#define F_CPU 16000000UL
#include <asf.h>
#include "vector.h"
#include "quaternion.h"

struct LimitRest {
	uint16_t limit;
	uint8_t rest;
};

struct PinTimers {
	struct LimitRest limitRest;
	uint8_t port;
};

struct LimitRest calculatePwm(uint8_t pwmLength);
struct PinTimers loadTimerValues();

volatile uint16_t counter = 0;
volatile uint16_t counter2 = 0;
volatile uint8_t pwmlength = 0;

volatile struct LimitRest limitRest = {0, 0};
volatile struct LimitRest limitRest2 = {0, 0};

volatile struct PinTimers pinTimers[6] = NULL;

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
		limitRest = calculatePwm(pwmlength);
		limitRest2 = calculatePwm(pwmlength * 2);

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
		pinTimers = 
	}
}

ISR( TIMER0_COMPA_vect )
{
	counter++;
	OCR0A = 255;
	
	//timer resetten aus if's auslagern und nur einmal ausführen nachdem alle ifs durchlaufen sind
	if (counter	> limitRest.limit)
	{
		OCR0A = limitRest.rest;
		counter = 0;
		PORTC ^= 1 << 0;
	}
	if (counter	> limitRest2.limit)
	{
		OCR0A = limitRest2.rest;
		counter = 0;
		PORTC ^= 1 << 1;
	}
	//sobald alle auf 0
	TCCR0B = (0<<CS00);
}

struct LimitRest calculatePwm(uint8_t pwmLength) {
	struct LimitRest result;
	
	uint16_t length = 100 + (100 * pwmlength / 255);
	uint16_t number = 160 * length;
	result.limit = number / 256;
	result.rest = number % 256;
	
	return result;
}

struct PinTimers loadTimerValues() {
	//TODO
	struct PinTimers result;
	
	//dummy values
	
}