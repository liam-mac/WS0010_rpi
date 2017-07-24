# WS0010_rpi
Raspberry Pi driver for WS0010 LED and OLED screens in C

________________________________________________________________
## Prereqs
This library is dependant on the bcm2835 library. That is the library that makes the GPIO pins available to a programmer in C/C++. 
Link: https://github.com/jperkin/node-rpio/tree/master/src
 ```
  # download the latest version of the library, say bcm2835-1.xx.tar.gz, then:
  tar zxvf bcm2835-1.xx.tar.gz
  cd bcm2835-1.xx
  ./configure
  make
  sudo make check
  sudo make install
  ```

## Compilation
For now I use: `gcc hello.c WS0010_rpi.c -l bcm2835`

## Before you run...
Make sure that the Raspberry Pi pins match the ones defined in the code. I defined the Raspberry Pi data pins that I used to hook up the OLED screen at the start of WS0010_rpi.c. The Control pins are defined in WS0010_rpi.h. 

I used an RPi 2. Your setup may differ. Refer to bcm2835.h for the correct pins to match. 

## Reasoning
I was writing a C program on a Raspberry Pi and needed to use an OLED. The one I bought used a WS0010 chip. I only found python-base drivers. It seemed awkward to call a python program from a C program. So I made this repo. 

## Future Improvements
- Add SPI functionality
- Add auto-scrolling for small screens
- Add a function to display user input

## Acknowlegement 
This library is a modified version of the work done by Nicholas Lopez Fernandez for a C-based WS0010 driver on a PIC microcontroller.
Link to his site: http://www.nicolaselectronics.be/winstar-16x2-character-oled-display-library/
