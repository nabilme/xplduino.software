#include "Arduino.h"
#include "scenarios_core.h"
#include "scenarios_gate.h"
#include "scenarios_latch.h"


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

extern Gate GATE[];
extern Latch LATCH[];


uint8_t evalInput(uint8_t _in_addr, uint8_t _in_par)
{

    #ifdef DEBUG_SCENARIOS
		Serial.print("evalInput addr: ");
		Serial.println(_in_addr, HEX);
		Serial.print("evalInput par: ");
		Serial.println(_in_par, HEX);
    #endif
	
	uint8_t type=_in_par & MASK_TYPE; // retrieve the type (5 LSB)
	uint8_t cmnd=_in_par >> 8; // retrieve the command (shift to right the 3 MSB)
		
	if (type){ // check if type is valid (not null)
	
		switch (type){ // select the action to do in function of the type and the command
		
			case T_GATE:
				return GATE[_in_addr].result();
			break;
			
			case T_LATCH:
				return LATCH[_in_addr].result();
			break;
			
			case T_DI:
				return !digitalRead(_in_addr);
			break;
			
			default:
			
			    #ifdef DEBUG_SCENARIOS
					Serial.println(" /evalInput type not found");   
			    #endif
				return WARNING_CODE;
		
		}
	}
	
			    #ifdef DEBUG_SCENARIOS
					Serial.println(" /evalInput type not valid");   
			    #endif

   return ERROR_CODE; 
}

