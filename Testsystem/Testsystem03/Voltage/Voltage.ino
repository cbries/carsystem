
#define MAX_VOLTAGE 500
#define N 3
int voltagePins[N] = { 8, 9, 10 };
int voltageValue = MAX_VOLTAGE;
byte voltageTargetValue = 0;

void setup()
{
  for(uint8_t i=0; i < N; ++i)
    pinMode(voltagePins[i], OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready... enter: + -");

  voltageTargetValue = map(voltageValue, 0, MAX_VOLTAGE, 0, 255);
}

void checkSerial()
{
  if(Serial.available())
  {
    byte character = (char)Serial.read();
    if(character == '\n')
      return;
    if(character == '\r')
      return;
      
    if(character == '-')
      voltageValue += 1;
    else if(character == '+')
      voltageValue -= 1;

    if(voltageValue < 0)
      voltageValue = 0;
    else if(voltageValue > MAX_VOLTAGE)
      voltageValue = MAX_VOLTAGE;

    voltageTargetValue = map(voltageValue, 0, MAX_VOLTAGE, 0, 255);

    Serial.print("New Voltage: ");
    Serial.print(voltageTargetValue);
    Serial.print("  ");
    Serial.print((MAX_VOLTAGE/100) - voltageValue/100.f);
    Serial.println("V");
  }
}

void loop() 
{
  checkSerial();

  for(uint8_t i = 0; i < N; ++i)   
    analogWrite(voltagePins[i], voltageTargetValue);
  
  delay(30);
}

