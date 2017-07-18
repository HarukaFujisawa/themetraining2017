
#include <Arduino.h>
#include <Wire.h>
#include "i2c_tool.h"

void writeI2C( byte dev_addr, byte addr, uint8_t cmd)
{
  writeI2C(dev_addr, addr, &cmd, 1);
}

void writeI2C( byte dev_addr, byte addr, uint8_t *cmd, size_t len)
{
  size_t i;
  for (i=0; i<len; i++) {
    Wire.beginTransmission(dev_addr);
    Wire.write(addr);
    Wire.write(cmd[i]);
    Wire.endTransmission();
    delayMicroseconds(27);    // 26.3us
  }
}

void readI2C( byte dev_addr, byte addr, uint8_t *dat, size_t len)
{
  Wire.beginTransmission(dev_addr);
  Wire.write(addr);
  Wire.endTransmission(false);

  int p = 0;
  Wire.requestFrom(dev_addr, len, true);
  while(Wire.available()) {
    dat[p++] = Wire.read();
  }
}


