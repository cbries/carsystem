/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Arduino.h"

//#define MAXVOLTAGE_FOR_5  // ATmega328 (5V, 16 MHz)
#define MAXVOLTAGE_FOR_3_3  // tbd

#ifdef MAXVOLTAGE_FOR_5
  const float MaxVoltage = 5.0f;
#else
  #ifdef MAXVOLTAGE_FOR_3_3
    const float MaxVoltage = 3.3f;
  #else
    const float MaxVoltage = 3.3f;
  #endif
#endif

#define DEFAULT_ENGINE_STEPS 5
#define DEFAULT_ENGINE_STEP_GAPTIME 75 /* msecs */

class Engine
{  
public:
  //! \param pinEngine
  //! \param maxVoltage 
  //! \param accelerationSteps
  //! \param breakDownSteps
  //! \param gaptime
  Engine(uint8_t pinEngine,
      float maxVoltage=3.3f,
      int accelerationSteps=DEFAULT_ENGINE_STEPS, 
      int breakDownSteps=DEFAULT_ENGINE_STEPS,
      int gaptime=DEFAULT_ENGINE_STEP_GAPTIME
    );
  
  void Tick();
  
  void ChangeSpeedTo(int percentage, int steps=-1);
    
private:
  bool IsHold();  
  void SetMaxSpeed(uint8_t percentage);

  //!
  int _currentSpeed;  
  //! 
  int _targetSpeed;
  //! value is absolute, related to the microcontroller
  int _maxSpeed;

  inline int speed(uint8_t percentage) {
    if(percentage > 100)
      return _maxSpeed;
    if(percentage < 0)
      return 0;
    return map(percentage, 0, 100, 0, _maxSpeed);
  }

  int _overrideSteps;
  int _accelerationSteps;
  int _breakDownSteps;

  int _walltimeGap;
  unsigned long _walltime;

  uint8_t _pinEngine;
};

#endif // __ENGINE_H__
