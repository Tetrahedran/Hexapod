#define F_CPU 16000000UL
#include <asf.h>
#include <math.h>
#include "hexapod.h"
#include "hexpod_constants.h"
#include "sort.h"

struct LimitRest {
	uint16_t limit;
	uint8_t rest;
};

struct PinTimer {
	struct LimitRest limitRest;
	uint8_t pin;
};

struct LimitRest calculatePwm(uint8_t pwmLength, bool firstPWM);
struct PinTimer* loadTimerValues(void);
struct LimitRest calculateFirstPwm(uint8_t pwmlength);

volatile uint16_t turnOffCounter = 0;
volatile uint16_t turnOnCounter = 0;
volatile uint8_t pinCounter = 0; //Zählt, welcher Wert in pinTimers als nächstes gelesen wird
volatile struct Vector trans = (struct Vector) {0.0f, 0.0f, 0.0f};

volatile struct PinTimer pinTimers[6];

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
	PORTC = 0xff;
	
	//Initialisierung 50ms timer
	TCCR2A = (1<<WGM01);
	TCCR2B = (1<<CS20);
	TIMSK2 |= (1<<OCIE2A);
	OCR2A = 255;
	
	//Initialisierung variabler PWM-Timer
	TCCR0A = (1<<WGM01);
	TCCR0B = (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 255;
	sei();
	
	initialize(0.25f, 0.23f, 0.0925f, 0.015f, 0.425f, 0.39f);
	
	struct PinTimer *pTimerVals = loadTimerValues();
	for(uint8_t x = 0; x < 6; x++) {
		pinTimers[x] = *(pTimerVals + x);
	}
	
	while (1) {
		struct PinTimer *pTimerVals = loadTimerValues();
		for(uint8_t x = 0; x < 6; x++) {
			pinTimers[x] = *(pTimerVals + x);
		}	
	}
	return 0;
}

ISR( TIMER2_COMPA_vect )
{
	turnOnCounter++;
	OCR2A = 255;
	if (turnOnCounter >= 3125)
	{
		TCCR0B |= (1<<CS00);
		turnOnCounter = 0;
		PORTB = 0x3f; // 0011 1111
	}
}

ISR( TIMER0_COMPA_vect )
{
	turnOffCounter++;
	OCR0A = 255;
	
	if (turnOffCounter	> pinTimers[pinCounter].limitRest.limit)
	if(turnOffCounter > 125)
	{
		//OCR0A = pinTimers[pinCounter].limitRest.rest;
		//PORTB &= ~(1<<pinTimers[pinCounter].pin);
		PORTB = 0;
		
		turnOffCounter = 0;
		//if(pinCounter >= 5) {
			//pinCounter = 0;
			//PORTB = 0;
			TCCR0B = 0;
		//} else {
			//pinCounter++;
		//}
	}
}

/*
* Berechnet die Werte limit (Zahl der Timerzyklen) und rest (Wert des letzten Zyklus) anhand eines PWM-Werts.
*/
struct LimitRest calculatePwm(uint8_t pwmlength, bool firstPWM) {
	struct LimitRest result;
	uint16_t limit;
	uint8_t rest;
	int length = 0;
	
	if(!firstPWM){
		length = 100 + ((100 * pwmlength) / 255);
		}else{
		length = ((100 * pwmlength) / 255);
	}
	int number = 160 * length;
	limit = number / 256;
	rest = number % 256;
	
	result.limit = limit;
	result.rest = rest;
	
	return result;
}

/*
* Lädt und sortiert die Werte, die das Abschalten der Servomotor Pins bestimmen.
*/
struct PinTimer* loadTimerValues(void) {
	static struct PinTimer result[6];
	static struct Pwmlength pwmlengths[6];
	//trans.z += 0.01f;
	float* angles = calcMotorAngles(trans, (struct Quaternion) {1.0f, 0.0f, 0.0f, 0.0f});
	for (short i = 0; i < 6; i++)
	{
		float angle = *(angles + i);
		uint8_t pwm = (angle + M_PI) / (2.0f * M_PI) * 255;
		pwmlengths[i] = (struct Pwmlength) {pwm, i};
	}
	
	//Beispielwerte
	//struct Pwmlength pwmlengths[6] = {{240, 0}, {55, 1}, {80, 2}, {100, 3}, {10, 4}, {100, 5}};
	
	struct Pwmlength *pPwm = sort(pwmlengths);
	
	for(uint8_t x = 0; x < 6; x++) {
		pwmlengths[x] = *(pPwm + x);
	}
	
	for (uint8_t i = 0; i < 6; i++) {
		uint8_t partialPwm = pwmlengths[i].pwmlength;
		
		if (i > 0) {
			partialPwm = partialPwm - pwmlengths[i - 1].pwmlength;
			result[i].limitRest = calculatePwm(partialPwm, false);
		}else{
			result[i].limitRest = calculatePwm(partialPwm, true);
		}
		
		result[i].pin = pwmlengths[i].pin;
	}
	
	return result;
}