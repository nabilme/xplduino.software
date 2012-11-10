#ifndef scenarios_gate_h
#define scenarios_gate_h

#include "Arduino.h"
#include "scenarios_core.h"


// type of command
//~ #define T_AND 0x00
//~ #define T_OR  0x01
//~ #define T_XOR 0x02

// address of the bits for parameter byte: 
#define ADDR_RESULT		        3   // result
#define ADDR_IN0_NOT            4   // input 0 is reversed
#define ADDR_IN1_NOT     		5   // input 1 is reversed
#define ADDR_IN0            	6   // input 0 status
#define ADDR_IN1     			7   // input 1 status

// macro to read bits inside parameter byte:
#define R_RESULT()         bitRead(parameter, ADDR_RESULT)
#define R_IN0_NOT()        bitRead(parameter, ADDR_IN0_NOT)
#define R_IN1_NOT()        bitRead(parameter, ADDR_IN1_NOT)
#define R_IN0()            bitRead(parameter, ADDR_IN0)
#define R_IN1()            bitRead(parameter, ADDR_IN1)

// macro to write bits inside parameter byte:
#define W_RESULT(value)         bitWrite(parameter, ADDR_RESULT, value)
#define W_IN0_NOT(value)        bitWrite(parameter, ADDR_IN0_NOT, value)
#define W_IN1_NOT(value)        bitWrite(parameter, ADDR_IN1_NOT, value)
#define W_IN0(value)            bitWrite(parameter, ADDR_IN0, value)
#define W_IN1(value)            bitWrite(parameter, ADDR_IN1, value)



class Gate
{
    public:
    
    Gate(void); // constructor

    uint8_t init(uint8_t _parameter, uint8_t _in0_addr, uint8_t _in0_par, uint8_t _in1_addr, uint8_t _in1_par);
    uint8_t eval();
    uint8_t result();

	uint8_t evalOutput();

	private:
    uint8_t in0_addr;		// input 0 (instance number)
    uint8_t in0_par;		// input 0 (type of source + type of data)
    uint8_t in1_addr;		// input 0 (instance number)
    uint8_t in1_par;		// input 0 (type of source + type of data)

    uint8_t  parameter;	// parameter data


};

#endif

