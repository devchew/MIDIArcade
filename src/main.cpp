#include "MIDIUSB.h"

//midi defaults
const byte channel = 2; 
const byte onValocity = 100;
const byte offValocity = 0;

const byte TOTAL_BUTTONS = 16;
// All the Arduino pins used for buttons, in order.
const byte BUTTONS_PIN[TOTAL_BUTTONS] = {A3, A2, A1, A0, 15, 14, 16, 10, 9, 8, 7, 6, 5, 4, 3, 2};
// Every pitch corresponding to every Arduino pin. Each note has an associated numeric pitch (frequency scale).
// See https://github.com/arduino/tutorials/blob/master/ArduinoZeroMidi/PitchToNote.h
const byte BUTTONS_PITCH[TOTAL_BUTTONS] = {36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
// Current state of the pressed buttons.
byte currentRead[TOTAL_BUTTONS];
// Temporary input reads to check against current state.
byte tempRead;

void noteOn(byte pitch) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, onValocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte pitch) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, offValocity};
  MidiUSB.sendMIDI(noteOff);
}

// The setup function runs once when you press reset or power the board
void setup() {
  // Initialize all the pins as a pull-up input.
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    pinMode(BUTTONS_PIN[i], INPUT_PULLUP);
  }
}

// The loop function runs over and over again forever
void loop() {
  //pretend to listeen
  MidiUSB.read();

  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    // Get the digital state from the button pin.
    // In pull-up inputs the button logic is inverted (HIGH is not pressed, LOW is pressed).
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    // Temporarily store the digital state.
    tempRead = buttonState;
    // Continue only if the last state is different to the current state.
    if (currentRead[i] != tempRead) {
      // See https://www.arduino.cc/en/pmwiki.php?n=Tutorial/Debounce
      delay(2);
      // Get the pitch mapped to the pressed button.
      byte pitch = BUTTONS_PITCH[i];
      // Save the new input state.
      currentRead[i] = tempRead;
      // Execute note on or noted off depending on the button state.
      if (buttonState == LOW) {
        noteOn(pitch);
      } else {
        noteOff(pitch);
      }
      MidiUSB.flush();
    }
  }
}
