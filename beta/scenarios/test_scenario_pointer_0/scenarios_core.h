#ifndef scenarios_core_h
#define scenarios_core_h

#include "Arduino.h"

//~ #define DEBUG_SCENARIOS

// type of class
#define T_GATE 0x01
// type of command gate
#define T_AND 0x00
#define T_OR  0x01
#define T_XOR 0x02


// type of class
#define T_LATCH 0x02
// type of command latch
#define T_SR  0x00
#define T_RS  0x01

// type of class
#define T_DI 0x0F
#define T_DO 0x0E
#define T_SWITCH 0x1A
#define T_LIGHTING 0x1B
#define T_SHUTTER 0x1C

#define MASK_TYPE 0x1F
#define WARNING_CODE 0xFE
#define ERROR_CODE 0xFF

	uint8_t evalInput(uint8_t _in_addr, uint8_t _in_par);


#endif

