#include <SoftwareSerial.h>        // Ref: https://docs.arduino.cc/learn/built-in-libraries/software-serial
#include <MIDI.h>                  // Ref: https://www.arduino.cc/reference/en/libraries/midi-library/
#include <JC_Button.h>             // Ref: https://www.arduino.cc/reference/en/libraries/jc_button/
#include <ResponsiveAnalogRead.h>  // Ref: https://www.arduino.cc/reference/en/libraries/responsiveanalogread/

// Define MIDI Channel, Note numbers and CC numbers.
const int MIDI_CH = 1;

const int NOTE_A = 34;
const int NOTE_B = 35;
const int NOTE_C = 36;
const int NOTE_D = 37;

const int CC_A = 34;
const int CC_B = 35;
const int CC_C = 36;
const int CC_D = 37;

// Setup buttons.
Button btnA(4);
Button btnB(5);
Button btnC(6);
Button btnD(7);

// Setup pots.
ResponsiveAnalogRead potA(A3, true);
ResponsiveAnalogRead potB(A2, true);
ResponsiveAnalogRead potC(A1, true);
ResponsiveAnalogRead potD(A0, true);

// Setup status LED.
const int LED_PIN = LED_BUILTIN;
const int LED_DURATION = 100;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// Setup MIDI.
SoftwareSerial softSerial(2, 3);
MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, midiA);

void setup() {
  pinMode(LED_PIN, OUTPUT);

  btnA.begin();
  btnB.begin();
  btnC.begin();
  btnD.begin();

  midiA.begin(MIDI_CH);
  Serial.begin(9600);
}

void loop() {
  btnA.read();
  btnB.read();
  btnC.read();
  btnD.read();

  potA.update();
  potB.update();
  potC.update();
  potD.update();

  // Check switch states, send MIDI notes.
  if (btnA.wasPressed()) {
    midiA.sendNoteOn(NOTE_A, 127, MIDI_CH);
    updatePreviousMillis();
    Serial.println("A");
  }
  if (btnB.wasPressed()) {
    midiA.sendNoteOn(NOTE_B, 127, MIDI_CH);
    updatePreviousMillis();
    Serial.println("B");
  }
  if (btnC.wasPressed()) {
    midiA.sendNoteOn(NOTE_C, 127, MIDI_CH);
    updatePreviousMillis();
    Serial.println("C");
  }
  if (btnD.wasPressed()) {
    midiA.sendNoteOn(NOTE_D, 127, MIDI_CH);
    updatePreviousMillis();
    Serial.println("D");
  }

  if (btnA.wasReleased()) {
    midiA.sendNoteOff(NOTE_A, 0, MIDI_CH);
    updatePreviousMillis();
  }
  if (btnB.wasReleased()) {
    midiA.sendNoteOff(NOTE_B, 0, MIDI_CH);
    updatePreviousMillis();
  }
  if (btnC.wasReleased()) {
    midiA.sendNoteOff(NOTE_C, 0, MIDI_CH);
    updatePreviousMillis();
  }
  if (btnD.wasReleased()) {
    midiA.sendNoteOff(NOTE_D, 0, MIDI_CH);
    updatePreviousMillis();
  }

  // Check pot values, send MIDI CC.
  if (potA.hasChanged()) {
    int value = potA.getValue();
    midiA.sendControlChange(CC_A, map(value, 0, 1022, 127, 0), MIDI_CH);
    updatePreviousMillis();
  }
  if (potB.hasChanged()) {
    int value = potB.getValue();
    midiA.sendControlChange(CC_B, map(value, 0, 1022, 127, 0), MIDI_CH);
    updatePreviousMillis();
  }
  if (potC.hasChanged()) {
    int value = potC.getValue();
    midiA.sendControlChange(CC_C, map(value, 0, 1022, 127, 0), MIDI_CH);
    updatePreviousMillis();
  }
  if (potD.hasChanged()) {
    int value = potD.getValue();
    midiA.sendControlChange(CC_D, map(value, 0, 1022, 127, 0), MIDI_CH);
    updatePreviousMillis();
  }

  // Light status LED if any MIDI messages had been sent.
  currentMillis = millis();
  if (currentMillis - previousMillis <= LED_DURATION) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void updatePreviousMillis() {
  previousMillis = currentMillis;
}