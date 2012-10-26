#include "Arduino.h"
#include "shutter_xpl.h"
#include "shutter_core.h"
#include "shutter_hard.h"

//~ #include "dimmer.h"
#include <avr/pgmspace.h>
#include "net.h"
#include "EtherShield_broadcast.h"



#define LF 10

#define  string_P_write(string) while(pgm_read_byte(&string[j])!='\0')\
                                {\
                                    buf[UDP_DATA_P+i]=pgm_read_byte(&string[j]);\
                                    i++;\
                                    j++;\
                                }\
                                j=0;\

#define  string_write(string)   while(string[j]!='\0')\
                                {\
                                    buf[UDP_DATA_P+i]=string[j];\
                                    i++;\
                                    j++;\
                                }\
                                j=0;\

 //############################### GESTION SHUTTER ##################################
extern Shutter SHUTTER[];


extern uint8_t buf[udpPayload_SIZE+1];

extern char vendor_id[];        // vendor id
extern char device_id[];        // device id
extern char instance_id[];      // instance id


//~ char instance_id[18+2]="maison";        // instance id

/*
// envoie les parametres reconfigurables CONFIG_LIST STATUS en reponse a CONFIG_LIST CMND
void shutter_xpl_confList(){
  
    char buffer[180]="\0"; //149

    sprintf(buffer, "%s%s", buffer,"xpl-stat\n{\nhop=1\nsource=");
    sprintf(buffer, "%s%s", buffer,Config.vendor_id);
    sprintf(buffer, "%s%s", buffer,"-shutter.");
    sprintf(buffer, "%s%s", buffer,Config.instance_id);
    sprintf(buffer, "%s%s", buffer,"\ntarget=*\n}\nconfig.list\n{\nconf=device\nreconf=new\nreconf=port\nreconf=channel\n}\n");

    ///Udp1.sendPacket(buffer, Config.targetIp, Config.targetPort);
    
    Serial.print(millis());
    Serial.println(" - SEND shutter_xpl  config ----> ");

}
*/


/*  XPL-SHUTTER.BASIC Structure    
 *  shutter.basic
 *  {
 *  command=[goto|activate|deactivate]
 *  [network=ID]
 *  [[device=ID]|[scene=ID]]
 *  [channel=#]
 *  [level=0-100]
 *  [fade-rate=[default|<time>]
 *  }
 */
// analyse la partie commande d'un schema shutter.basic
int16_t shutter_basic(uint8_t *udpPayload, int16_t first_cmnd){

#define GOTO        0
#define DEVICE      1
#define LEVEL       2
//~ #define FADE        3
//~ #define TOGGLE      4
//~ #define LAST        5

int8_t cmnd=0; //donnees temporaire pour la recherche des commandes

int16_t i=first_cmnd;

char device[16+1]="\0"; // device id 128+1 originally
char temp[4+1]="\0";    // level ou fade-rate au format char pour conversion en int

int8_t int_level=0; //level
int8_t int_fade=0; //fade-rate

while(udpPayload[i]!='}'){
    //recherche commande
    if(udpPayload[i]=='c' &! bitRead(cmnd,GOTO)){
        i++;
        if(udpPayload[i]=='o'){i++;}else{return i;}
        if(udpPayload[i]=='m'){i++;}else{return i;}
        if(udpPayload[i]=='m'){i++;}else{return i;}
        if(udpPayload[i]=='a'){i++;}else{return i;}
        if(udpPayload[i]=='n'){i++;}else{return i;}
        if(udpPayload[i]=='d'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}
        if(udpPayload[i]=='g'){
            i++;
            if(udpPayload[i]=='o'){i++;}else{return i;}
            if(udpPayload[i]=='t'){i++;}else{return i;}
            if(udpPayload[i]=='o'){i++;}else{return i;}
            if(udpPayload[i]==LF ){i++;}else{return i;}
            bitSet(cmnd,GOTO);
            // Serial.println("GOTO");
        }
        //~ else if(udpPayload[i]=='t'){
            //~ i++;
            //~ if(udpPayload[i]=='o'){i++;}else{return i;}
            //~ if(udpPayload[i]=='g'){i++;}else{return i;}
            //~ if(udpPayload[i]=='g'){i++;}else{return i;}
            //~ if(udpPayload[i]=='l'){i++;}else{return i;}
            //~ if(udpPayload[i]=='e'){i++;}else{return i;}
            //~ if(udpPayload[i]==LF ){i++;}else{return i;}
            //~ bitSet(cmnd,TOGGLE);
            //~ // Serial.println("TOGGLE");
        //~ }
        else{return i;} 
    }
    else if(udpPayload[i]=='d' &! bitRead(cmnd,DEVICE)){
        i++;
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='v'){i++;}else{return i;}
        if(udpPayload[i]=='i'){i++;}else{return i;}
        if(udpPayload[i]=='c'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}
        
        for(int j=0; j<16; j++){
            if(udpPayload[i]==LF){device[j]='\0'; i++; break;}
            else{
                device[j]=udpPayload[i];
            }
            i++;
        }
    
        // while(udpPayload[i]!=LF){devicei++;}//on ignore le contenu du device pour le moment
        //stocker dans une variable intermediaire et rechercher le nom du device
        bitSet(cmnd,DEVICE);
        // Serial.println("DEVICE");
    }
    else if(udpPayload[i]=='l'&! bitRead(cmnd,LEVEL)){
        i++;
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='v'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='l'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}
        //~ if(udpPayload[i]=='l'){
            //~ i++;
            //~ if(udpPayload[i]=='a'){i++;}else{return i;}
            //~ if(udpPayload[i]=='s'){i++;}else{return i;}
            //~ if(udpPayload[i]=='t'){i++;}else{return i;}
            //~ if(udpPayload[i]==LF ){i++;}else{return i;}
            //~ bitSet(cmnd,LAST);
            //~ // Serial.println("LAST");
        //~ }
        //~ else{
        memset(temp, 0, sizeof(temp));
        int j=0;
        while(udpPayload[i]!=LF){
            if(j==4){return i;}
            temp[j]=udpPayload[i];
            i++;
            j++;
        }
        temp[j]='\0';
        i++;
        // while(udpPayload[i]!=LF){i++;}//on ignore le contenu du level pour le moment
        //stocker dans une variable intermediaire et convertir en type entier
        sscanf(temp, "%d", &int_level);
        //~ }
        bitSet(cmnd,LEVEL);
        // Serial.println("LEVEL");
    }
    //~ else if(udpPayload[i]=='f'&! bitRead(cmnd,FADE)){
        //~ i++;
        //~ if(udpPayload[i]=='a'){i++;}else{return i;}
        //~ if(udpPayload[i]=='d'){i++;}else{return i;}
        //~ if(udpPayload[i]=='e'){i++;}else{return i;}
        //~ if(udpPayload[i]=='-'){i++;}else{return i;}
        //~ if(udpPayload[i]=='r'){i++;}else{return i;}
        //~ if(udpPayload[i]=='a'){i++;}else{return i;}
        //~ if(udpPayload[i]=='t'){i++;}else{return i;}
        //~ if(udpPayload[i]=='e'){i++;}else{return i;}
        //~ if(udpPayload[i]=='='){i++;}else{return i;}
        //~ memset(temp, 0, sizeof(temp));
        //~ int j=0;
        //~ while(udpPayload[i]!=LF){
            //~ if(j==4){return i;}
            //~ temp[j]=udpPayload[i];
            //~ i++;
            //~ j++;
        //~ }
        //~ temp[j]='\0';
        //~ i++;
        //~ // while(udpPayload[i]!=LF){i++;}//on ignore le contenu du fade pour le moment
        //~ //stocker dans une variable intermediaire et convertir en type entier
        //~ sscanf(temp, "%d", &int_fade);
        //~ bitSet(cmnd,FADE);
        //~ // Serial.println("FADE");
    //~ }
    else{return i;} // pas de commande trouvee
    //mettre le tout dans une boucle pour scanner l'ensemble des commandes
        
    }

    //verification que commande goto, device et level sont present. Fade-rate est en option.
    if(bitRead(cmnd,GOTO) && bitRead(cmnd,DEVICE) && bitRead(cmnd,LEVEL)){
    
        ///ici, on dispose du nom du device et du level. Il faut donc executer l'action voulu: trouver le device dans la liste, et changer sa consigne
    
        //recherche du device shutter a modifier
        int position=shutter_find(device, 0);
        if(position>=0)
        {
            //~ // commande toggle
            //~ if(bitRead(cmnd,TOGGLE)){
                //~ Serial.print(device);
                //~ Serial.print(" -> set toggle command to ");
                //~ if(bitRead(cmnd,LAST)){
                //~ SHUTTER[position].toggle(0, 255, int_fade, 0);
                //~ Serial.println("last");
                //~ }else{
                //~ SHUTTER[position].toggle(0, int_level, int_fade, 0);
                //~ Serial.println(int_level);
                //~ }
//~ 
            //~ }
            //~ else
            //~ // commande classique
            //~ {
                SHUTTER[position].new_setpoint(int_level);
            #ifdef DEBUG_SHUTTER_XPL
                Serial.print(device);
                Serial.print(" -> new_sp command to ");
            #endif

                    SHUTTER[position].new_setpoint(int_level);
                    Serial.println(int_level);

            /// nota: le nouveau status est renvoye automatiquement dans une autre routine (shutter_status)
        }
        else
        {
        #ifdef DEBUG_SHUTTER_XPL
            Serial.print(device);
            Serial.println(" -> NOT FOUND !!!");
        #endif
        }
        //~ shutter_xpl_status(T_TRIG, device, 65);
        //~ delay(5000);
    #ifdef DEBUG_SHUTTER_XPL
        Serial.print("device= ");
        Serial.println(device);
        Serial.print("level= ");
        Serial.println(int_level,DEC);
        //~ Serial.print("fade= ");
        //~ Serial.println(int_fade);
    #endif
    
    return 0;
    }else{return i;}

}



const prog_uint8_t string_xpltrig[] PROGMEM = {"xpl-trig\n\0"};
const prog_uint8_t string_xplstat[] PROGMEM = {"xpl-stat\n\0"};
const prog_uint8_t string_xplcmnd[] PROGMEM = {"xpl-cmnd\n\0"};

const prog_uint8_t string_stat_source[] PROGMEM = {"{\nhop=1\nsource=xplduino-shutter.\0"};
const prog_uint8_t string_stat_target[] PROGMEM = {"\ntarget=*\n}\nshutter.device\n{\ndevice=\0"};
const prog_uint8_t string_stat_level[] PROGMEM = {"\nlevel=\0"};
const prog_uint8_t string_stat_end[] PROGMEM = {"\n}\n\0"};


// Broadcast the data
int shutter_status(byte _shutter_id, byte _type){

    int i=0;
    int j=0;

    char _current[3+1]="\0"; 

    sprintf(_current, "%s%d", _current, SHUTTER[_shutter_id].current*100/SHUTTER[_shutter_id].cycle_reference_time); ///TODO: convertir en %

    /* raz buffer */
    memset(buf,0,udpPayload_SIZE);

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

    string_write(SHUTTER[_shutter_id].name)

    string_P_write(string_stat_level)

    string_write(_current)

    string_P_write(string_stat_end)

#ifdef DEBUG_SHUTTER_XPL
    for(i=UDP_DATA_P;i<200;i++){
        Serial.print(buf[i]);
    if(buf[i]==0)
        break;
    }
#endif
    /* envoi du message */
    SendBroadcastData(buf, i, XPL_PORT);

#ifdef DEBUG_SHUTTER_XPL
    Serial.print("SH#stat-");
    Serial.print(SHUTTER[_shutter_id].name);
    Serial.print("-");
    Serial.println(_current);
#endif
    return i;

}
/*
// recherche le device dans la liste et renvoie son etat si trouvee
void shutter_xpl_request(struct_command *_command){
  
  
    if(!(strcmp(_command[0].value,"devstate")) && (!strcmp(_command[1].name,"device")))     // devstate
    {

    //on recherche le device a modifier
    int position=shutter_find(_command[1].value, 0);

        if(position>=0)
        {

            char _temp[10]="\0";  //Used to convert int to char
            sprintf(_temp, "%s%d", _temp, SHUTTER[position].current_request);
            shutter_xpl_status(STAT, _command[1].value, _temp);

            // trigger(_command[1].value, SHUTTER[position].current_request);

        }
        else
        {
            shutter_xpl_status(STAT, _command[1].value, "not-found");  //  the device is not founded
        }
    } 

}
*/

// recherche le device dans la liste et modifie ses parametres suite a un message CONFIG.RESPONSE 
int shutter_find(char *_name, int first){
    // on parcourt la liste des device shutter en commencant par la ligne "first"
    for(int i=first; i < SHUTTER_MAX; i++){

    #ifdef DEBUG_SHUTTER_XPL
        Serial.print(i);
        Serial.print("device '");
        Serial.print(_name);
        Serial.print(" compare to '");
        Serial.println(SHUTTER[i].name);
    #endif

        if(!strcmp(_name, SHUTTER[i].name))    // si device trouve alors on retourne le numero
        {
    #ifdef DEBUG_SHUTTER_XPL
        Serial.print("device '");
        Serial.print(_name);
        Serial.print(" ' has been found at position ");
        Serial.println(i);
    #endif
            return i;
        }
    }
    return -1; // si device pas trouve alors on retourne -1
}

const prog_uint8_t string_cmnd_target[] PROGMEM = {"\ntarget=gromain-shutter.\0"};
const prog_uint8_t string_cmnd_schema[] PROGMEM = {"\n}\nshutter.basic\n{\ncommand=goto\ndevice=\0"};
const prog_uint8_t string_cmnd_fade[] PROGMEM = {"\nfade-rate=\0"};

int shutter_xpl_cmdshutter(char *_instance_id, char *_device, byte _new_setpoint, byte _new_fade){

    int i=0;
    int j=0;


    char _current[3+1]="\0"; 
    char _fade[3+1]="\0"; 

    sprintf(_current, "%s%d", _current, _new_setpoint);
    sprintf(_fade, "%s%d", _fade, _new_fade);


    /* raz buffer */
    memset(buf,0,udpPayload_SIZE);

    /* construction du message type commande */
    string_P_write(string_xplcmnd)

    string_P_write(string_stat_source)

    string_write(instance_id)

    string_P_write(string_cmnd_target)
    
    string_write(_instance_id)

    string_P_write(string_cmnd_schema)

    string_write(_device)

    string_P_write(string_stat_level)

    string_write(_current)

    string_P_write(string_cmnd_fade)

    string_write(_fade)

    string_P_write(string_stat_end)

#ifdef DEBUG_SHUTTER_XPL
    for(i=UDP_DATA_P;i<200;i++){
        Serial.print(buf[i]);
    if(buf[i]==0)
        break;
    }
#endif

    /* envoi du message */
    SendBroadcastData(buf, i, XPL_PORT);

    return i;
#ifdef DEBUG_SHUTTER_XPL
    Serial.print(millis());
    Serial.print(" - SEND shutter_xpl command ----> ");
    Serial.print(_device);
    Serial.print(" current= ");
    Serial.print(_new_setpoint);
    Serial.println("%");
#endif
}


/*
void shutter_xpl_stopshutter(char *_vendor_id, char *_device_id, char *_instance_id, char *_device){
    
    char buffer[240]="\0"; // 213

    sprintf(buffer, "%s%s", buffer,"xpl-cmnd\n{\nhop=1\nsource=");
    sprintf(buffer, "%s%s", buffer,Config.vendor_id);
    sprintf(buffer, "%s%s", buffer,"-scenario.");
    sprintf(buffer, "%s%s", buffer,Config.instance_id);
    sprintf(buffer, "%s%s", buffer,"\ntarget=");
    sprintf(buffer, "%s%s", buffer,_vendor_id);
    sprintf(buffer, "%s%s", buffer,"-");
    sprintf(buffer, "%s%s", buffer,_device_id);
    sprintf(buffer, "%s%s", buffer,".");
    sprintf(buffer, "%s%s", buffer,_instance_id);
    sprintf(buffer, "%s%s", buffer,"\n}\nshutter.basic\n{\nrequest=stop\ndevice=");
    sprintf(buffer, "%s%s", buffer,_device);
    sprintf(buffer, "%s%s", buffer,"\n}\n");

    ///Udp1.sendPacket(buffer, Config.targetIp, Config.targetPort);
    
    Serial.print(millis());
    Serial.print(" - SEND shutter_xpl stop command ----> ");
    Serial.println(_device);

}
*/

/*
// recherche le device dans la liste et modifie ses parametres suite a un message CONFIG.RESPONSE 
void shutter_xpl_response(struct_command *_command){

    if(!(strcmp(_command[0].name,"device")) && (!strcmp(_command[1].name,"new")))   
    {

        //on recherche le device a modifier
        int position=shutter_find(_command[0].value, 0);
        
        if(position>=0)
        {
            // on recherche si le nouveau nom n existe pas deja
            if(shutter_find(_command[1].value, position+1)<0)
            {
            // TODO: ajouter une condition si champs vide (fin de la liste)
            // TODO: permettre de reaffecter le numero de pin (port+channel) depuis un message xpl

                //~ Serial.print("changed name of device '");
                //~ Serial.print(SHUTTER[position].name);

                sprintf(SHUTTER[position].name,"%s",_command[1].value);

                //~ Serial.print("' in position ");
                //~ Serial.print(position);
                //~ Serial.print(" to ");
                //~ Serial.println(SHUTTER[position].name);

                shutter_xpl_status(TRIG, _command[1].value, "ok");

                // return;
            }
            else
            {
                //~ Serial.print("device name'");
                //~ Serial.print(_command[1].value);
                //~ Serial.println("' already affect to a device !");

                shutter_xpl_status(TRIG, _command[0].value, "Er02");   //  the device is not founded or new name already assigned
            }

        }
        else
        {
            //~ Serial.print("device '");
            //~ Serial.print(_command[0].value);
            //~ Serial.println("' is not founded and can't be changed !");

            shutter_xpl_status(TRIG, _command[0].value, "Er01");   //  the device is not founded or new name already assigned
        }

#endif
    }
}
*/

