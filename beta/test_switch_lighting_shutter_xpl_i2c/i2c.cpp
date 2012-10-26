#include "i2c.h"
#include <Wire.h>

void expanderWrite(int _expander, byte _data ) {
  _expander = _expander | B0111000;  // Address with three address pins grounded.
  //~ _expander = B0111000;  // Address with three address pins grounded.
  Wire.beginTransmission(_expander);
  Wire.write(_data ^ 0xFF); // ecriture de l'octet apres inversion (niveau de sortie des pcf8574)
  Wire.endTransmission();
  
      //~ Serial.print(_expander, BIN);
      //~ Serial.print("/");
      //~ Serial.println(_data ^ 0xFF, BIN);
}

byte expanderRead(int _expander) {

  byte _data;
  _expander = _expander | B0100000;  // Address with three address pins grounded.
  Wire.requestFrom(_expander, 1);


  if(Wire.available()) {
    _data = Wire.read();
  }
  return _data;
}
 
