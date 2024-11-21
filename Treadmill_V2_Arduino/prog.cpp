#include "prog.h"
#include "Arduino.h"

// constructor
prog::prog(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void prog::prog_on() {
  digitalWrite(_pin, HIGH);
}

void prog::prog_off() {
  digitalWrite(_pin, LOW);
}

void prog::pulse_code(int peaks) {
  for (int i = 0; i<peaks ; i++ ) {
    digitalWrite(_pin, HIGH);
    delay(50);
    digitalWrite(_pin, LOW);
    delay(50);
  }
}
