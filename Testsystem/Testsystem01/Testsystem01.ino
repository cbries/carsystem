/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * http://www.christianbenjaminries.de
 * https://github.com/cbries
 */

#include "Engine.h"
#include "CarLamps.h"
#include "ThirdParty.h"

const uint8_t PinEngine = 5;
const uint8_t PinFront = 6;
const uint8_t PinBack = 9;
const uint8_t PinTurnLeft = 8;
const uint8_t PinTurnRight = 7; 

Engine engine(PinEngine, 3.3);
CarLamps frontLights(PinFront);
CarLamps backLights(PinBack);
CarTurnLights turnLights(PinTurnLeft, PinTurnRight);

void setup() 
{
  frontLights.Switch(true);
  backLights.Switch(true);
}

unsigned long WallTime = 0;
int TurnLightsIndex = 0;
int TurnLightsIndexMax = 4;

void loop() 
{
  engine.Tick();
  frontLights.Tick();
  backLights.Tick();
  turnLights.Tick();

  if(WallTime < millis())
  {    
    switch(TurnLightsIndex)
    {
      case 0: 
        engine.ChangeSpeedTo(0);
        turnLights.Start(CarTurnLights::None); 
      break;
      
      case 1: 
        engine.ChangeSpeedTo(90, 15);
        turnLights.Start(CarTurnLights::Left); 
      break;
      
      case 2: 
        engine.ChangeSpeedTo(100);
        turnLights.Start(CarTurnLights::Right); 
      break;
      
      case 3: 
        engine.ChangeSpeedTo(75);
        turnLights.Start(CarTurnLights::LeftRight);
      break;
    }
    
    WallTime = millis() + 5000;
    
    TurnLightsIndex++;
    if(TurnLightsIndex >= TurnLightsIndexMax)
      TurnLightsIndex = 0;
  }


}


