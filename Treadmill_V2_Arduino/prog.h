#ifndef prog_h
#define prog_h

#include "Arduino.h"

class prog {
  public:
    prog(int pin); // Constructor
    void prog_on();
    void prog_off();
    void pulse_code(int peaks);
  private:
    int _pin;
};

#endif