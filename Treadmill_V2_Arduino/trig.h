#ifndef trig_h
#define trig_h

#include "Arduino.h"

class trig {
  public:
    trig(int pin); // Constructor
    void trig_on();
    void trig_off();
  private:
    int _pin;
};

#endif