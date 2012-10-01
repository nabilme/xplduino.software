/*
GRADATEURS 230V (4 canaux, potentionnellement plus)
 
par Gromain59
 
 Partie materielle:
 - triac piloté par une sortie digitale via un optocoupleur
 - opto-coupleur AC pour la détection du passage à zéro de la phase
 
 Partie logicielle:
 -       une interruption materielle sur l'entrée 2 lors du passage à zéro de la phase
 -       une interruption logicielle qui intervient entre 100µs et 1400µs.
      => l'intervalle d'interruption est variable afin d'obtenir une courbe de luminosité linéaire selon la setpoint, du fait de la forme sinusoïdale du signal.
 on a donc:
 1. detection du passage à zero sur l'entree 2
 2. execution de zero_cross(): traitement des canaux avec une setpoint de 0% et de 100%
//                                               3. desactivation d'interruption materielle, 
    activation de l'interruption logicielle sur la base de retard[0]
 4. interruption au bout de retard[c2] µs (c2=0)
 5. execution de controle_canaux()
      => incrementation de l'index c2
              //                                                                              et si c2 est supérieur à 49, alors c'est le dernier cycle
            //                                                                                      => extinction de tous les canaux
            //                                                                                      => activation de l'interruption materielle
     //                                                                                        sinon:
      //                                                    => activation sorties des canaux ayant pour setpoint 98% (soit un retard de 469µs) ou si 
       => reconfiguration de l'interruption temporelle avec un nouveau retard, retard[c2]
            
Pour modifier la setpoint d'un channel, il faut envoyer via le serial monitor une trame de la forme:
            " D/0/45/F"
      => un espace
      => "D" pour indiquer le début de la trame
      => "/" comme séparateur
      => le channel concerné (de 0 à 7 ici)
      => "/" comme séparateur
      => le niveau souhaité (de 0% à 100%)
      => "/" comme séparateur      
      => "F" pour indiquer la fin de la trame
      
      Une fois la trame recu, la fonction sscanf se charge de recuperer les donnees.
      On convertit la setpoint recu en niveau (de 0 à 50 niveaux)
      
ressources: 
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1230333861/30 pour les premiers essais
http://www.hoelscher-hi.de/hendrik/english/dimmer.htm pour la partie electronique
http://www.abcelectronique.com/bigonoff/domocan.php?par=3efd4 pour la partie electronique et soft (PIC)
le forum arduino
 
 V0 = 1ere version fonctionnelle
 V4 = Ajout de la possibilité d'un délai de transition lors d'un changement de setpoint
 V5 = test avec memoire structure + configuration par MAXCANAL
 V6 = ajout champ nom dans structure Canal
*/


//~ #include <stdio.h>                   // pour le traitement de la trame contenant le changement de setpoint
#define F_CPU 8000000UL  // 8 MHz (* used for delay inside loop)
#include "util/delay.h" 
#include "avr/io.h"  
#include <avr/interrupt.h>
#include "usiTwiSlave.h"

#define I2C_SLAVE_ADDR  0x26
#define REG_SIZE 10

#define R_INPUT       0x50

#define R_CHANNEL0_SP   0x10
#define R_CHANNEL1_SP   0x11
#define R_CHANNEL2_SP   0x12
#define R_CHANNEL3_SP   0x13
#define R_CHANNEL0_FD   0x20
#define R_CHANNEL1_FD   0x21
#define R_CHANNEL2_FD   0x22
#define R_CHANNEL3_FD   0x23

#define W_CHANNEL0_SP   0x30
#define W_CHANNEL1_SP   0x31
#define W_CHANNEL2_SP   0x32
#define W_CHANNEL3_SP   0x33
#define W_CHANNEL0_FD   0x40
#define W_CHANNEL1_FD   0x41
#define W_CHANNEL2_FD   0x42
#define W_CHANNEL3_FD   0x43


// Somewhere to store the values the master writes to i2c register 2 and 3.
//~ static volatile uint8_t i2cReg[REG_SIZE];
//~ static volatile uint8_t indexReg=0;


#define OFFSET_TIME 1500    // delay before first timer interruption after zero cross detection
#define DELAY_COUNTER 70   // delay between two timer interruptions
#define MAX_TIMER_INTERRUPTION 100

// definition des 4 sorties dimmer (0 -> 3)

#define DIM0_DDR   DDRB
#define DIM0_PORT  PORTB
#define DIM0       PINB0

#define DIM1_DDR   DDRB
#define DIM1_PORT  PORTB
#define DIM1       PINB1

#define DIM2_DDR   DDRB
#define DIM2_PORT  PORTB
#define DIM2       PINB2

#define DIM3_DDR   DDRB
#define DIM3_PORT  PORTB
#define DIM3       PINB3

#define LED PORTD4
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LEDDDRPIN DDD4


#define MAX_CHANNEL 4                   // definir ici le nombre de canaux à gérer (8 par défaut)

int initChannel(void);
void setup(void);
void zero_cross(void);



/*
uint8_t retard[]= {
255, // 98% 225V 1469 us
159, // 96% 222V 1867 us
114, // 94% 220V 2154 us
93, // 92% 218V 2388 us
80, // 90% 215V 2589 us
72, //  V  us
65, // 88% 213V 2769 us
60, // 86% 210V 2933 us
57, // 84% 208VV ms us
54, // 82% 205V 3228 us
51, // 80% 202V 3363 us
49, // 78% 200V 3492 us
48, // 76% 197V 3616 us
46, // 74% 195V 3736 us
44, // 72% 192V 3852 us
44, // 70% 189V 3964 us
43, // 68% 186V 4074 us
42, // 66% 184VV ms us
42, // 64% 180V 4288 us
41, // 62% 178V 4393 us
40, // 60% 175V 4496 us
40, // 58% 172V 4598 us
40, // 56% 168V 4699 us
40, // 54% 166V 4800 us
40, // 52% 162V 4900 us
40, // 50% 159V 5000 us
40, // 48% 155V 5100 us
40, // 46% 152V 5201 us
40, // 44% 149V 5301 us
40, // 42% 145V 5402 us
41, // 40% 141V 5504 us
42, // 38% 138VV ms us
42, // 36% 133V 5712 us
43, // 34% 130VV ms us
44, // 32% 126VV ms us
44, // 30% 121V 6036 us
46, // 28% 117V 6148 us
48, // 26% 112V 6264 us
49, // 24% 107V 6384 us
51, // 22% 102V 6508 us
54, // 20% 97V 6637 us
57, // 18% 92VV ms us
60, // 16% 85V 6915 us
65, // 14% 79V 7067 us
72, // 12% 72V 7231 us
80, // 10% 64V 7411 us
93, // 8% 56V 7612 us
114, // 6% 46VV ms us
159, // 4% 32V 8132 us
160 //  2V zéro us
};
*/

typedef struct Canal Canal;
struct Canal
{
    uint8_t current;                   // setpoint
    uint8_t setpoint;                    // actual setpoint (0=100%, 50=0%)
    uint8_t fading_counter_setpoint;    // fading time (1=10 ms)
    uint8_t fading_counter_current;     // fading time counter

};

Canal channel[MAX_CHANNEL]; // instance creation


// A callback triggered when the i2c master attempts to read from a register.
uint8_t i2cReadFromRegister(uint8_t reg)
{

  switch ( reg )
  {
	case R_INPUT:
		reg=0xAB;
    break;
    
	case R_CHANNEL0_SP:
		reg=channel[0].current;
    break;
      
 	case R_CHANNEL1_SP:
		reg=channel[1].current;
    break;
      
 	case R_CHANNEL2_SP:
		reg=channel[2].current;
    break;
      
 	case R_CHANNEL3_SP:
		reg=channel[3].current;
    break;
    
	case R_CHANNEL0_FD:
		reg=channel[0].fading_counter_setpoint;
    break;
      
 	case R_CHANNEL1_FD:
		reg=channel[1].fading_counter_setpoint;
    break;
      
 	case R_CHANNEL2_FD:
		reg=channel[2].fading_counter_setpoint;
    break;
      
 	case R_CHANNEL3_FD:
		reg=channel[3].fading_counter_setpoint;
    break;

	default:
		reg=0xDA;
	}
	
    return reg;

}

// A callback triggered when the i2c master attempts to write to a register.
void i2cWriteToRegister(uint8_t reg, uint8_t value)
{

  switch ( reg )
  {  
	case W_CHANNEL0_SP:
		channel[0].setpoint=value;
    break;
      
 	case W_CHANNEL1_SP:
		channel[1].setpoint=value;
    break;
      
 	case W_CHANNEL2_SP:
		channel[2].setpoint=value;
    break;
      
 	case W_CHANNEL3_SP:
		channel[3].setpoint=value;
    break;
    
	case W_CHANNEL0_FD:
		channel[0].fading_counter_setpoint=value;
    break;
      
 	case W_CHANNEL1_FD:
		channel[1].fading_counter_setpoint=value;
    break;
      
 	case W_CHANNEL2_FD:
		channel[2].fading_counter_setpoint=value;
    break;
      
 	case W_CHANNEL3_FD:
		channel[3].fading_counter_setpoint=value;
    break;

	default:
		reg=0xCC;
	}

}



volatile uint8_t c1=0;      // index of channel
volatile uint8_t c2=MAX_TIMER_INTERRUPTION;      // phase delay counter

// _delay_ms uses a floating point datatype if you call
// that function in many places in your code then it becomes
// very fat. An integer is enough for us:
//
// delay x milliseconds:
void delay_ms(unsigned int xms)
{
        while(xms){
                _delay_ms(0.96);
                xms--;
        }
}


ISR(INT0_vect){
    
    zero_cross();
    
}

ISR(TIMER1_COMPA_vect){


    if (c2>0)      //Apres le 49 eme cycle, d'autre interruption arrivent (PB pas résolu) il faut donc empêcher le triac de s'allumer au 50 eme cycle car 50 = 0%
    {


        unsigned char sreg;

        /* Save global interrupt flag */
        sreg = SREG;
        /* Disable interrupts */
        cli();
               
        //~ OCR1A=retard[c2]; // new compare value
        OCR1A=DELAY_COUNTER; // new compare value
        
        /* Restore global interrupt flag */
        SREG = sreg;

        if(channel[0].current==c2){
            DIM0_PORT |= _BV(DIM0); //on
        }else{
            DIM0_PORT &= ~_BV(DIM0); //off
        }

        if(channel[1].current==c2){
            DIM1_PORT |= _BV(DIM1); //on
        }else{
            DIM1_PORT &= ~_BV(DIM1); //off
        }

        if(channel[2].current==c2){
            DIM2_PORT |= _BV(DIM2); //on
        }else{
            DIM2_PORT &= ~_BV(DIM2); //off
        }

        if(channel[3].current==c2){
            DIM3_PORT |= _BV(DIM3); //on
        }else{
            DIM3_PORT &= ~_BV(DIM3); //off
        }

        c2--;

    }
    else{

        DIM0_PORT &= ~_BV(DIM0); //off
        DIM1_PORT &= ~_BV(DIM1); //off
        DIM2_PORT &= ~_BV(DIM2); //off
        DIM3_PORT &= ~_BV(DIM3); //off

        TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12)); // last interrupt, stop the timer

    }

}


void setup(void) {    // Debut du setup

    // initialise l'interruption de temps Timer0
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;

    OCR1A=0;

    // initialise interruption sur INT0
    DDRD &= ~(1 << DDD2); // pin PD2 en entrée
    MCUCR = (1 << ISC00)|(1 << ISC01); // front descendant
    GIMSK  |= ( 1 << INT0 );  // Listen for INT

    sei();
 
}   // Fin du setup



void zero_cross() {              // Fonction associee à l'interruption 0                          

        LED_PORT|= _BV(LED);

    c2=MAX_TIMER_INTERRUPTION;

    unsigned char sreg;

    /* Save global interrupt flag */
    sreg = SREG;
    /* Disable interrupts */
    cli();
           
    OCR1A=OFFSET_TIME; // first compare value
 
    /* Restore global interrupt flag */
    SREG = sreg;
    
    TIMSK=(0<<TOIE1)|(1<<OCIE1A)|(0<<OCIE1B);           // activate the  comparator A
    TCCR1B=(0<<CS12)|(1<<CS11)|(0<<CS10)|(1<<WGM12);    // prescaler 1/8 and CTC mode   


      for (c1=0; c1 < MAX_CHANNEL; c1++){       // on parcourt les sorties pour verifier leur setpoint
            
            // fading: increase of light
         if(channel[c1].fading_counter_setpoint){
            if (channel[c1].current<channel[c1].setpoint){                                       // si la setpoint actuelle est inferieur a la setpoint demandee
                  channel[c1].fading_counter_current--;                                          // on decremente le compteur de transition
                  if (channel[c1].fading_counter_current<=0){                                    // et si celui ci est nul
                        channel[c1].current++;                                                   // on incremente la setpoint actuelle pour tendre a se rapprocher de la setpoint demandee
                        channel[c1].fading_counter_current=channel[c1].fading_counter_setpoint;  // et on re-initialise le compteur de transition
                  }
            }
            // fading: decrease of light
            if (channel[c1].current>channel[c1].setpoint){                                       // si la setpoint actuelle est superieur a la setpoint demandee
                  channel[c1].fading_counter_current--;                                          // on decremente le compteur de transition
                  if (channel[c1].fading_counter_current<=0){                                    // et si celui ci est nul
                        channel[c1].current--;                                                   // on decremente la setpoint actuelle pour tendre a se rapprocher de la setpoint demandee
                        channel[c1].fading_counter_current=channel[c1].fading_counter_setpoint;  // et on re-initialise le compteur de transition
                  }
            }
         }else{
            channel[c1].current=channel[c1].setpoint;
         }
      }

    if(channel[0].current==c2)
        DIM0_PORT |= _BV(DIM0); //on

    if(channel[1].current==c2)
        DIM1_PORT |= _BV(DIM1); //on

    if(channel[2].current==c2)
        DIM2_PORT |= _BV(DIM2); //on

    if(channel[3].current==c2)
        DIM3_PORT |= _BV(DIM3); //on

    c2--;

}                                 // End of zero cross detection
 
                       // Main Loop
int main(void)
{   

    /* enable pin as output */
    LED_DDR|= (1<<LEDDDRPIN);
        
    setup();
    initChannel();
       
    usiTwiSlaveInit(I2C_SLAVE_ADDR, i2cReadFromRegister, i2cWriteToRegister);

    sei();

 
    while (1) {



        // set output to 5V, LED on
        //~ LED_PORT|= _BV(LED);
        //~ delay_ms(1000);

        // led off, pin=0
        LED_PORT &= ~_BV(LED);

        delay_ms(1000);

   }
       return(0); 
}

int initChannel(void){ //on initialise les paramètres de chaque channel.

    // mode output
    DIM0_DDR |= _BV(DIM0);
    DIM1_DDR |= _BV(DIM1);
    DIM2_DDR |= _BV(DIM2);
    DIM3_DDR |= _BV(DIM3);
        
    // configuration du channel 0
    channel[0].current=0;
    channel[0].setpoint=0;
    channel[0].fading_counter_current=channel[0].fading_counter_setpoint=20;              //100 passages à zéro par secondes et 50 valeurs de setpoint possible donc pour 0 à 100% chauqe incrément de fading_counter_setpoint vaut une demi seconde

    // configuration du channel 1
    channel[1].current=0;
    channel[1].setpoint=0;
    channel[1].fading_counter_current=channel[1].fading_counter_setpoint=5;

    // configuration du channel 2
    channel[2].current=0;
    channel[2].setpoint=0;
    channel[2].fading_counter_current=channel[2].fading_counter_setpoint=5;

    // configuration du channel 3
    channel[3].current=0;
    channel[3].setpoint=0;
    channel[3].fading_counter_current=channel[3].fading_counter_setpoint=5;

    return 0;

}
