/*
 * CarControl01.cpp
 *
 * Created: 02.07.2015 10:35:10
 *  Author: Christian Benjamin Ries
 */ 

#ifndef F_CPU
	#define F_CPU 9600000UL
#endif
#define PIN_ENGINE PB1
#define PIN_REAR_LAMPS PB4
#define PIN_FRONT_LAMPS PB3
#define PIN_BLINK_LEFT PB2
#define PIN_BLINK_RIGHT PB5

#define WALLTIME_STATEMACHINE 50
#define WALLTIME_ENGINE 250
#define WALLTIME_BLINK 450

//#define MAXVOLTAGE_FOR_5  // ATmega328 (5V, 16 MHz)
#define MAXVOLTAGE_FOR_3_3  // ATTiny13A (used when assembled, 3.3V, ~10 MHz)

#include "CarControl01.h"

#include <avr/io.h>
#include <avr/interrupt.h>

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

		//OCR0B = _currentSpeed;
		OCR0A = _currentSpeed;
		
		_walltime = millis() + WALLTIME_ENGINE;
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
			case On: sbi(PORTB, _pin); _currentState = true; break;
			case Off: cbi(PORTB, _pin); _currentState = false; break;
			case Blink: /* ignore */ break;
		}
	}
	
	void tick()
	{
		if(_mode != Blink)
			return;
			
		if(_walltime > millis())
			return;
					
		_currentState = !_currentState;

		if(_currentState)
			sbi(PORTB, _pin);
		else
			cbi(PORTB, _pin);

		_walltime = millis() + WALLTIME_BLINK;
	}	
};

inline void setup()
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
	TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
	
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0A |= (1 << COM0B1);
	
	#if defined (__AVR_ATtiny13__)
		TIMSK0 |= 1<<TOIE0;
	#else
		#if defined (__AVR_ATtiny85__)
			TIMSK |= (1 << TOIE0);
		#else
			#pragma error "Missing target cpu!"
		#endif
	#endif

	sei();	
}

int main(void)
{	
	setup();
	
	Engine engine(PIN_ENGINE);
	Lamps frontLamps(PIN_FRONT_LAMPS);
	Lamps rearLamps(PIN_REAR_LAMPS);
	Lamps leftBlinker(PIN_BLINK_LEFT);
	Lamps rightBlinker(PIN_BLINK_RIGHT);	
	
	uint8_t switchToState = 0;
	uint8_t currentState = -1;
	
	unsigned long wallTime = 0;
		
    for(;;)
    {		
		engine.tick();
		frontLamps.tick();
		rearLamps.tick();
		leftBlinker.tick();
		rightBlinker.tick();
			
		// Must be changed for cover any switch case.
		#define MAXSTATES 6

		unsigned long currentMillis = millis();

		if(wallTime < currentMillis)
		{
			++switchToState;
			
			if(switchToState >= MAXSTATES)
				switchToState = 0;
		}
				
		if(switchToState == currentState)
			continue;
		
		currentState = switchToState;
		
		switch(currentState)
		{
			case 0: 
				frontLamps.changeMode(Lamps::Off);
				frontLamps.changeMode(Lamps::Blink);
			break;
			
			case 1:
				frontLamps.changeMode(Lamps::On);
				rearLamps.changeMode(Lamps::Off);
			break;	
			
			case 2:
				frontLamps.changeMode(Lamps::Off);
				rearLamps.changeMode(Lamps::Off);
				
				engine.setSpeed(50, 25);
			break;
			
			case 3:
				engine.setSpeed(75, 10);
			break;
			
			case 4: 
				engine.setSpeed(100, 10);
			break;
			
			case 5:
				engine.setSpeed(0, 50);
			break;
		}		
		
		wallTime = currentMillis + WALLTIME_STATEMACHINE;
    }
}
