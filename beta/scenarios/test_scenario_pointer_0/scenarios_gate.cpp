#include "Arduino.h"
#include "scenarios_gate.h"

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

// Constructor
Gate::Gate(void)
{

}

// this routin, called one time at the start-up, initialize the gate
uint8_t Gate::init(uint8_t _parameter, uint8_t _in0_addr, uint8_t _in0_par, uint8_t _in1_addr, uint8_t _in1_par)
{

    parameter=_parameter;   // parameters of the gate + result of the evaluation

    in0_addr=_in0_addr; // affect the adress of input 0
    in0_par=_in0_par;	// affect the type and the command of input 0

    in1_addr=_in1_addr;	// affect the adress of input 1
    in1_par=_in1_par;	// affect the type and the command of input 1

    #ifdef DEBUG_SCENARIOS
		Serial.println("init gate");
	#endif

    return 1;
}


// this routin update the status of the gate
uint8_t Gate::eval()
{
	
	W_IN0(evalInput(in0_addr, in0_par));

	W_IN1(evalInput(in1_addr, in1_par));
	
	W_RESULT(evalOutput());

    #ifdef DEBUG_SCENARIOS
		Serial.println("/gate eval");
	#endif

   return 1; 
}

// this routin update the output in function of the inputs and of the type gate
uint8_t Gate::evalOutput()
{

    #ifdef DEBUG_SCENARIOS
		Serial.println("GATE: ");
		Serial.println((parameter & 0x0F),BIN);
		Serial.println(parameter,BIN);
	#endif
			
	switch ((parameter & 0x03)){
		
		case T_AND:	
			#ifdef DEBUG_SCENARIOS
				Serial.print("T_AND=");
				Serial.println(R_IN0() && R_IN1());
			#endif
			return (R_IN0() && R_IN1());

		break;
		
		case T_OR:
			#ifdef DEBUG_SCENARIOS
				Serial.print("T_OR=");
				Serial.println(R_IN0() || R_IN1());
			#endif
			return (R_IN0() || R_IN1());

		break;
		
		default:
			return WARNING_CODE;
		break;	
			
	}

   return ERROR_CODE; 
}

// this routin is generally called by other element of the scenario. It's reply the status of the gate.
uint8_t Gate::result()
{

	return R_RESULT();

}
