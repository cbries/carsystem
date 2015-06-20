# Wishlist

## Features
 - Blinker rechts
 
 - Blinker links

 - zuschaltbare Frontbeleuchtung

 - zuschaltbare Rückbeleuchtung

 - Warnblinker aktivieren

 - Kommandos über Funk, Infrarot, oder?

 - einfache bis triviale Programmierung

 - möglichst auf offene und freie Software und Bibliotheken setzen

 - Stop/Halt eines Fahrzeugs auf Befehl

 - Anfahrverzögerung

 - Bremsverzögerung

 - Abstandhalten zum Vordermann/vorfahrenden Fahrzeug

 - ...
 
 ## How to programm a Arduino Pro Mini with Arduino MEGA 2560?
 
 1) Open the example "ArduinoISP" and put it onto an ATMEGA2560.
 
 2) 5. Use a breadboard, and make sure you connect the following pins:
```
  MEGA  =>  Pro Mini
   52   =>   13  
   53   =>   RST  
   50   =>   12    
   51   =>   11  
   GND  =>   GND  
   +5V = >   VCC  
```
Also connect  a 10uF capacitor over GND and RESET on the Arduino MEGA 2560. The "minus"-sign must be connected to the GND.
 
 3) Open the "Blink LED" example, and change the code to fit your setup:
```CPP
void setup() {
  pinMode(7, OUTPUT); pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); pinMode(10, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
```

 4) Set your programmer to "Arduino as ISP", and do "Upload with Programmer" from the file-menu in the Arduino IDE.
 