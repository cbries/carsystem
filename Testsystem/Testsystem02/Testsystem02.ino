
//
// Required Libraries:
// https://code.google.com/p/rogue-code/wiki/SoftPWMLibraryDocumentation
//

#include <SoftPWM.h>
#include <SoftPWM_timer.h>

//uint8_t PinNormal = 4;
uint8_t PinBlink = 3;
uint8_t PinBreakDown = 2;

// ##########################################################
bool Enabled_BreakDown = false;
unsigned long RecentMillis_BreakDown = 0;
unsigned long IntervalMillis_BreakDown = 2500;

bool Enabled_Blink = false;
unsigned long RecentMillis_Blink = 0;
unsigned long IntervalMillis_Blink = 400;

const int Minimum_BreakDown = 70;
const int Maximum_BreakDown = 100;

const int Minimum_Blink = 0;
const int Maximum_Blink = 90;
// ##########################################################

void setup()
{
//  pinMode(PinNormal, OUTPUT);
//  digitalWrite(PinNormal, HIGH);
  
  SoftPWMBegin();
  SoftPWMSet(PinBlink, Minimum_Blink);
  SoftPWMSet(PinBreakDown, Minimum_BreakDown);
  
  SoftPWMSetFadeTime(PinBlink, 75, 100);
  SoftPWMSetFadeTime(PinBreakDown, 10, 25);
}

void loop()
{
  unsigned long currentMillis = millis();
  
  //
  // BreakDown
  //
  if(currentMillis - RecentMillis_BreakDown > IntervalMillis_BreakDown)
  {
    SoftPWMSetPercent(PinBreakDown, !Enabled_BreakDown ? Maximum_BreakDown : Minimum_BreakDown);
    Enabled_BreakDown = !Enabled_BreakDown;
    RecentMillis_BreakDown = millis();
  }
  
  //
  // Blinking
  //
  if(currentMillis - RecentMillis_Blink > IntervalMillis_Blink)
  {
    SoftPWMSetPercent(PinBlink, !Enabled_Blink ? Maximum_Blink : Minimum_Blink);    
    Enabled_Blink = !Enabled_Blink;    
    RecentMillis_Blink = millis();
  }  
}

