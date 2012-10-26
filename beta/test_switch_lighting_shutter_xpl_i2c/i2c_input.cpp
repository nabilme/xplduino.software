#include "i2c_input.h"

#include "i2c.h"
#include <Wire.h>


extern byte i2c_in[];


// recupere la valeur d'une entree
byte i2c_in_retreive(byte _DI_address){

    StructAddress _address;

    _address.bitAddress= _DI_address & 0x0F;
    _address.byteAddress= (_DI_address >> 4);

    return bitRead(i2c_in[_address.byteAddress],_address.bitAddress);

}

int i2c_in_update(){
    
    /* recuperation expander i2c */
    for(int i=0; i<I2C_IN_NUMBER; i++){
        i2c_in[i]=expanderRead(i);
        //~ Serial.print("lu: ");
        //~ Serial.println(expanderRead(i), BIN);
        //~ Serial.println(i2c_in[i], BIN);
    }
}

/* Create one global object */
byte i2c_in[I2C_IN_NUMBER]; 
