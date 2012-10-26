#include "switch_hard.h"
#include "switch_core.h"

#include "config.h"

extern Switch SWITCH[];
extern int switch_status(byte _switch_id, byte _type);

/* this routin retreive the status of the digital input linked to the
 * switch. The source is encoded in the 4 MSB bits of byte "parameter"
 */


int switch_hard(){

    for(int i=0; i < SWITCH_MAX; i++){

        byte type_source=(SWITCH[i].parameter >> 4);

        if (type_source==1){ // carte d'entrée multiplexée
            //~ if (SWITCH[i].update(mux_retreive(SWITCH[i].DI_address))) // si l'update renvoi une valeur >0 alors c'est qu'il y a eu un chgt
                switch_status(i, T_TRIG); //TRIG 
        
        }
        else if (type_source==0){ // digital input de la carte
            if (SWITCH[i].update(digitalRead(SWITCH[i].DI_address)))
                switch_status(i, T_TRIG); //TRIG 
        }
        else if (type_source==0x02){ // digital input de la carte
            if (SWITCH[i].update(i2c_in_retreive(SWITCH[i].DI_address))) // si l'update renvoi une valeur >0 alors c'est qu'il y a eu un chgt
                switch_status(i, T_TRIG); //TRIG
        }
        else{

        }

    }

    return 0;

}




