#ifndef probabilityFunctions_h
#define probabilityFunctions_h

#include "Arduino.h"

String flip_coin(); 
bool enforce_prob(int prob);
int get_choice(int numOptions);

#endif