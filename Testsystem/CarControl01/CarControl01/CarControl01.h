/*
 * CarControl01.h
 *
 * Created: 02.07.2015 13:41:47
 *  Author: Christian Benjamin Ries
 */ 

#ifndef _CARCONTROL01_H_
#define _CARCONTROL01_H_

#include <util/atomic.h>

#ifndef cbi
	#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
	#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile unsigned long __timeCounterMsecs = 0;

ISR (TIM0_COMPA_vect)
{
	// ignore
}

/*
 * Datasheet ATTiny13A, page 44
 */
ISR (TIM0_OVF_vect)
{
	static uint8_t overflowCounter = 0;

	if(++overflowCounter >= 4)
	{
		__timeCounterMsecs++;

		overflowCounter = 0;		
	}
}

template<class T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline unsigned long millis() 
{
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = __timeCounterMsecs;
	}
	return millis_return;
}

#endif /* _CARCONTROL01_H_ */