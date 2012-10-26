#ifndef shutter_hard_h
#define shutter_hard_h

#include "Arduino.h"
#include <stdio.h>
#include "shutter_core.h"

#include "i2c_output.h"

int shutter_hard();
int shutter_status(byte _switch_id, byte _type);
int shutter_post(); // update status of each shutter
int shutter_status();

#endif
