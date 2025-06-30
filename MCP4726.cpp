/*MIT License

Copyright (c) 2025 Huzefa T

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/



#include "MCP47X6.h"

// constructor
MCP47X6::MCP47X6(TwoWire& bus, uint8_t addr, Resolution res)
  : _wire(bus)
  , _addr(addr)
  , _res(res)
  , _vref(VREF_VDD)
  , _gain(GAIN_1)
  , _pd(PD_NORMAL)
  , _vrefVolt(5.0f)
  , _lastCfg(0)
{ }

void MCP47X6::begin() {
  // nothing—Wire.begin() must be called in your sketch
}

// set reference
bool MCP47X6::setVref(Vref v, bool toEEPROM) {
  _vref = v;
  return writeConfig(toEEPROM);
}

// set buffer gain
bool MCP47X6::setGain(Gain g, bool toEEPROM) {
  _gain = g;
  return writeConfig(toEEPROM);
}

// set power-down mode
bool MCP47X6::setPowerDown(PowerDown pd, bool toEEPROM) {
  _pd = pd;
  return writeConfig(toEEPROM);
}

// write raw code
bool MCP47X6::writeCode(uint16_t code, bool toEEPROM) {
  code = clampCode(code);
  return writeMem(code, toEEPROM);
}

// write by voltage
bool MCP47X6::writeVoltage(float volts, bool toEEPROM) {
  uint16_t maxc = (1u << _res) - 1;
  uint16_t code = uint16_t((volts/_vrefVolt)*maxc + 0.5f);
  return writeCode(code, toEEPROM);
}

// read back volatile code
bool MCP47X6::readCode(uint16_t &code) {
  uint8_t n = 2 + (_res==RES8 ? 1 : 2);
  _wire.requestFrom(_addr, n);
  if(_wire.available() < n) return false;

  uint8_t cfgNV = _wire.read();
  uint8_t cfg   = _wire.read();
  _lastCfg = cfg;  

  if(_res == RES12) {
    uint8_t hi = _wire.read(), lo = _wire.read();
    code = ((uint16_t)hi<<4) | (lo>>4);
  }
  else if(_res == RES10) {
    uint8_t hi = _wire.read(), lo = _wire.read();
    code = ((uint16_t)hi<<2) | (lo>>6);
  }
  else {
    code = _wire.read();
  }
  return true;
}

// true while EEPROM busy (RDY/BSY==0)
bool MCP47X6::busy() {
  uint16_t tmp;
  if(!readCode(tmp)) return true;
  return ((_lastCfg & 0x80) == 0);
}

// clamp to max code
uint16_t MCP47X6::clampCode(uint16_t c) const {
  uint16_t m = (1u << _res) - 1;
  return (c > m ? m : c);
}

// write only config bits
bool MCP47X6::writeConfig(bool ee) {
  _wire.beginTransmission(_addr);
  uint8_t cmd = ee?0b011:0b010;  // 011=WriteAll EEPROM, 010=WriteVolatileMem
  uint8_t b0  = (cmd<<5)
              | (_vref<<3)
              | ((_pd & 3)<<1)
              | 0;           // gain unused here
  _wire.write(b0);

  // pad out data length
  if(_res==RES12) {
    _wire.write(0); _wire.write(0);
  }
  else if(_res==RES10) {
    _wire.write(0); _wire.write(0);
  }
  else {
    _wire.write(0);
  }
  return (_wire.endTransmission() == 0);
}

// write config + DAC code
bool MCP47X6::writeMem(uint16_t code, bool ee) {
  _wire.beginTransmission(_addr);
  uint8_t cmd = ee?0b011:0b010;
  uint8_t hdr = (cmd<<5)
              | (_vref<<3)
              | ((_pd & 3)<<2)
              | (_gain<<1);
  _wire.write(hdr);

  if(_res==RES12) {
    code &= 0x0FFF;
    _wire.write((code>>4)&0xFF);
    _wire.write((code<<4)&0xF0);
  }
  else if(_res==RES10) {
    code &= 0x03FF;
    _wire.write((code>>2)&0xFF);
    _wire.write((code<<6)&0xC0);
  }
  else {  // 8-bit
    _wire.write(code & 0xFF);
  }

  return (_wire.endTransmission() == 0);
}


/*
#include <Wire.h>
#include "MCP47X6.h"

MCP47X6 dac(Wire, 0x60, MCP47X6::RES12);

void setup() {
  // Initialize I2C bus at your chosen pins & speed:
  Wire.begin(21, 22, 400000);
  // Optionally set internal pullups:
  // pinMode(21, INPUT_PULLUP);
  // pinMode(22, INPUT_PULLUP);

  dac.begin();                   //Initialize DAC
  dac.setVrefVoltage(3.3f);      // if VDD=3.3V
  dac.setVref(MCP47X6::VREF_VDD); //Using VDD is VREF
  dac.setGain(MCP47X6::GAIN_1);
  dac.setPowerDown(MCP47X6::PD_NORMAL);

  // Example: write 1.234 V (volatile only)
  dac.writeVoltage(1.234f, false);
}

void loop() {
  // … your code …
}
*/
