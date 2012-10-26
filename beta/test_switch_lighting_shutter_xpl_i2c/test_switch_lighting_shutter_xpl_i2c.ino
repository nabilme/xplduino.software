//**************************************************************//
//  Name    : test.pde                                          //
//  Author  : R .TISSOT                                         //
//  Date    : 23/02/2012                                        //
//  Version : 0.0                                               //
//  Notes   : Code for testing library                          //
//          :                                                   //
//****************************************************************



#include <Wire.h>
#include "i2c.h"

#include "switch_core.h"
#include "switch_hard.h"
#include "i2c_input.h"
#include "i2c_output.h"

#include "lighting_xpl.h"
#include "lighting_core.h"
#include "lighting_hard.h"

#include "shutter_core.h"
#include "shutter_hard.h"

#include "EtherShield_broadcast.h"
#include "clock.h"
#include "MemoryFree.h"

/// à remplacer gromain par xplduino
char vendor_id[8+2]={"xplduino\0"};     // vendor id
char device_id[8+2]={"lighting\0"};     // device id
char instance_id[18+1]={"garage\0"};    // instance id

Lighting LIGHTING[LIGHTING_MAX];


Switch SWITCH[SWITCH_MAX];


Shutter SHUTTER[SHUTTER_MAX];


//~ #define R_DPRE0(i)              bitRead(SWITCH[i].parameter, ADDR_PRE0)
//~ #define R_DPRE1(i)              bitRead(SWITCH[i].parameter, ADDR_PRE1)
long temp=0;

extern byte pulse_100ms;
extern byte pulse_1000ms;
extern byte pulse_500ms;

//~ #define High 0
//~ #define Low 1

//~ // Packet udpPayload, must be big enough to packet and payload
//~ #define udpPayload_SIZE 300
//~ char buf[udpPayload_SIZE+1];


void setup() {

    //start serial
    Serial.begin(115200);

    delay(1000);

    setup_udp();

    //~ mux_setup();

    clock_setup();

    Wire.begin();

    SWITCH[0].init("bp_salon", 0x20, 0x00, 10); // bp_salon, origine carte MUX et type "on" si l'entrée est à 1, d'adresse "0x01", "on" si entrée à 1 plus de 10*100ms, sinon pulse.
    SWITCH[1].init("bp_cuisine", 0x20, 0x01, 10); 
    SWITCH[2].init("bp_2", 0x20, 0x02, 10); 
    SWITCH[3].init("bp_3", 0x20, 0x03, 10); 
    SWITCH[4].init("bp_4", 0x20, 0x04, 10); 
    SWITCH[5].init("bp_5", 0x20, 0x05, 10); 
    SWITCH[6].init("bp_6", 0x20, 0x06, 10); 
    SWITCH[7].init("bp_7", 0x20, 0x07, 5); 
    //~ SWITCH[8].init("bp_8", 0x20, 0x10, 10); 
    //~ SWITCH[9].init("bp_9", 0x20, 0x11, 10); 
    //~ SWITCH[10].init("bp_10", 0x20, 0x12, 10); 
    //~ SWITCH[11].init("bp_11", 0x20, 0x13, 10); 
    //~ SWITCH[12].init("bp_12", 0x20, 0x14, 10); 
    //~ SWITCH[13].init("bp_13", 0x20, 0x15, 10); 
    //~ SWITCH[14].init("bp_14", 0x20, 0x16, 10); 
    //~ SWITCH[15].init("bp_15", 0x20, 0x17, 10); 
    
//~ int Lighting::init(char *_name, byte _parameter, byte _DO_address, byte _fade)
    LIGHTING[0].init("grange", 0x20, 0x00, 10);
    LIGHTING[1].init("cuisine", 0x20, 0x01, 10);
    //~ LIGHTING[2].init("wc", 0x20, 0x02, 10);
    //~ LIGHTING[3].init("chambre", 0x20, 0x03, 10);
    //~ LIGHTING[4].init("garage", 0x20, 0x04, 10);
    //~ LIGHTING[5].init("buanderie", 0x20, 0x05, 10);
    //~ LIGHTING[6].init("bureau", 0x20, 0x06, 10);
    //~ LIGHTING[7].init("vestiaire", 0x20, 0x07, 10);
    //~ 
    //~ lighting_hard_init();

    SHUTTER[0].init(0x21,0x04, 0x05, "v_salon", 180); //type pulse
    SHUTTER[1].init(0x22,0x06, 0x07, "v_sdb", 180); //type permanent up/down
    SHUTTER[2].init(0x24,0x02, 0x03, "v_cuisine", 180); //type permanent on/off up/down
pinMode(4,OUTPUT);
    //~ SHUTTER[0].init(0x22,0x04, 0x05, "v_salon", 180); //type permanent
    //~ SHUTTER[1].init(0x22,0x06, 0x07, "vol_sdb", 180);


    Serial.print("free RAM in kB: ");
    Serial.println(freeMemory());



    //~ expanderWrite(_DO_address >> 4, i2c_out[_DO_address >> 4]);
//~ expanderWrite(0, B00000000);
//~ delay(2000);
    //~ LIGHTING[0].toggle(0,50,10,60);
    //~ LIGHTING[0].toggle(0,50,10,60);
    //~ LIGHTING[1].toggle(0,100,10,60);
    //~ LIGHTING[1].toggle(0,100,10,60);
    
    //test envoi d'une commande lighting vers un autre xplduino
    lighting_xpl_cmdlighting("garage", "grange", 50, 2);

    // initialise les volets au démarrage
    SHUTTER[0].reset();
    SHUTTER[1].reset();
    SHUTTER[2].reset();

}

void loop() {

    clock_update();

    loop_udp();
    
    if(pulse_100ms){
        //~ temp=millis();
        /* recupere les valeurs des multiplexeurs */
        i2c_in_update();

 //~ int expander_in=B0100000;  // Address with three address pins grounded. 
  //~ Serial.print("Addr input: B");
  //~ Serial.print(expander_in, BIN);
  //~ Serial.print(", Read: ");
  //~ Serial.println(expanderRead(expander_in), BIN);


        /* update des switchs */
        switch_hard();
        
        shutter_post();
        shutter_hard();
        
        //~ Serial.println(millis()-temp);
        //~ Serial.println(millis());
        
        if(bitRead(SWITCH[0].status, ADDR_PULSE))
            LIGHTING[0].toggle(0,50,10,60);
            //~ lighting_xpl_cmdlighting("garage", "wc", 50, 2);

        if(bitRead(SWITCH[1].status, ADDR_PULSE))
            LIGHTING[1].toggle(0,50,10,60);
            //~ lighting_xpl_cmdlighting("garage", "wc", 0, 2);

        
        if(bitRead(SWITCH[2].status, ADDR_PULSE))
                SHUTTER[0].opening();
        
        if(bitRead(SWITCH[3].status, ADDR_PULSE))
                SHUTTER[0].closing();
        
        if(bitRead(SWITCH[4].status, ADDR_PULSE))
               SHUTTER[1].opening();
        
        if(bitRead(SWITCH[5].status, ADDR_PULSE))
                SHUTTER[1].closing();
        
        if(bitRead(SWITCH[6].status, ADDR_PULSE))
                SHUTTER[2].opening();
        
        if(bitRead(SWITCH[7].status, ADDR_PULSE))
                SHUTTER[2].closing();

//eteint tout !
        if(bitRead(SWITCH[0].status, ADDR_ON_OSR)){
            for(int i=0; i < LIGHTING_MAX; i++){
                LIGHTING[i].new_setpoint(100,0);
            }
        }
        
        if(bitRead(SWITCH[0].status, ADDR_ON_OSF)){
            for(int i=0; i < LIGHTING_MAX; i++){
                LIGHTING[i].new_setpoint(0,0);
            }
        }
 
        i2c_out_update();
 
        /* update des lighting */
        lighting_hard();
        
        lighting_hard_status();
    }

    if(pulse_1000ms){

        shutter_status();

    }

}





