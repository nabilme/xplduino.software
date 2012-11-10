#ifndef scenarios_command_h
#define scenarios_command_h

#include "Arduino.h"
#include "scenarios_core.h"

class Command
{
    public:
    
    Command(void); // constructor

    uint8_t init(uint8_t _in_addr, uint8_t _in_par, uint8_t _out_addr, uint8_t _out_par);
    uint8_t eval();
    
	private:
    uint8_t in_addr;	// input (instance number)
    uint8_t in_par;	// input (type of source + type of data)
    uint8_t out_addr;	// output (instance number)
    uint8_t out_par;	// output (type of source + type of data)
 


};

#endif

