// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define OFF       0x00

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

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  // write the new setpoint or fade rate
  Wire.beginTransmission(0x26); // transmit to device #0x26
  Wire.write(W_CHANNEL0_SP); // write command 1
  Wire.write((byte)OFF); // write command 
  Wire.endTransmission();    // stop transmitting
  
  // send the command to read a value
  Wire.beginTransmission(0x26); // transmit to device #0x26
  Wire.write(R_CHANNEL0_SP); // write command 1
  Wire.endTransmission();    // stop transmitting
  
  // read the value
  Wire.requestFrom(0x26, 1);    // request 6 bytes from slave device #2
  
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.println(c, DEC);         // print the character
  }

  
  delay(500);
}
