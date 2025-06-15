# An ESP8266-based Server Monitor
This ESP8266-based server monitor PCB displays the status of up to 8 servers with RGB LEDs. If it receives HTTP 200, the LED will turn green. If it receives anything else, it will turn yellow the first time and red subsequent times.

The big white patches of silkscreen are there for you to write the server names on. You can use whiteboard markers if they change often, permanent marker (clean off with ethanol) or labels.

The firmware is just an Arduino sketch. I've done it this way because Arduino is popular, so this is the easiest for most people to modify.

It will send the HTTP GET requests with https, but will not verify the certificates. This is by design -- there's not much memory on an ESP8266, and you'd have to go through the hassle of loading the certificates onto the MCU storage manually. This is also why it only sends one request at a time instead of creating a http client and then sending all 9. That would often causes out-of-memory errors!

# BOM
- 1x ESP8266 mini-D1 pro. The non-pro version will work, but you'll have to solder in header pins, then solder those directly the pads on the board. The Pro version is lower profile and solders flush to the board. 
- 1x TM1640 LED driver
- 8x 5050 RGB LED
- 1x 1411 Tantalum capacitor 100uf 6.3V (you can use electrolytic but it will be bigger)
- 1x 0603 0.1uF ceramic capacitor 50V
- 4x 0402 10k ohm resistor
- 1x 0402 20k ohm resistor (this is for the power LED to make it similar brightness to the others. If you don't care, you can use another 10k)
- 3x 0603 red LED (or any color that works under ESP8266 GPIO HIGH voltage)
- Antenna for mini-D1 Pro (optional)
- 1x PCB

# Total Cost

In Asia, about VND 164.550 ($6.31) without antenna or 190.000 (7.29) with antenna (board included, assuming you make 5 units). In the West probably double, but it's still pretty cheap!

# Photo

![Photo of the completed server monitor](https://raw.githubusercontent.com/seanboyce//servermon/refs/heads/main/servermon_generic.JPG)
