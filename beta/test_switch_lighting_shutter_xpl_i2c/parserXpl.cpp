/*
 * xPL message parser v1.0 by Gromain59 - xplduino@gmail.com
 * This code is parsing a xPL message stored in 'received' udpPayload
 */

#include "parserXpl.h"

#include "lighting_xpl.h"
#include "shutter_xpl.h"
#include <stdio.h>
#include "MemoryFree.h"
#include "net.h"
#include "Arduino.h"

// define the identifier
#define CMND        0
#define TRIG        1
#define STAT        2
#define CONFIG      3
#define LIGHT       4
#define BASIC       5
#define LIST        6
#define RESPONSE    7
#define REQUEST     8
#define SHUT        9

#define COMMAND     12
#define HOP         13
#define SOURCE      14
#define TARGET      15

extern char vendor_id[];        // vendor id
extern char device_id[];        // device id
extern char instance_id[];      // instance id

#define LF 10


int parse(uint8_t *udpPayload, int len)
{

    //~ strcpy(vendor_id,"gromain");
    //~ strcpy(device_id,"lighting");
    //~ strcpy(instance_id,"garage");


int8_t lf=0; // nombre de retour à la ligne
int16_t i=UDP_DATA_P; //position du premier octet de date (depend du type de carte: presence ou non de l'entete du datagramme de 42 octets)
int16_t first_cmnd=0;//position de la premiere commande
int8_t t=0; //pour decomposition target
// int8_t cmnd=0; //compteur nombre de commande

int16_t msg=0; //stocke les details du message (type, schema...)

// controle de la premiere partie
if(udpPayload[i]=='x'){i++;}else{return i;}
if(udpPayload[i]=='p'){i++;}else{return i;}
if(udpPayload[i]=='l'){i++;}else{return i;}
if(udpPayload[i]=='-'){i++;}else{return i;}

//recherche type message
if(udpPayload[i]=='c'){
    i++;
    if(udpPayload[i]=='m'){i++;}else{return i;}
    if(udpPayload[i]=='n'){i++;}else{return i;}
    if(udpPayload[i]=='d'){i++;}else{return i;}
    bitSet(msg,CMND);
}
else if(udpPayload[i]=='s'){
    i++;
    if(udpPayload[i]=='t'){i++;}else{return i;}
    if(udpPayload[i]=='a'){i++;}else{return i;}
    if(udpPayload[i]=='t'){i++;}else{return i;}
    bitSet(msg,STAT);
}
else if(udpPayload[i]=='t'){
    i++;
    if(udpPayload[i]=='r'){i++;}else{return i;}
    if(udpPayload[i]=='i'){i++;}else{return i;}
    if(udpPayload[i]=='g'){i++;}else{return i;}
    bitSet(msg,TRIG);
}
else{return i;} // pas de message trouvee

if(udpPayload[i]==LF){i++; lf++;}else{return i;}

if(udpPayload[i]=='{'){i++;}else{return i;}

if(udpPayload[i]==LF){i++; lf++;}else{return i;}

//recherche hop, source et target
for(int j=0; j<3; j++){

    if(!bitRead(msg,HOP) && udpPayload[i]=='h'){ //recherche HOP

        i++;
        if(udpPayload[i]=='o'){i++;}else{return i;}
        if(udpPayload[i]=='p'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}
        
        while(udpPayload[i]!=LF){i++;}//on ignore le contenu du hop
        
        bitSet(msg,HOP);
        
        i++;
        lf++;
    }
    else if(!bitRead(msg,SOURCE) && udpPayload[i]=='s'){ //recherche SOURCE
        
        bitSet(msg,SOURCE);
        
        i++;
        if(udpPayload[i]=='o'){i++;}else{return i;}
        if(udpPayload[i]=='u'){i++;}else{return i;}
        if(udpPayload[i]=='r'){i++;}else{return i;}
        if(udpPayload[i]=='c'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}
        
        while(udpPayload[i]!=LF){i++;}// on ignore le contenu de la source
        
        i++;
        lf++;
    }
    else if(!bitRead(msg,TARGET) && udpPayload[i]=='t'){ //recherche TARGET
        
        bitSet(msg,TARGET);
        
        i++;
        if(udpPayload[i]=='a'){i++;}else{return i;}
        if(udpPayload[i]=='r'){i++;}else{return i;}
        if(udpPayload[i]=='g'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='t'){i++;}else{return i;}
        if(udpPayload[i]=='='){i++;}else{return i;}

        if(udpPayload[i]=='*'){

            i++;
            if(udpPayload[i]==LF){i++; lf++;}else{return i;}

        }
        else if(udpPayload[i]==vendor_id[0]){

            t=1;
            i++;
            while(vendor_id[t]!='\0'){
                if(udpPayload[i]==vendor_id[t]){i++; t++;}else{return i;}
            }

            if(udpPayload[i]=='-'){i++;}else{return i;}

            if(udpPayload[i]==device_id[0]){
                t=1;
                i++;
                while(device_id[t]!='\0'){
                    if(udpPayload[i]==device_id[t]){i++; t++;}else{return i;}
                }

            if(udpPayload[i]=='.'){i++;}else{return i;}
            }else{return i;}

            if(udpPayload[i]==instance_id[0]){
                t=1;
                i++;
                while(instance_id[t]!='\0'){
                    if(udpPayload[i]==instance_id[t]){i++; t++;}else{return i;}
                }
                if(udpPayload[i]==LF){i++;}else{return i;}
            }else{return i;}
        }
        else{
            return i;
        }

    }
    else{
        //~ Serial.println(":(");
        return i;
    } // manque un element (HOP, SOURCE, TARGET...)
}

if(udpPayload[i]=='}'){i++;}else{return i;} // fin du header

if(udpPayload[i]==LF){i++; lf++;}else{return i;}

//decrypter le type de schema
//config.list, config.response ou lighting.basic; lighting.request
//TODO: on pourrait scinder cette partie pour la rendre plus lisible: schema BUTTON, LIGHTING ou SHUTTER ? voir si c'est pertinent

if(udpPayload[i]=='l'){ //schema lighting

    i++;
    if(udpPayload[i]=='i'){i++;}else{return i;}
    if(udpPayload[i]=='g'){i++;}else{return i;}
    if(udpPayload[i]=='h'){i++;}else{return i;}
    if(udpPayload[i]=='t'){i++;}else{return i;}
    if(udpPayload[i]=='i'){i++;}else{return i;}
    if(udpPayload[i]=='n'){i++;}else{return i;}
    if(udpPayload[i]=='g'){i++;}else{return i;}
    if(udpPayload[i]=='.'){i++;}else{return i;}
    bitSet(msg,LIGHT);
    
    if(udpPayload[i]=='b'){ // lighting.basic ?

        i++;
        if(udpPayload[i]=='a'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='i'){i++;}else{return i;}
        if(udpPayload[i]=='c'){i++;}else{return i;}
    bitSet(msg,BASIC);
    }
    else if(udpPayload[i]=='r'){ // lighting.request ?

        i++;
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='q'){i++;}else{return i;}
        if(udpPayload[i]=='u'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='t'){i++;}else{return i;}
        bitSet(msg,REQUEST);

    }

}
else if(udpPayload[i]=='s'){ //schema lighting

    i++;
    if(udpPayload[i]=='h'){i++;}else{return i;}
    if(udpPayload[i]=='u'){i++;}else{return i;}
    if(udpPayload[i]=='t'){i++;}else{return i;}
    if(udpPayload[i]=='t'){i++;}else{return i;}
    if(udpPayload[i]=='e'){i++;}else{return i;}
    if(udpPayload[i]=='r'){i++;}else{return i;}
    if(udpPayload[i]=='.'){i++;}else{return i;}
    bitSet(msg,SHUT);
    
    if(udpPayload[i]=='b'){ // lighting.basic ?

        i++;
        if(udpPayload[i]=='a'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='i'){i++;}else{return i;}
        if(udpPayload[i]=='c'){i++;}else{return i;}
    bitSet(msg,BASIC);
    }
    else if(udpPayload[i]=='r'){ // lighting.request ?

        i++;
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='q'){i++;}else{return i;}
        if(udpPayload[i]=='u'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='t'){i++;}else{return i;}
        bitSet(msg,REQUEST);

    }

}
else if(udpPayload[i]=='c'){// schema config ? TODO: ces schemas pouvant etre commun aux BUTTON, LIGHTING et SHUTTER, il faudrait pouvoir les filtrer selon le contenu de TARGET

    i++;
    if(udpPayload[i]=='o'){i++;}else{return i;}
    if(udpPayload[i]=='n'){i++;}else{return i;}
    if(udpPayload[i]=='f'){i++;}else{return i;}
    if(udpPayload[i]=='i'){i++;}else{return i;}
    if(udpPayload[i]=='g'){i++;}else{return i;}
    if(udpPayload[i]=='.'){i++;}else{return i;}
    bitSet(msg,CONFIG);

    if(udpPayload[i]=='r'){ // config.response ?

        i++;
        if(udpPayload[i]=='e'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='p'){i++;}else{return i;}
        if(udpPayload[i]=='o'){i++;}else{return i;}
        if(udpPayload[i]=='n'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='e'){i++;}else{return i;}
        bitSet(msg,RESPONSE);
    }
    else if(udpPayload[i]=='l'){ // config.list ?

        i++;
        if(udpPayload[i]=='i'){i++;}else{return i;}
        if(udpPayload[i]=='s'){i++;}else{return i;}
        if(udpPayload[i]=='t'){i++;}else{return i;}
        bitSet(msg,LIST);
    }
}
else{
    return i;
}

if(udpPayload[i]==LF){i++; lf++;}else{return i;}

if(udpPayload[i]=='{'){i++;}else{return i;}//debut commande

if(udpPayload[i]==LF){i++; lf++; first_cmnd=i;}else{return i;}//memorisation debut commande

while(udpPayload[i]!='}'){ // tant que pas la fin de la sequence commande on compte le nombre de commande et leur integrite (x=y)

    if(udpPayload[i]=='=' &! bitRead(msg,COMMAND)){bitSet(msg,COMMAND);}
    else if(udpPayload[i]=='=' && bitRead(msg,COMMAND)){return i;}
    else if(udpPayload[i]==LF){lf++; bitClear(msg,COMMAND);}

    i++;

}

i++;

if(udpPayload[i]==LF){i++; lf++;}else{return i;}

if(udpPayload[i]!='\0'){return i;}

analyse_schema(udpPayload, msg, first_cmnd);


return 0;


}

// analyse le resultat du parsing pour action. Parametres: le buffer contenant le message, l'octet contenant les resultats du parsing, et la position du premier octet de la partie commande
void analyse_schema(uint8_t *udpPayload, int16_t msg, int16_t first_cmnd){

    //~ Serial.print(millis());
    //~ Serial.println(" - Lighting device target");

#ifdef lighting_activated
    if(bitRead(msg,LIGHT) && bitRead(msg,BASIC)) // demande lighting.basic
    {
        //~ Serial.println("   --> request for lighting lighting.basic");
        //~ Serial.println(lighting_basic(udpPayload, first_cmnd));
        lighting_basic(udpPayload, first_cmnd);
    }
    else
#endif
#ifdef shutter_activated
    if(bitRead(msg,SHUT) && bitRead(msg,BASIC)) // demande lighting.basic
    {
        //~ Serial.println("   --> request for shutter.basic");
        //~ Serial.println(shutter_basic(udpPayload, first_cmnd));
        shutter_basic(udpPayload, first_cmnd);
    }
    else
#endif
    if(bitRead(msg,CONFIG) && bitRead(msg,LIST)) // request for config.list
    {
        //~ Serial.println("   --> request for lighting config.list");
        // lighting_xpl_confList();
    }
    else
    if(bitRead(msg,LIGHT) && bitRead(msg,REQUEST)) // demande lighting.request
    {
        //~ Serial.println("   --> request for lighting lighting.request");
        // lighting_xpl_request(command); // lance analyse du schema
    }
    else
    if(bitRead(msg,CONFIG) && bitRead(msg,RESPONSE)) // demande config.response
    {
        //~ Serial.println("   --> request for lighting configuration");
        // lighting_xpl_response(command);
    }
    else
    {
        //~ Serial.println("   --> command unknown");
    }
}
