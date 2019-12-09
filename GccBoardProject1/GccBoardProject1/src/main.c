

#define F_CPU 16000000UL
#include <asf.h>
#include <math.h>
#include "hexapod.h"
#include "hexpod_constants.h"
#include "sort.h"
#include "acceleration.h"
#include "uart.h"
#include <util/delay.h>

struct LimitRest {
	uint16_t limit;
	uint8_t rest;
};

struct PinTimer {
	struct LimitRest limitRest;
	uint8_t pin;
};

struct LimitRest calculatePwm(uint8_t pwmLength, bool firstPWM);
void loadTimerValues(struct Quaternion absRot);
struct LimitRest calculateFirstPwm(uint8_t pwmlength);
float convertToAccel(uint8_t relAcc);

volatile uint16_t turnOffCounter = 0;
volatile uint16_t turnOnCounter = 0;
volatile uint8_t pinCounter = 0; //ZÃ¤hlt, welcher Wert in pinTimers als nÃ¤chstes gelesen wird

volatile uint8_t calcBool = 0;

volatile struct Vector trans = (struct Vector) {0.0f, 0.0f, 0.163f};

volatile struct PinTimer pinTimers[6];


ISR( TIMER2_COMPA_vect )
{
	turnOnCounter++;
	OCR2A = 249;
	if (turnOnCounter == 30)
	{
		calcBool = 0;
	}
	if (turnOnCounter >= 50)
	{
		//UCSR0B |= (0<<RXCIE0);//Disable Usart interrupt
		TCCR0B |= (1<<CS00);
		turnOnCounter = 0;
		PORTA = 0xff; // 0011 1111
	}
}

ISR( TIMER0_COMPA_vect )
{
	turnOffCounter++;
	OCR0A = 255;
	
	if (turnOffCounter	> pinTimers[pinCounter].limitRest.limit)
	{
		OCR0A = pinTimers[pinCounter].limitRest.rest;
		PORTA &= ~(1<<pinTimers[pinCounter].pin);
		
		turnOffCounter = 0;
		
		for (;pinTimers[pinCounter].limitRest.limit == pinTimers[pinCounter + 1].limitRest.limit && pinCounter < 5; pinCounter++)
		{
			PORTA &= ~(1<<pinTimers[pinCounter + 1].pin);
		}
		
		pinCounter++;
				
		if(pinCounter >= 5) {
			pinCounter = 0;
			PORTA = 0;
			TCCR0B = 0;
			calcBool = 1;
			//UCSR0B |= (1<<RXCIE0);//Enable Usart Interrupt
		}
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
	
	length = ((100 * pwmlength) / 255);
	
	if(firstPWM){
		length += 100;
	}
	int number = 160 * length;
	limit = number / 256;
	rest = number % 256;
	
	result.limit = limit;
	result.rest = rest;
	
	
	return result;
}

/*
* LÃ¤dt und sortiert die Werte, die das Abschalten der Servomotor Pins bestimmen.
*/
void loadTimerValues(struct Quaternion absRot) {
	static struct Pwmlength pwmlengths[6];
	float angles[6];
	calcMotorAngles(angles, trans, absRot);
	for (uint8_t i = 0; i < 6; i++)
	{
		float angle = angles[i];
		uint8_t pwm = (angle + M_PI) / (2.0f * M_PI) * 255;
		pwmlengths[i] = (struct Pwmlength) {pwm, i};
	}
	
	//Beispielwerte
	//struct Pwmlength pwmlengths[6] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}};
	for (short i = 1; i < 6; i = i+2)
	{
		pwmlengths[i].pwmlength = 255 - pwmlengths[i].pwmlength;
	}
	
	sort(pwmlengths);

	for (uint8_t i = 0; i < 6; i++) {
		uint8_t partialPwm = pwmlengths[i].pwmlength;
		if (i > 0) {
			partialPwm = partialPwm - pwmlengths[i - 1].pwmlength;
			pinTimers[i].limitRest = calculatePwm(partialPwm, false);
		}else {
			pinTimers[i].limitRest = calculatePwm(partialPwm, true);
		}
		
		pinTimers[i].pin = pwmlengths[i].pin;
	}

}

float convertToAccel(uint8_t relAcc)
{
	float quotient = (relAcc - 127.0f) / 256.0f;
	float baum = 2.0f * quotient;
	float nulle = baum * maxAcc;
	return nulle;	
}


int main (void)
{
	_delay_ms(100);
	
	cli();
	
	//uart_init(UART_BAUD_RATE);
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU) );
	
	//nicht benötigt
	////Power Source
	//DDRB = 0xff;
	//PORTB = 0xff;
	
	DDRC = 0xff;
	PORTC = 0x00;
	
	DDRA = 0xff;
	
	//Initialisierung 50ms timer
	TCCR2A = (1<<WGM01);
	TCCR2B = (1<<CS22);
	TIMSK2 |= (1<<OCIE2A);
	OCR2A = 249;
	
	//Initialisierung variabler PWM-Timer
	TCCR0A = (1<<WGM01);
	TCCR0B = (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 255;
	sei();

	initialize(0.55f, 0.24f, 0.215f, 0.055f, 0.45f, 0.45f);
	
	loadTimerValues((struct Quaternion) {1,0,0,0});
	
	while (1) {
		
		if(/*calcBool ==*/ 1 && /*uartConsistencyCheck() ==*/ 1)
		{
			uint8_t dataArray[6];
			uartGetData(dataArray);
			//_delay_ms(250);
			sendString(dataArray);
			clearReceiveBufferIfNecessary();
			//Konsitenzprüfung
			//float xAcc = convertToAccel(dataArray[3]);
			//float yAcc = convertToAccel(dataArray[4]);
			//loadTimerValues(accelerationsToAngles(xAcc, yAcc));

			calcBool = 0;
		}
	}
	return 0;
}