#include "lighting_hard.h"
#include "lighting_core.h"
#include "i2c_output.h"
#include "config.h"

extern Lighting LIGHTING[];
extern int lighting_status(byte _lighting_id, byte _type);
///provisoire
//~ #define MAX_LIGHTING 2
#define T_CMND 1
#define T_STAT 2
#define T_TRIG 3

int lighting_hard(){

    for(int i=0; i < LIGHTING_MAX; i++){

        if(bitRead(LIGHTING[i].status, ADDR_SP_CHANGED)){

            byte type_source=(LIGHTING[i].parameter >> 4);
//~ Serial.print(" source n");
//~ Serial.print(i);
//~ Serial.print("  =");
//~ Serial.println(LIGHTING[i].parameter >> 4);
            if (type_source==0x01){ // carte d'entrée multiplexée
                //~ if (LIGHTING[i].update(mux_retreive(LIGHTING[i].DI_address))) // si l'update renvoi une valeur >0 alors c'est qu'il y a eu un chgt
                    //~ LIGHTING_status(i, T_TRIG); //TRIG 
            
            }
            else if (type_source==0x03){ // dimmer
                //~ if (LIGHTING[i].update(digitalRead(LIGHTING[i].DI_address)))
                    //~ LIGHTING_status(i, T_TRIG); //TRIG 
            //~ }
            }
            else if (type_source==0x00){ // digital output de la carte
                digitalWrite(LIGHTING[i].DO_address, LIGHTING[i].setpoint);
                lighting_status(i, T_TRIG); //TRIG
                bitWrite(LIGHTING[i].status, ADDR_STATUS, true);
            }
            else if (type_source==0x02){ // i2c
            //~ Serial.println("i2c");
                i2c_out_write(LIGHTING[i].DO_address, LIGHTING[i].setpoint);
                lighting_status(i, T_TRIG); //TRIG
                bitWrite(LIGHTING[i].status, ADDR_STATUS, true);
            }            
            else{

            }
        bitWrite(LIGHTING[i].status, ADDR_SP_CHANGED, false); // on indique qu'on a traité le changement
        }
    }

    return 0;

}

int lighting_hard_init(){
 
    for(int i=0; i < LIGHTING_MAX; i++){

        if(LIGHTING[i].parameter >> 4 == 0) // si type PIN alors on configure la pin en sortie
        pinMode(LIGHTING[i].DO_address,OUTPUT);

    }

    return 0;
    
}

int lighting_hard_status(){

    for(int i=0; i < LIGHTING_MAX; i++){

        if(!bitRead(LIGHTING[i].status, ADDR_STATUS)){
            lighting_status(i, T_TRIG); //TRIG
            bitWrite(LIGHTING[i].status, ADDR_STATUS, true);
        }
        
    }

    return 0;

}
