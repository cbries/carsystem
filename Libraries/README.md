# Installing ATTiny13A support for Arduino IDE

- see http://forum.arduino.cc/index.php?topic=89781.330  

- On windows it will usually go in `C:\arduino\arduino-<version>\hardware\arduino\cores\core13`. For me it was `C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\core13`.

- I added this to my boards.txt:
```
attiny13.name=Attiny 13A standalone  
  
attiny13.upload.using=arduino:arduinoisp  
attiny13.upload.maximum_size=1024  
attiny13.upload.speed=19200  
  
attiny13.bootloader.low_fuses=0x7B  
attiny13.bootloader.high_fuses=0xFF  
attiny13.bootloader.path=empty  
attiny13.bootloader.file=empty  
  
attiny13.bootloader.unlock_bits=0xFF  
attiny13.bootloader.lock_bits=0xFF  
  
attiny13.build.mcu=attiny13a  
attiny13.build.f_cpu=128000  
attiny13.build.core=core13  
```