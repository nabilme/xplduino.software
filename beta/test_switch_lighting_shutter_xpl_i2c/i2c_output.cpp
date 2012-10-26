#include "i2c_output.h"
#include "i2c.h"
#include <Wire.h>


extern byte i2c_out[];


// recupere la valeur d'une entree
byte i2c_out_retreive(byte _DO_address){

    // decodage DO_address
    byte _bitAddress= _DO_address & 0x0F;
    byte _byteAddress= (_DO_address >> 4);

    return bitRead(i2c_out[_byteAddress],_bitAddress);

}

byte i2c_out_write(byte _DO_address, byte _value){
    
    /* ecriture expander i2c */
    // decodage DO_address
    byte _bitAddress= _DO_address & 0x0F;
    byte _byteAddress= (_DO_address >> 4);
 
    bitWrite(i2c_out[_byteAddress],_bitAddress, _value);

    //~ expanderWrite(_byteAddress, i2c_out[_byteAddress]);

}

void i2c_out_update(void){
    
    /* ecriture expander i2c */
    // decodage DO_address
    for(int i=0; i<I2C_OUT_NUMBER; i++){
        expanderWrite(i, i2c_out[i]);
    }
}





/* Create one global object */
byte i2c_out[I2C_OUT_NUMBER]; 
