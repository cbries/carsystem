/*
 * CarControl02.cpp
 *
 * Created: 02.07.2015 17:58:51
 *  Author: Christian Benjamin Ries
 */ 

#include <avr/interrupt.h>

volatile unsigned long __msecs = 0;

ISR(TIM0_OVF_vect) 
{
	static int timer_overflow_count = 0;
	if (++timer_overflow_count > 4) 
	{   
		++__msecs;
	
		timer_overflow_count = 0;
	}
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
	TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0A |= (1 << COM0B1);
	TIMSK0 |= 1<<TOIE0;
	sei();

	unsigned long wallTimeBlink = 0;
	unsigned long wallTimeEngine = 0;
	bool decrement = true;
	uint8_t v = 255;

	while(1) 
	{
		if(wallTimeBlink < __msecs)
		{
			wallTimeBlink = __msecs + 100;

			PORTB ^= 1<<PB4;			
		}
		
		if(wallTimeEngine < __msecs)
		{
			wallTimeEngine = __msecs + 5;
			
			if(decrement)
			{
				if(--v <= 0)
					decrement = false;
			}
			else
			{
				if(++v >= 255)
					decrement = true;
			}
				
			OCR0B = v;
		}
	}
}