#ifndef lighting_xpl_h
#define lighting_xpl_h

#include "Arduino.h"
#include <stdio.h>
#include <string.h>
#include "lighting_core.h"
#include "config.h"

//~ #include "xpl.h"

//~ #include "config.h"	// specific project data

//~ class lighting_xplClass {
//~ private:

	int16_t lighting_basic(uint8_t *udpPayload, int16_t first_cmnd);
	int lighting_status(byte _lighting_id, byte _type);
	int lighting_find(char *_name, int first);
    int lighting_xpl_cmdlighting(char *_instance_id, char *_device, byte _new_setpoint, byte _new_fade);

#endif




