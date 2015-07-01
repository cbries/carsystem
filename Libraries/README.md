# Installing ATTiny13A support for Arduino IDE

- see http://www.instructables.com/id/Programming-an-ATTiny13A-using-Arduino-servo-int/?ALLSTEPS  

- We now need to set up the IDE to recognise our ATTiny13A as an arduino compatible chip  
 a) Download and extract this file: https://github.com/tekstop/attiny/tree/Arduino1  
 
 b) Copy the folder "attiny" from the extracted ZIP to the /hardware/ folder in your arduino IDE installation directory

- We then need to offset the clock speed (not a very elegant solution, but i'm working on it!):  
 a) Open Hardware/attiny/boards.txt  
 
 b) Scroll to the bottom of the file, this last few lines contains the data relevant to our chip  
 
 c) Change "attiny13.build.f_cpu=9600000L" to "attiny13.build.f_cpu=1000000L"

- This will correct delay times etc.  
 a) Open the Arduino IDE again  
 
 b) Go to Tools>Board and select "ATtiny13 (internal 9.6 MHz clock)"  
