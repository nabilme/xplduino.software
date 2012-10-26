#ifndef switch_hard_h
#define switch_hard_h

#include "Arduino.h"
#include <stdio.h>
#include "switch_core.h"

#include "i2c_input.h"

int switch_hard();
int switch_status(byte _switch_id, byte _type);

#endif
