#include <SoftwareSerial.h>

SoftwareSerial readPort(11, 9); // rx, tx

uint8_t ledRed = 2;
bool ledRedState = false;

void setup()  
{
  pinMode(ledRed, OUTPUT);
  digitalWrite(ledRed, LOW);
  
  readPort.begin(1200);
  
  delay(200);
}

bool checkTwoBytes(byte b0, byte b1)
{
  if(readPort.read() != b0)
    return false;
  if(readPort.read() != b1)
    return false;
  return true;
}

void setRedLed(bool state)
{
  if(state)
    digitalWrite(ledRed, HIGH);
  else
    digitalWrite(ledRed, LOW);
}

void loop() 
{
  while (readPort.available()) 
  {
    int sig = readPort.read();

    if(sig == 0)
      break;

    switch(sig)
    {
      case 181: 
        if(checkTwoBytes(178, 14))
          setRedLed(true);
      break;    
      
      case 206: 
        if(checkTwoBytes(178, 14))
          setRedLed(false);
      break;
    }
  }

  delay(1000); 
}
