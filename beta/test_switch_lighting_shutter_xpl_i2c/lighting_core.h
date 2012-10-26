#ifndef lighting_core_h
#define lighting_core_h

//~ #include "Arduino.h"
#include "Arduino.h"
// #include "config.h"      // contient les parametres propres a la carte

#define DEFAULT     100
#define OFF         0
#define REDUCE      50

#define ADDR_POST0  4   fonction de post-traitement associé (cf tableau)
#define ADDR_POST1  5   fonction de post-traitement associé (cf tableau)
#define ADDR_POST2  6   fonction de post-traitement associé (cf tableau)
#define ADDR_POST3  7   fonction de post-traitement associé (cf tableau)

#define ADDR_FADE           0   // 1=fade autorisée
#define ADDR_LAST_DIRECTION 1   // 1=derniere direction était UP
#define ADDR_STATUS         2   // 1=status envoyé
#define ADDR_SP_CHANGED     3   // 1=consigne a changé

#define R_FADE()            bitRead(status, ADDR_FADE)
#define R_LAST_DIRECTION()  bitRead(status, ADDR_LAST_DIRECTION)
#define R_STATUS()          bitRead(status, ADDR_STATUS)
#define R_SP_CHANGED()      bitRead(status, ADDR_SP_CHANGED)


#define W_FADE(value)               bitWrite(status, ADDR_FADE, value)
#define W_LAST_DIRECTION(value)     bitWrite(status, ADDR_LAST_DIRECTION, value)
#define W_STATUS(value)             bitWrite(status, ADDR_STATUS, value)
#define W_SP_CHANGED(value)         bitWrite(status, ADDR_SP_CHANGED, value)


class Lighting
{

    // int  setpoint;   // current demandee
    // byte last_setpoint;          // derniere valeur demandée
    // byte status;             // status (bit0: 1=changement de valeur en cours)
    // char     name[16+1];         // name de la variable
    // byte fade;   // fade rate
    // byte parameter;                  // ensemble de parametres
    // byte     DO_address;             // affecte une DO_address pour chaque canal.


    byte   last_setpoint;         // dernière consigne demandée
    //~ byte   max_setpoint;          // valeur maximum de la consigne (restriction)
    //~ int    timer;                 // timer d'extinction automatique

    public:
    byte   parameter;             // octet contenant un ensemble de paramètres (cf détails)
    byte   status;                // status (bit0: 1=changement de valeur  en cours)
    byte   DO_address;            // adresse Digital Output de la commande
    byte   fade;                  // fade rate par défaut (0=fade interdit)
    char   name[16+1];            // nom de l'instance
    int    setpoint;              // consigne demandee   
    
    Lighting(void); // constructor

    int init(char *_name, byte _parameter, byte _DO_address, byte _fade);
    int new_setpoint(byte _new_setpoint, byte _fade);   // force une nouvelle consigne sans tenir compte de la current actuelle
    int toggle(byte _mini, byte _maxi, byte _fade, int _timer); // bascule la sortie sur une nouvelle current mini ou maxi en fonction de la current actuelle
    int extinction_light(boolean _pulse);
    int stop(void);


};
    
#endif




