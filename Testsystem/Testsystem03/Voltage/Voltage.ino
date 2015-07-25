
int voltagePin = 9;
int voltageValue = 0;
byte voltageTargetValue = 0;

#define MAX_VOLTAGE 50

void setup()
{
  pinMode(voltagePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready... enter: + -");
}

void checkSerial()
{
  if(Serial.available())
  {
    byte character = (char)Serial.read();
    if(character == '+')
      voltageValue += 1;
    else if(character == '-')
      voltageValue -= 1;

    if(voltageValue < 0)
      voltageValue = 0;
    else if(voltageValue > MAX_VOLTAGE)
      voltageValue = MAX_VOLTAGE;

    voltageTargetValue = map(voltageValue, 0, MAX_VOLTAGE, 0, 255);

    Serial.print("New Voltage: ");
    Serial.print(voltageTargetValue);
    Serial.print("  ");
    Serial.print(voltageValue/10.f);
    Serial.println("V");
  }
}

void loop() 
{
  checkSerial();
  
  analogWrite(voltagePin, voltageTargetValue);
  
  delay(30);
}

