#ifndef config_h
#define config_h

#define lighting_activated
//~ #define DEBUG_LIGHTING_XPL
//~ #define DEBUG_LIGHTING_CORE

#define shutter_activated
//~ #define DEBUG_SHUTTER_XPL
//~ #define DEBUG_SHUTTER_CORE

#define switch_activated
//~ #define DEBUG_SWITCH_XPL
//~ #define DEBUG_SWITCH_CORE

//~ #define DEBUG_ETHERNET

/// debug message xpl
//~ #define DEBUG_INCOMING
//~ #define DEBUG_OUTCOMING





#ifdef lighting_activated
#define LIGHTING_MAX 2  // maximum of light
#endif

#ifdef switch_activated
#define SWITCH_MAX 8    // maximum of button
#endif

#ifdef shutter_activated
#define SHUTTER_MAX 3   // maximum of shutter
#endif

#define XPL_PORT 3865

#define T_CMND 1
#define T_STAT 2
#define T_TRIG 3

#define VENDOR_ID xplduino
#define DEVICE_ID xplduino

//~ 
//~ char vendor_id[8+1]={'xplduino\0'};     // vendor id
//~ char device_id[8+1]={'lighting\0'};     // device id
//~ char instance_id[18+1]={'garage\0'};    // instance id

#endif
