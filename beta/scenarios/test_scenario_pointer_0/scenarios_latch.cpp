#include "Arduino.h"
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
 *       version 0.2
 * 
 */

extern Latch LATCH[];

// Constructor
Latch::Latch(void)
{

}

// this routin, called one time at the start-up, initialize the latch
uint8_t Latch::init(uint8_t _parameter, uint8_t _in0_addr, uint8_t _in0_par, uint8_t _in1_addr, uint8_t _in1_par)
{

    parameter=_parameter;   // parameters (type of shutter...)

    in0_addr=_in0_addr; // affect the adress of input 0
    in0_par=_in0_par; // affect the adress of input 0
    in1_addr=_in1_addr; // affect the adress of input 1
    in1_par=_in1_par; // affect the adress of input 1

    #ifdef DEBUG_SCENARIOS
		Serial.println("init latch");
	#endif

    return 1;
}


// this routin update the status of the latch
uint8_t Latch::eval()
{
	
	W_IN0(evalInput(in0_addr, in0_par));

	W_IN1(evalInput(in1_addr, in1_par));
	
	evalOutput();
    #ifdef DEBUG_SCENARIOS
		Serial.println(R_RESULT());
		Serial.println(parameter, BIN);
		Serial.println("/latch eval");
	#endif

   return 1; 
}


// this routin update the output in function of the inputs and of the type latch
uint8_t Latch::evalOutput()
{

	if(R_TYPE()){
		
		#ifdef DEBUG_SCENARIOS
			Serial.print(" T_SR=");
		#endif
		
		if(R_IN0())
			W_RESULT(1);
			
		if(R_IN1())
			W_RESULT(0);

	}
	else
	{
		
		#ifdef DEBUG_SCENARIOS
			Serial.print(" T_RS=");
		#endif
		
		if(R_IN1())
			W_RESULT(0);
			
		if(R_IN0())
			W_RESULT(1);

	}
	
   return 1; 

}

// this routin is generally called by other element of the scenario. It's reply the status of the gate.
uint8_t Latch::result()
{

	return R_RESULT();

}
