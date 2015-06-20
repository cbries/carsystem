/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#include "Engine.h"

Engine::Engine(
  uint8_t pinEngine,
  float maxVoltage,
  int accelerationSteps, 
  int breakDownSteps,
  int gaptime /* msecs */
  ) 
  : _pinEngine(pinEngine)
  , _accelerationSteps(accelerationSteps)
  , _breakDownSteps(breakDownSteps)
  , _walltimeGap(gaptime)
{
  float percentage = 0.0f;

  if(maxVoltage >= MaxVoltage || maxVoltage < 0)
    maxVoltage = MaxVoltage;

  SetMaxSpeed(maxVoltage / MaxVoltage * 100.f);
        
  pinMode(_pinEngine, OUTPUT);
  analogWrite(_pinEngine, 0);    
}
  
bool Engine::IsHold()
{
  return _currentSpeed == _targetSpeed;
}
  
void Engine::Tick()
{
  if(IsHold())
    return;
    
  if(_walltime > millis())
    return;
        
  int delta = 0;      
  int localSteps = -1;
      
  if(_targetSpeed < _currentSpeed)
  {
    localSteps = (_overrideSteps == -1 ? _breakDownSteps : _overrideSteps);
    
    delta = _currentSpeed - _targetSpeed;
    if(delta < localSteps)
      _currentSpeed = _targetSpeed;
    else
    {
      _currentSpeed -= localSteps;
    }
  }
  else
  {
    localSteps = (_overrideSteps == -1 ? _accelerationSteps : _overrideSteps);
    
    delta = _targetSpeed - _currentSpeed;
    if(delta < localSteps)
      _currentSpeed = _targetSpeed;
    else
    {
      _currentSpeed += localSteps;
    }
  }
    
  _walltime = millis() + _walltimeGap;
    
  analogWrite(_pinEngine, _currentSpeed);
}
      
void Engine::ChangeSpeedTo(int percentage, int steps)
{   
  if(percentage > 100)
    percentage = 100;
  if(percentage < 0)
    percentage = 0;    
  _overrideSteps = steps;
  _targetSpeed = speed(percentage);   
  _walltime = millis();
}

void Engine::SetMaxSpeed(uint8_t percentage)
{
  #ifdef MAXVOLTAGE_FOR_5
    _maxSpeed = map(percentage, 0, 100, 0, 168);
  #endif
    
  #ifdef MAXVOLTAGE_FOR_3_3
    _maxSpeed = 255;
  #endif
}

