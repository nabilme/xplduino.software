/*       Part of the xplduino project - http://code.google.com/p/xplduino/
 *       Write by Romain TISSOT CHARLOD
 *       Contact: xplduino@gmail.com
 * 
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *       
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *       
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA
 * 
 *       version 0.2
 * 
 */
/********************************************************************************************
 * switch_lib
 * bibliotheque gerant les éclairages (bibliotheque lighting_lib)
 * version 0.0 beta
 *
 * init() => initialise les parametres de l'objet boutton
 * update() => sur appel de la fonction, reactualise l'etat du switch
 *
 ********************************************************************************************/

#include "Arduino.h"
#include "lighting_core.h"

//############################### GESTION LIGHTING ##################################
extern Lighting LIGHTING[];

#define DEBUG_LIGHTING_CORE

//constructor
Lighting::Lighting(){
}

int Lighting::init(char *_name, byte _parameter, byte _DO_address, byte _fade)
{
    
    parameter =_parameter;
    DO_address =_DO_address;    // affectation du n° de DO_address

    W_STATUS(false); // raz flag pour envoi nouvel etat0

    
    sprintf(name,"%s",_name);   // namenique de la variable

    if(_fade>0) // si fade autorisée
    {
        fade=_fade; // mode dimmer: temps de fade lors d'un changement niveau de canal (par pas de 10 ms) ex: 2 x 50 cycles de 10ms= 1s de transition
        W_FADE(true); // le fading est autorisee
    }
    else
    {
        fade=0; // mode dans dimmer
        W_FADE(false); // le fading est interdit
    }
    
    setpoint=OFF;       // consigne demandee
    last_setpoint=DEFAULT;      // consigne demandee


#ifdef DEBUG_LIGHTING_CORE
    Serial.print("L#");
    Serial.print(name);
    Serial.print("-c=");
    Serial.print(setpoint, DEC);
    Serial.print("-t=");
    Serial.print(fade, DEC);
    Serial.print("-p=");
    Serial.println(DO_address, DEC);
#endif

    return 0;
}



// name: toggle
// @param:
// byte _mini -> minimum level to applied
// byte _maxi -> maximum level to applied
// byte _fade -> fade rate to applied
// @return

// la fonction toggle est une fonction télérupteur évolué. Elle permet de basculer l'état de la lumiere entre une consigne mini et une consigne maxi.

int Lighting::toggle(byte _mini, byte _maxi, byte _fade, int _timer)
{

#ifdef DEBUG_LIGHTING_CORE
    Serial.println("toggle debut ---");
    Serial.println(setpoint);
    Serial.println(_mini, DEC);
    Serial.println(_maxi, DEC); 
    Serial.println("--- toggle fin");
#endif

    // si maxi = 255 alors on utilise la derniere valeur enregistre
    if(_maxi == 255){

        //~ _maxi= constrain(last_setpoint,5,100);
        _maxi= constrain(last_setpoint,0,100);
        
        if(setpoint>0)
        {
            new_setpoint(_mini, _fade);
        }
        else
        {
            new_setpoint(_maxi, _fade);
        }
        
    }
    else{
        if(R_LAST_DIRECTION())
        {
            new_setpoint(_mini, _fade);
        }
        else
        {
            new_setpoint(_maxi, _fade);
        }
    }

    return 0;
    
}

// name: new_setpoint
// @param:
// byte _new_setpoint -> new level to applied
// byte _fade -> fade rate to applied
// @return

// la fonction new_setpoint permet d'appliquer une nouvelle consigne à la lumière. Dans cette fonction, on met en forme les paramètres pour être compatible.


int Lighting::new_setpoint(byte _new_setpoint, byte _fade)  // force une nouvelle current sans tenir compte de la current actuelle
{

    // mise en forme bornage des paramètres
    _fade=constrain( _fade, 0, 255);
    _new_setpoint=constrain(_new_setpoint, 0, 100);

    // la nouvelle consigne est-elle différente de l'actuelle ?
    if(setpoint != _new_setpoint){

        //~ if(LIGHTING[_id].setpoint > _new_setpoint){W_LAST_DIRECTION(true);}else{W_LAST_DIRECTION(false);};
        W_LAST_DIRECTION(_new_setpoint > setpoint);
        
    #ifdef DEBUG_LIGHTING_CORE
        Serial.print("sens=");
        Serial.println(R_LAST_DIRECTION());
    #endif
        
        setpoint=_new_setpoint; // nouvelle consigne

        W_STATUS(false); // raz flag pour envoi nouvel etat
        W_SP_CHANGED(true); // la consigne a changé
        
        fade=_fade;

    return 0;

    }

    return 1;

}


int Lighting::extinction_light(boolean _pulse)
{

}

int Lighting::stop(void)        // force de l'arret de la variation
{

    //~ current_last=LIGHTING[_id].current_request=dimmer_stop(_id);

    return 0;
}




