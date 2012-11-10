
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

/* this sample code is made to test a simple equation:
 * 
 *            _____        _____        ______
 * [ pin 2 ]-| AND |------|     |------| S    |-----[ pin 13 ]
 * [ pin 3 ]-|_____|      | OR  |      |      |
 *            _____       |     |      |  SR  |
 * [ pin 4 ]-| AND |------|_____|      |      |
 * [ pin 5 ]-|_____|                   |      |
 * [ pin 6 ]---------------------------| R    |
 *                                     |______|
 * 
 *  note: you can activate verbose mode by uncomment #define DEBUG_SCENARIOS in scenarios_core.h
*/


#include "scenarios_core.h"
#include "scenarios_gate.h"
#include "scenarios_latch.h"
#include "scenarios_command.h"

#include "MemoryFree.h"

Gate GATE[4];
Latch LATCH[4];
Command COMMAND[4];


void setup()
{
    delay(1000);

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	pinMode(13, OUTPUT);  
	pinMode(2, INPUT);  
	pinMode(3, INPUT);  
	pinMode(4, INPUT);  
	pinMode(5, INPUT);  
	pinMode(6, INPUT);  

	digitalWrite(2, HIGH);   // set the pullup
	digitalWrite(3, HIGH);   // set the pullup
	digitalWrite(4, HIGH);   // set the pullup
	digitalWrite(5, HIGH);   // set the pullup
	digitalWrite(6, HIGH);   // set the pullup



	// start serial port at 115200 bps:	
	Serial.begin(115200);

    Serial.print("free RAM in kB: ");
    Serial.println(freeMemory());


	GATE[1].init(T_AND, 2, T_DI, 3, T_DI);
	GATE[2].init(T_AND, 4, T_DI, 5, T_DI);
	GATE[3].init(T_OR, 1, T_GATE, 2, T_GATE);

	LATCH[0].init(T_SR, 3, T_GATE, 6, T_DI);

	COMMAND[0].init(0, T_LATCH, 13, T_DO);
	
}

void loop()
{
 
  sketch();
  
  delay(100);
  
}


void sketch()
{
	//~ Serial.println("Sketch: ");
	
	//~ Serial.print(millis());
	//~ Serial.print("-");

	GATE[1].eval();	
	GATE[2].eval();	
	GATE[3].eval();
	LATCH[0].eval();
	COMMAND[0].eval();

	//~ Serial.println(millis());

	//~ Serial.print("Result: ");
    //~ Serial.println(LATCH[0].result());

  
}
