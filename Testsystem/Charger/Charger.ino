
#include <LiquidCrystal.h>

const uint8_t pinLedRed = 8;
const uint8_t pinLedGreen = 9;
const uint8_t pinCharger = 10;
const uint8_t pinVoltage = A0;

const uint8_t N_Voltage = 10;
const uint8_t N_RecentAverages = 5;

const char NoBat[] = "No BAT!";
const char BatOk[] = "BAT ok!";
const char Unknown[] = "???";
const char Initialize[] = "Initialize...";
const char Ready[] = "Ready";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float minvoltage = 35.f;
float recentaverages[N_RecentAverages] = {0.0f};

void push(float average)
{
  for(uint8_t i=0; i < N_RecentAverages - 1; ++i)
    recentaverages[i] = recentaverages[i];
  recentaverages[N_RecentAverages - 1] = average;
}

inline float last()
{
  return recentaverages[N_RecentAverages - 1];
}

void setup() 
{
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinCharger, OUTPUT);

  digitalWrite(pinLedRed, HIGH);
  digitalWrite(pinLedGreen, LOW);
  digitalWrite(pinCharger, LOW);

  Serial.begin(9600);

  lcd.begin(20, 4);
  lcd.print(Initialize);

  for(uint8_t i=0; i < N_RecentAverages; ++i)
  {
    bool hasVoltage;
    float value = checkVoltage(hasVoltage);
    push(value);
  }

  printState(Ready);
}

inline void clear()
{
  lcd.setCursor(0, 0);
  for(uint8_t i=0; i < 20*4; ++i)
    lcd.print(" ");
  lcd.setCursor(0, 0);
}

void printState(const char *msg)
{
  clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  lcd.setCursor(0, 1);
  lcd.print(last());
}

float checkVoltage(bool &hasVoltage)
{
  uint8_t values[N_Voltage] = { 0 };
  
  for(int i=0; i < N_Voltage; ++i)
  {
    values[i] = analogRead(pinVoltage);
    
    delay(50);
  }

  float average = 0.0f;
  for(int i=0; i < N_Voltage; ++i)
    average += values[i];
  average /= (float) N_Voltage;
  
  hasVoltage = average > minvoltage;

  return average;
}

void changeChargerState(bool on)
{
  if(on)
  {
    digitalWrite(pinCharger, HIGH);  
    digitalWrite(pinLedRed, LOW);
    digitalWrite(pinLedGreen, HIGH);
  }
  else
  {
    digitalWrite(pinCharger, LOW);
    digitalWrite(pinLedRed, HIGH);
    digitalWrite(pinLedGreen, LOW);
  }
}

void checkSerial()
{
  if(Serial.available())
  {
    char ch = Serial.read();

    switch(ch)
    {
      case '+': changeChargerState(true); break;
      case '-': changeChargerState(false); break;
      default:
        return;
    }

    delay(25);
  }
}

void loop() 
{
  checkSerial();
  
  bool batState = false;

  float value = checkVoltage(batState);
  if(batState)
    push(value);

  if(batState)
  {
    printState(BatOk);
  }
  else
  {
    printState(NoBat);
  }
}
