/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#include "CarLamps.h"

CarLamps::CarLamps()
{ }

CarLamps::CarLamps(uint8_t pin)
  : _pin(pin)
{
  pinMode(_pin, OUTPUT);
}

void CarLamps::SetPin(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void CarLamps::Switch(bool state)
{
  _currentState = state;
  digitalWrite(_pin, _currentState ? HIGH : LOW);
}

void CarLamps::Toggle()
{
  if(_currentState)
  {
    digitalWrite(_pin, LOW);
    _currentState = false;
  }
  else
  {
    digitalWrite(_pin, HIGH);
    _currentState = true;
  }
}

void CarLamps::Tick()
{
  // ignore 
}

// #####################################
// 
// #####################################

CarTurnLights::CarTurnLights(uint8_t pinLeft, uint8_t pinRight)
  : _side(None)
{
  _leftLamps.SetPin(pinLeft);
  _rightLamps.SetPin(pinRight);
}

void CarTurnLights::Start(Side side, unsigned long intervall)
{
  Stop();
  _side = side;
  _intervall = intervall;  
  _previousMillis = millis();
}

void CarTurnLights::Stop()
{
  _previousMillis = 0;  
  _intervall = 0;
  _side = None;
  _leftLamps.Switch(false);
  _rightLamps.Switch(false);
}

void CarTurnLights::Tick()
{
  unsigned long currentMillis = millis();
  if(currentMillis - _previousMillis > _intervall) 
  {
    switch(_side)
    {
      case None: Stop(); break;
      case Left: _leftLamps.Toggle(); break;
      case Right: _rightLamps.Toggle(); break;
      case LeftRight: 
        _leftLamps.Toggle();
        _rightLamps.Toggle();
      break;
    }
    
    _previousMillis = millis();
  }
}

