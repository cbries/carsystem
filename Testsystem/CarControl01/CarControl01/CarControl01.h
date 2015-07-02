/*
 * CarControl01.h
 *
 * Created: 02.07.2015 13:41:47
 *  Author: Christian Benjamin Ries
 */ 

#ifndef _CARCONTROL01_H_
#define _CARCONTROL01_H_

#ifndef cbi
	#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
	#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

template<class T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

volatile unsigned long __timeCounterMsecs = 0;

inline unsigned long millis() { return __timeCounterMsecs; }


#endif /* _CARCONTROL01_H_ */