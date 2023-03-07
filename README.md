# MIDIArcade

project based on https://leandrolinares.com/blog/arduino-midi-controller/

ported to platform.io and fixed to work with Melodics trening app

## Melodics fix

if your midi device only sends data, add read to pretend to listenn
```c
  MidiUSB.read();
```