#include "switch_xpl.h"
#include "switch_hard.h"
#include "switch_core.h"
//~ #include "mux_input.h"
#include <avr/pgmspace.h>

#include "EtherShield_broadcast.h"

extern Switch SWITCH[];
extern uint8_t buf[udpPayload_SIZE+1];


//~ extern char device_id[8+2];     // device id
extern char instance_id[];      // instance id
 ///provisoire
//~ char device_id[8+2]="switch";       // device id
//~ char instance_id[18+2]="maison";        // instance id


//~ #define UDP_DATA_P 0 ///provisoire

#define  string_P_write(string) while(pgm_read_byte(&string[j])!='\0')\
                                {\
                                    buf[UDP_DATA_P + i++]=pgm_read_byte(&string[j++]);\
                                }\
                                j=0;\

#define  string_write(string)   while(string[j]!='\0')\
                                {\
                                    buf[UDP_DATA_P + i++]=string[j++];\
                                }\
                                j=0;\

// chaines stockées en mémoire programme

const prog_uint8_t string_xpltrig[] PROGMEM = {"xpl-trig\n\0"};
const prog_uint8_t string_xplstat[] PROGMEM = {"xpl-stat\n\0"};
const prog_uint8_t string_xplcmnd[] PROGMEM = {"xpl-cmnd\n\0"};

const prog_uint8_t string_pulse[] PROGMEM = {"pulse\0"};
const prog_uint8_t string_dpulse[] PROGMEM = {"dpulse\0"};
const prog_uint8_t string_high[] PROGMEM = {"high\0"};
const prog_uint8_t string_low[] PROGMEM = {"low\0"};

const prog_uint8_t string_stat_source[] PROGMEM = {"{\nhop=1\nsource=xplduino-switch.\0"};
const prog_uint8_t string_stat_target[] PROGMEM = {"\ntarget=*\n}\nsensor.basic\n{\ncurrent=\0"};
const prog_uint8_t string_stat_device[] PROGMEM = {"\ndevice=\0"};
const prog_uint8_t string_stat_end[] PROGMEM = {"\n}\n\0"};


/* 
 * this routine prepares the xPL message to send 
 */


int switch_status(byte _switch_id, byte _type){

#ifdef DEBUG_SWITCH_XPL
        if(_type==T_TRIG){
            Serial.print("trigger of ");
        }

        Serial.print(SWITCH[_switch_id].name);
        Serial.print(": ");

        if(bitRead(SWITCH[_switch_id].status, ADDR_PULSE)){
            Serial.println("Pulse");
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_DPULSE)){
            Serial.println("Dpulse");
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_ON_OSR)){
            Serial.println("High");
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_ON_OSF)){
            Serial.println("Low");
        }
#endif

    int i=0;
    int j=0;

    /* construction du message */

    switch (_type)
    {

    case T_STAT:
        string_P_write(string_xplstat)
    break;

    case T_TRIG:
        string_P_write(string_xpltrig)
    break;

    default: 
    return 0; // error on message type, we don't send the message
    
    }

    string_P_write(string_stat_source)

    string_write(instance_id)

    string_P_write(string_stat_target)

        if(bitRead(SWITCH[_switch_id].status, ADDR_PULSE)){
            string_P_write(string_pulse)
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_DPULSE)){
            string_P_write(string_dpulse)
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_ON_OSR)){
            string_P_write(string_high)
        }
        else if(bitRead(SWITCH[_switch_id].status, ADDR_ON_OSF)){
            string_P_write(string_low)
        }

    string_P_write(string_stat_device)

    string_write(SWITCH[_switch_id].name)

    string_P_write(string_stat_end)

    /* envoi du message */
    SendBroadcastData(buf, i, XPL_PORT);

    // pour le debug du message
#ifdef DEBUG_SWITCH_XPL
    for(i=UDP_DATA_P;i<200;i++){
        Serial.print(buf[i]);
    if(buf[i]==0)
        break;
    }
#endif


    return 1;


}




