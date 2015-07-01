/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#ifndef __CARLAMPS_H__
#define __CARLAMPS_H__

#include "Arduino.h"

//!
//! 
//! 
class CarLamps
{
public:
  CarLamps();
  CarLamps(uint8_t pin);
  inline void SetPin(uint8_t pin);
  void Switch(bool state);
  void Toggle();
  void Tick();
  inline bool State() { return _currentState; }

private:
  uint8_t _pin;
  bool _currentState;
};

//!
//!
//! 
class CarTurnLights
{
public:
  enum Side { None=0, Left=1, Right=2, LeftRight=3 };
  
private:
  CarLamps _leftLamps;
  CarLamps _rightLamps;
  
  const static unsigned long Intervall = 500;
  
  Side _side;
  unsigned long _intervall;
  unsigned long _previousMillis;
  
public:
  CarTurnLights(uint8_t pinLeft, uint8_t pinRight);  
  void Start(Side side, unsigned long intervall = Intervall);
  void Stop();
  void Tick();
};

#endif // __CARLAMPS_H__

