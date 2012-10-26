#ifndef shutter_xpl_h
#define shutter_xpl_h

#include "Arduino.h"
#include <stdio.h>
#include <string.h>
#include "shutter_core.h"
#include "config.h"

//~ #include "xpl.h"

//~ #include "config.h" // specific project data

//~ class shutter_xplClass {
//~ private:

    int16_t shutter_basic(uint8_t *udpPayload, int16_t first_cmnd);
    int shutter_status(byte _shutter_id, byte _type);
    int shutter_find(char *_name, int first);
    int shutter_xpl_cmdshutter(char *_instance_id, char *_device, byte _new_setpoint, byte _new_fade);
    //~ int time_to_pourcent(int _value_to_convert, int _cycle_reference_time);

#endif




