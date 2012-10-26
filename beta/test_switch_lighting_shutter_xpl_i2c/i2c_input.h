#ifndef i2c_input_h
#define i2c_input_h

#include <avr/io.h>
#include "Arduino.h"
#include <stdio.h>


#define I2C_IN_NUMBER 2 // nombre de multiplexeur chainés (2 par carte digital 16 in)


extern byte i2c_in[I2C_IN_NUMBER];
//~ extern byte expanderRead(int _expander);

#ifndef StructAddress
typedef struct StructAddress StructAddress;
struct StructAddress
{
    byte byteAddress;
    byte bitAddress;
};
#endif
//~ void mux_setup(); // setup
//~ byte mux_input(void); // recupere les valeurs d'un multiplexeur
byte i2c_in_retreive(byte _DI_address); // recupere la valeur d'une entree
int  i2c_in_update(void); // recupere les valeurs des multiplexeurs


#endif
