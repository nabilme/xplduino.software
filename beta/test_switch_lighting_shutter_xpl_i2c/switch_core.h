#ifndef switch_core_h
#define switch_core_h

#include "Arduino.h"
#include <stdio.h>

//~ #define MAX_SWITCH 16

#define T_CMND 1
#define T_STAT 2
#define T_TRIG 3

#define ADDR_TEMP           0   // 1=nouveau niveau de l'entree
#define ADDR_PULSE          1   // 1=appui pulse
#define ADDR_DPULSE         2   // 1=appui double pulse
#define ADDR_ON             3  // 1=appui maintenu
#define ADDR_ON_OSR         4   // 1=appui maintenu, sur un cycle
#define ADDR_ON_OSF         5   // 1=switch relaché, sur un cycle
#define ADDR_LEVEL          6  // 1=niveau precedent de l'entree


#define ADDR_HIGH           0   // 1=LOW, inverse le sens de l’entrée
#define ADDR_PRE0           4   // fonction de pré-traitement associé
#define ADDR_PRE1           5   // fonction de pré-traitement associé


#define R_LEVEL             bitRead(status, ADDR_LEVEL)
#define R_PULSE             bitRead(status, ADDR_PULSE)
#define R_DPULSE            bitRead(status, ADDR_DPULSE)
#define R_ON                bitRead(status, ADDR_ON)
#define R_ON_OSR            bitRead(status, ADDR_ON_OSR)
#define R_ON_OSF            bitRead(status, ADDR_ON_OSF)
#define R_TEMP              bitRead(status, ADDR_TEMP)

#define R_HIGH              bitRead(parameter, ADDR_HIGH)
#define R_PRE0              bitRead(parameter, ADDR_PRE0)
#define R_PRE1              bitRead(parameter, ADDR_PRE1)


#define W_LEVEL(value)      bitWrite(status, ADDR_LEVEL, value)
#define W_PULSE(value)      bitWrite(status, ADDR_PULSE, value)
#define W_DPULSE(value)     bitWrite(status, ADDR_DPULSE, value)
#define W_ON(value)         bitWrite(status, ADDR_ON, value)
#define W_ON_OSR(value)     bitWrite(status, ADDR_ON_OSR, value)
#define W_ON_OSF(value)     bitWrite(status, ADDR_ON_OSF, value)
#define W_TEMP(value)       bitWrite(status, ADDR_TEMP, value)

#define W_HIGH(value)       bitWrite(parameter, ADDR_HIGH, value)
#define W_PRE0(value)       bitWrite(parameter, ADDR_PRE0, value)
#define W_PRE1(value)       bitWrite(parameter, ADDR_PRE1, value)


class Switch
{
    public:

    //méthodes
    Switch();

    int init(char *_name, byte _parameter, byte _DI_address, byte _maintained_delay); // initialise les paramètres du switch
    int update(byte _new_level); // mis à jour de l'état de l'entrée
    int isPulse();  // renvoi l'état pulse
    int isDoublePulse();  // renvoi l'état double pulse
    int isOn();    // renvoi l'état On
    int isOnOSR(); // renvoi l'état du trigger Off -> On
    int isOff();   // renvoi l'état Off
    int isOnOSF(); // renvoi l'état du trigger On -> Off

    int config(); //provisoire, juste pour relire les valeurs et vérifier qu'elles sont correctement memorisees. TODO: a supprimer

    private:

    byte   maintained_delay;      // délai en ms button pour considérer comme maintenu
    byte   timer_maintained;      // compteur de temps incrémenté tant que l’entrée est à 1 (x100ms)
    byte   timer_doublepulse;     // compteur de temps décrémenté dès qu'une impulsion est détecté

    public:

    char   name[16+1];            // nom du switch
    byte   DI_address;            // entrée numérique de la fonction de pré-traitement
    byte   status;                // octet contenant les états calculés (cf détails)
    byte   parameter;             // octet contenant un ensemble de paramètres (cf détails)


};




#endif




