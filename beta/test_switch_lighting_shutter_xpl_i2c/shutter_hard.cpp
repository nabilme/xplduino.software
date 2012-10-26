#include "shutter_hard.h"
#include "shutter_core.h"

#include "config.h"

extern Shutter SHUTTER[];
extern int shutter_status(byte _shutter_id, byte _type);

/* this routin retreive the status of the digital input linked to the
 * shutter. The source is encoded in the 4 MSB bits of byte "parameter"
 */


int shutter_hard(){

    for(int i=0; i < SHUTTER_MAX; i++){

        byte type_source=(SHUTTER[i].parameter >> 4);

        if (type_source==1){ // carte d'entrée multiplexée
            //~ if (SHUTTER[i].update(mux_retreive(SHUTTER[i].DI_address))) // si l'update renvoi une valeur >0 alors c'est qu'il y a eu un chgt
                //~ shutter_status(i, T_TRIG); //TRIG 
        
        }
        else if (type_source==POST_PIN){ // digital output de la carte
            digitalWrite(SHUTTER[i].DO1, bitRead(SHUTTER[i].status, ADDR_CMD_DO1)); //TODO: revoir gestion DO address pour le shutter
            digitalWrite(SHUTTER[i].DO2, bitRead(SHUTTER[i].status, ADDR_CMD_DO2)); //TODO: revoir gestion DO address pour le shutter
            bitWrite(SHUTTER[i].status, ADDR_STATUS, true);
        }
        else if (type_source==POST_I2C){ // i2c

            i2c_out_write(SHUTTER[i].DO1, bitRead(SHUTTER[i].status, ADDR_CMD_DO1));
            i2c_out_write(SHUTTER[i].DO2, bitRead(SHUTTER[i].status, ADDR_CMD_DO2));

        } 
        else{

        }

    }

    return 0;

}

int shutter_post(){

    for(int i=0; i < SHUTTER_MAX; i++){
        SHUTTER[i].controle();
        SHUTTER[i].post();

    }

    return 0;

}

int shutter_status(){

    for(int i=0; i< SHUTTER_MAX; i++){
        
        if(!bitRead(SHUTTER[i].status, ADDR_STATUS)) // if is set equal to the processed (no change in the loop)
        { 
        shutter_status(i, T_TRIG); // TRIG
        }
        if(SHUTTER[i].current == SHUTTER[i].setpoint &! bitRead(SHUTTER[i].status, ADDR_STATUS)) // if is set equal to the processed (no change in the loop)
        {
            bitWrite(SHUTTER[i].status, ADDR_STATUS, true);
        }
    }

    return 0;

}


