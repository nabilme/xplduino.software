#include "Arduino.h"
#include "scenarios_command.h"

/*       Part of the xplduino project - http://code.google.com/p/xplduino/
 *       Write by Romain TISSOT CHARLOD
 *       Contact: xplduino@gmail.com
 * 
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *       
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *       
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA
 * 
 *       version 0.0: creation
 * 
 */

extern Command COMMAND[];

// Constructor
Command::Command(void)
{

}

// this routin, called one time at the start-up, initialize the command
uint8_t Command::init(uint8_t _in_addr, uint8_t _in_par, uint8_t _out_addr, uint8_t _out_par)
{
    in_addr=_in_addr;	// affect the adress of input
    in_par=_in_par;		// affect the adress of input
 
    out_addr=_out_addr;	// affect the adress of output
    out_par=_out_par;	// affect the adress of output
    
	#ifdef DEBUG_SCENARIOS
		Serial.println("init command");   
	#endif

    return 1;
}



uint8_t Command::eval()
{
	
	#ifdef DEBUG_SCENARIOS	
		Serial.print("out_addr= ");
		Serial.println(out_addr);
		Serial.print("out_par= ");
		Serial.println(out_par & 0x1F, HEX);
	#endif

	uint8_t result=evalInput(in_addr, in_par);
	
	if(result == ERROR_CODE || result == WARNING_CODE)
		return ERROR_CODE;
	
	
	if(result){
		
		switch (out_par & MASK_TYPE){
			case T_LIGHTING:
				//~ return GATE[_in_addr].result();
			break;
			case T_SHUTTER:
				//~ return LATCH[_in_addr].result();
			break;
			case T_SWITCH:
				//~ return !digitalRead(_in_addr);
			break;
			case T_DO:
				digitalWrite(out_addr, HIGH);
			break;
			
			default:
			return WARNING_CODE;

		}
	}
	else
	{ // result is null
	
		switch (out_par & 0x1F){
			case T_LIGHTING:
				// here do an action (toggle, on, off...)
			break;
			case T_SHUTTER:
				// here do an action (toggle, up, down...)
			break;
			case T_DO:
				digitalWrite(out_addr, LOW);
			break;
			
			default:

			return WARNING_CODE;

		}
	}

   return 1; 
}


