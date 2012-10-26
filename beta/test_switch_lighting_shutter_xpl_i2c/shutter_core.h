#ifndef shutter_core_h
#define shutter_core_h

#include "Arduino.h"

//~ #define shutter_lib_debug // uncomment to activate debug message

// type of command
#define POST_PIN 0x00
#define POST_MUX 0x01
#define POST_I2C 0x02

// address of the bits for parameter byte:
#define ADDR_TYPE_PULSE              0   // par impulsion
#define ADDR_TYPE_PERMANENT          1   // par maintien relais up et down
#define ADDR_TYPE_PERMANENT_COMB     2   // par maintien relais: variante à 1 relais marche et 1 relais sélection up ou down

// address of the bits for status byte: 
#define ADDR_OPEN               0   // open the shutter
#define ADDR_CLOSE              1   // close the shutter
#define ADDR_OPEN_PULSE         2   // pulse on the opening output
#define ADDR_CLOSE_PULSE        3   // pulse on the closing output
#define ADDR_STATUS             4   // status flag
#define ADDR_CMD_DO1            5   // open the shutter or start the motor
#define ADDR_CMD_DO2            6   // close the shutter or select the direction

// macro to read bits inside parameter byte:
#define R_TYPE_PULSE                 bitRead(parameter, ADDR_TYPE_PULSE)
#define R_TYPE_PERMANENT             bitRead(parameter, ADDR_TYPE_PERMANENT)
#define R_TYPE_PERMANENT_COMB        bitRead(parameter, ADDR_TYPE_PERMANENT_COMB)

// macro to read bits inside status byte:
#define R_OPEN                  bitRead(status, ADDR_OPEN)
#define R_CLOSE                 bitRead(status, ADDR_CLOSE)
#define R_OPEN_PULSE            bitRead(status, ADDR_OPEN_PULSE)
#define R_CLOSE_PULSE           bitRead(status, ADDR_CLOSE_PULSE)
#define R_STATUS                bitRead(status, ADDR_STATUS)
#define R_CMD_DO1               bitRead(status, ADDR_CMD_DO1)
#define R_CMD_DO2               bitRead(status, ADDR_CMD_DO2)

// macro to write bits inside status byte:
#define W_OPEN(value)           bitWrite(status, ADDR_OPEN, value)
#define W_CLOSE(value)          bitWrite(status, ADDR_CLOSE, value)
#define W_OPEN_PULSE(value)     bitWrite(status, ADDR_OPEN_PULSE, value)
#define W_CLOSE_PULSE(value)    bitWrite(status, ADDR_CLOSE_PULSE, value)
#define W_STATUS(value)         bitWrite(status, ADDR_STATUS, value)
#define W_CMD_DO1(value)        bitWrite(status, ADDR_CMD_DO1, value)
#define W_CMD_DO2(value)        bitWrite(status, ADDR_CMD_DO2, value)

class Shutter
{
    public:
    
    Shutter(void); // constructor

    uint8_t init(uint8_t _parameter, uint8_t _DO1, uint8_t _DO2, char *name, int _cycle_reference_time);
    uint8_t opening();
    uint8_t closing();
    uint8_t new_setpoint(int _setpoint);
    uint8_t controle();
    uint8_t stop();
    uint8_t post();
    uint8_t reset();

    int time_to_pourcent(int _value_to_convert, int _cycle_reference_time);
    int pourcent_to_time(int _value_to_convert, int _cycle_reference_time); 
   
    char    name[16+1];            // name of the shutter
    uint8_t parameter;             // parameter data
    uint8_t status;                // status data
    int     setpoint;              // setpoint (1=100ms)
    int     cycle_reference_time;  // reference time for a complete cycle (1=100ms)
    int     current;               // current position(1=100ms)
    uint8_t impulsion;             // time counter for the pulse function (1=100ms)
    uint8_t DO1;                   // Output 1 address
    uint8_t DO2;                   // Output 2 address




   private:
};

#endif

