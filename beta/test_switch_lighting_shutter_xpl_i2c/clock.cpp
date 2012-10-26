#include "clock.h"

#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//~ #define LED_DDR   DDRD
//~ #define LED_PORT  PORTD
//~ #define LED       PIND4
//~ #define LED_PIN   PIND

volatile byte c10ms=0;
volatile byte c500ms=0;
volatile byte c1000ms=0;

byte pulse_100ms=0;
byte pulse_1000ms=0;
byte pulse_500ms=0;


ISR(TIMER2_COMPA_vect){
  TCCR2A = 0;
  c10ms++;
}

void clock_setup() {

 // Serial.begin(115200);
  
//~ LED_DDR |= _BV(LED);

TCCR2A = 0;

OCR2A=0x9C; //156

TIMSK2=(1<<OCIE2A)|(0<<TOIE2)|(0<<OCIE2B);
TCCR2A=(1<<CS22)|(0<<CS21)|(1<<CS20);
sei();
delay(1);

//~ set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

}

void clock_update() {

  pulse_100ms=0;
  pulse_1000ms=0;
  pulse_500ms=0;    
 
  if (c10ms>=100){ // 100 ms
    c10ms=0;
    c500ms++;
    c1000ms++;
    pulse_100ms=1;
  }
  
  if (c500ms>=5){ // 500 ms
    c500ms=0;
    pulse_500ms=1;
  }
  
  if (c1000ms>=10){ // 1 seconde
    c1000ms=0;
    pulse_1000ms=1;
  }
  
  if(pulse_500ms){
    //~ LED_PIN |= _BV(LED);  
  }
  
  //~ sleep_enable();          // enables the sleep bit in the mcucr register
}

