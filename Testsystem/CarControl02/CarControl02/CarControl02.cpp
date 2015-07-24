/*
 * CarControl02.cpp
 *
 * Created: 02.07.2015 17:58:51
 *  Author: Christian Benjamin Ries
 */ 

#include <avr/interrupt.h>
#include <util/atomic.h>

/*

PB0 = Frontlicht
PB4 = Rücklicht

PB1 = Blinker rechts
PB5 = Blinker links

PB2 = IR Receiver ?
PB3 = Engine

 */

/*
	Fcpu 1.2Mhz

	Prescaler:
		1 => 4,687Hz
		8 => 585Hz
		64 => 73Hz
		256 => 18Hz

	Fcpu 9.6Mhz

	Prescaler:
		1 => 37,500Hz
		8 => 4,687Hz
		64 => 585Hz
		256 => 146Hz
*/
#define F_CPU 9600000UL#
#define PIN_ENGINE PB3

inline void toggleBlinker()
{
	PORTB ^= (1 << PB1);	// Blinker rechts
	PORTB ^= (1 << PB5);		
}

inline void toggleLight(bool state)
{
	// PB0 := Frontlicht
	// PB4 := Rücklicht
	if(state)
	{		
		PORTB |= (1 << PB0);
		PORTB |= (1 << PB4);		
	}
	else
	{
		PORTB &= ~(1 << PB0);
		PORTB &= ~(1 << PB4);
	}
}

inline void toggleEngine(bool state)
{
	if(state)
		PORTB |= (1 << PIN_ENGINE);
	else
		PORTB &= ~(1 << PIN_ENGINE);
}

volatile unsigned long __timeCounterMsecs = 0;

inline unsigned long millis()
{
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = __timeCounterMsecs;
	}
	return millis_return;
}

ISR(TIM0_COMPA_vect)
{
	static uint8_t counter = 0;
	
	if(++counter >= 5)
	{
		++__timeCounterMsecs;

		counter = 0;
	}	
}

template<class T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void) 
{
	DDRB = (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);

	/*
		CS02 CS01 CS00 Description
		 0    0    0   No clock source (Timer/Counter stopped)
		 0    0    1   clkI/O/(No prescaling)
		 0    1    0   clkI/O/8 (From prescaler)
		 0    1    1   clkI/O/64 (From prescaler)
		 1    0    0   clkI/O/256 (From prescaler)
		 1    0    1   clkI/O/1024 (From prescaler)
	*/	

	OCR0A = 240;
	TCNT0 = 0;
	TIMSK0 = TIMSK0 | (1<<OCIE0A); 	//enable compare match interrupt
	//TIMSK = TIMSK | (1<<OCIE0A); 	//enable compare match interrupt
	TCCR0A = (1<<WGM01); 	        //CTC
	TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);

	sei();

	unsigned long walltime = 0;

	toggleLight(true);
	//toggleEngine(true);

	for(;;) 
	{
		unsigned long currentMillis = millis();
		if(walltime < currentMillis)
		{
			walltime = currentMillis + 50;

			toggleBlinker();			
		}
	}
}