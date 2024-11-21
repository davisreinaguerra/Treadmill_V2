#include "trig.h"
#include "Arduino.h"

// constructor
trig::trig(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void trig::trig_on() {
  digitalWrite(_pin, HIGH);
}

void trig::trig_off() {
  digitalWrite(_pin, LOW);
}
