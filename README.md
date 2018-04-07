# SNES-Controller-Interceptor

Based very heavily on @jasburns' [NintendoSpy](https://github.com/jaburns/NintendoSpy), these are Arduino sketches to intercept controller data from one SNES console and send it to another.

[This guide](http://speedrun.evilash25.com/2017/02/controller-input-display-and-arduino.html) should help with more information on wiring. Once you're all set you should connect the wires to the Arduino like this:

* LATCH to pin 3
* DATA IN to pin 4 (this is from the current SNES controller)
* CLOCK to pin 6
* DATA OUT to pin 10 (this is for the other SNES controller)

## Further reading

* [Arduino Port Manipulation](https://www.arduino.cc/en/Reference/PortManipulation)
* [How to control Arduino pins from registers without digitalWrite and digitalRead](https://www.eprojectszone.com/how-to-control-arduino-pins-from-registers-without-digitalwrite-and-digitalread/)
* [Programming for SPI](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi)
* [Arduino Wireless Communication](https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/)
