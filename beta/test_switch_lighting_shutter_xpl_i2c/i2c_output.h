#ifndef i2c_output_h
#define i2c_output_h

#include <avr/io.h>
#include "Arduino.h"
#include <stdio.h>


#define I2C_OUT_NUMBER 2 // nombre de multiplexeur chaoutés (2 par carte digital 16 out)


extern byte i2c_out[I2C_OUT_NUMBER];
//~ extern byte expanderRead(outt _expander);

//~ #ifndef StructAddress TODO: corriger ce pb de redeclaration
//~ typedef struct StructAddress StructAddress;
//~ struct StructAddress
//~ {
    //~ byte byteAddress;
    //~ byte bitAddress;
//~ };
//~ #endif

//~ void mux_setup(); // setup
//~ byte mux_output(void); // recupere les valeurs d'un multiplexeur
byte i2c_out_retreive(byte _DO_address); // recupere la valeur d'une entree
byte  i2c_out_write(byte _DO_address, byte value); // ecrit dans le tampon i2c out
void i2c_out_update(void); // met à jour les pcf

#endif
