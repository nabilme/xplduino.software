#ifndef i2c_h
#define i2c_h

#include "Arduino.h"
#include <Wire.h>

//~ #include <Wire.h>


void expanderWrite(int _expander, byte _data);
byte expanderRead(int _expander);
 

#endif
