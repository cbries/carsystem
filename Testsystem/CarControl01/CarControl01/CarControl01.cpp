/*
 * CarControl01.cpp
 *
 * Created: 02.07.2015 10:35:10
 *  Author: Christian Benjamin Ries
 */ 

#define F_CPU 9600000
#define PIN_ENGINE PB1
#define PIN_REAR_LAMPS PB0
#define PIN_FRONT_LAMPS PB2
#define PIN_BLINK_LEFT PB3
#define PIN_BLINK_RIGHT PB4

#define WALLTIME_STATEMACHINE 5000
#define WALLTIME_ENGINE 500
#define WALLTIME_BLINK 250

//#define MAXVOLTAGE_FOR_5  // ATmega328 (5V, 16 MHz)
#define MAXVOLTAGE_FOR_3_3  // ATTiny13A (used when assembled, 3.3V, ~10 MHz)

#include "CarControl01.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR (TIM0_OVF_vect)
{
	static long overflowCounter = 0;
	if(++overflowCounter >= 1200)
	{
		++__timeCounterMsecs;

		overflowCounter = 0;		
	}
}

class Engine
{
private:
	uint8_t _pin;
	uint8_t _maxSpeed;
	uint8_t _currentSpeed;
	uint8_t _targetStepping;
	uint8_t _targetSpeed;
	unsigned long _walltime;
public:
	Engine(uint8_t pin, uint8_t maxSpeed=255) 
		: _pin(pin), _currentSpeed(0), _targetStepping(1), _targetSpeed(0), _walltime(0)
	{
		#ifdef MAXVOLTAGE_FOR_5
			// 168 is the equivalent for getting ~3.3V for an 5V output.
			_maxSpeed = map(168 / (float) maxSpeed * 100, 0, 100, 0, 168);
		#endif
		#ifdef MAXVOLTAGE_FOR_3_3
			_maxSpeed = maxSpeed;
		#endif
	}
	
	inline void setSpeed(uint8_t percentage, uint8_t steps=1) {
		_targetSpeed = map<uint8_t>(percentage, 0, 100, 0, _maxSpeed);
		_targetStepping = steps;
	}
	
	void tick()
	{
		if(_currentSpeed == _targetSpeed)
			return;
	
		if(_walltime > millis())
			return;
		
		_walltime = millis() + WALLTIME_ENGINE;
		
		uint8_t delta = 0;
		
		if(_targetSpeed < _currentSpeed)
		{
			delta = _currentSpeed - _targetSpeed;
			if(_targetStepping > delta)
				_targetStepping = delta;
			
			_currentSpeed -= _targetStepping;			
		}
		else
		{
			delta = _targetSpeed - _currentSpeed;
			if(_targetStepping > delta)
			_targetStepping = delta;
			
			_currentSpeed += _targetStepping;
		}

		OCR0B = _currentSpeed;		
	}	
};

class Lamps
{
public:
	enum Mode { Off, On, Blink };
private:
	uint8_t _pin;
	Mode _mode;
	unsigned long _walltime;
	bool _currentState;
public:	
	Lamps(uint8_t pin) 
		: _pin(pin), _mode(Off), _walltime(0), _currentState(false) 
	{}
	
	void changeMode(Mode mode) 
	{
		if(mode == _mode)
			return;
			
		_mode = mode;

		switch(_mode)
		{
			case On: sbi(_pin, 1); _currentState = true; break;
			case Off: sbi(_pin, 0); _currentState = false; break;
			case Blink: /* ignore */ break;
		}
	}
	
	void tick()
	{
		if(_mode != Blink)
			return;
			
		if(_walltime > millis())
			return;
			
		_walltime = millis() + WALLTIME_BLINK;
		
		_currentState = !_currentState;
		
		if(_currentState)
			sbi(_pin, 1);
		else
			sbi(_pin, 0);
	}	
};

int main(void)
{
	// \see http://www.adnbr.co.uk/articles/adc-and-pwm-basics
	DDRB = 1<<DDB5 | 1<<DDB4 | 1<<DDB3 | 1<<DDB2 | 1<<DDB1 | 1<<DDB0;
	// Set Timer 0 prescaler to clock/8.
	// At 9.6 MHz this is 1.2 MHz.
	TCCR0B |= (1 << CS01) | (1 << CS00);
	// Set to 'Fast PWM' mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// Clear OC0B output on compare match, upwards counting.
	TCCR0A |= (1 << COM0B1);
	
	Engine engine(PIN_ENGINE);
	Lamps frontLamps(PIN_FRONT_LAMPS);
	Lamps rearLamps(PIN_REAR_LAMPS);
	Lamps leftBlinker(PIN_BLINK_LEFT);
	Lamps rightBlinker(PIN_BLINK_RIGHT);	
	
	uint8_t switchToState = 0;
	uint8_t currentState = switchToState;
	
	unsigned long wallTime = 0;
	
    for(;;)
    {		
		engine.tick();
		frontLamps.tick();
		rearLamps.tick();
		leftBlinker.tick();
		rightBlinker.tick();
		
		// Must be changed for cover any switch case.
		#define MAXSTATES 5

		if(wallTime < millis())
		{
			wallTime = millis() + WALLTIME_STATEMACHINE;

			if(++switchToState >= MAXSTATES)
				switchToState = 0;
		}
		
		if(switchToState == currentState)
			continue;
		
		currentState = switchToState;
		
		switch(currentState)
		{
			case 0: 
				engine.setSpeed(0);
				leftBlinker.changeMode(Lamps::Off);
				rightBlinker.changeMode(Lamps::Off);
				frontLamps.changeMode(Lamps::Off);
				rearLamps.changeMode(Lamps::Off);
			break;
			
			case 1:
				engine.setSpeed(25);
				leftBlinker.changeMode(Lamps::Blink);
			break;
			
			case 2:
				engine.setSpeed(50);
				rightBlinker.changeMode(Lamps::Blink);
			break;	

			case 3:
				engine.setSpeed(75);
				frontLamps.changeMode(Lamps::On);
				rearLamps.changeMode(Lamps::On);
			break;

			case 4:
				engine.setSpeed(100);
			break;
		}
    }
}
