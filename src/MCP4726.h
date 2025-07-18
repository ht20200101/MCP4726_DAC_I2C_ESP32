MIT License

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
THE SOFTWARE.


#ifndef MCP47X6_H
#define MCP47X6_H

#include <Wire.h>

class MCP47X6 {
public:
  // 8-, 10- or 12-bit device
  enum Resolution { RES8 = 8, RES10 = 10, RES12 = 12 };

  // Reference voltage selection
  enum Vref {
    VREF_VDD              = 0,
    VREF_PIN_UNBUFFERED   = 1,
    VREF_PIN_BUFFERED     = 2
  };

  // Output buffer gain (only when using VREF pin)
  enum Gain { GAIN_1 = 0, GAIN_2 = 1 };

  // Power-down resistor to GND
  enum PowerDown {
    PD_NORMAL = 0,
    PD_1K     = 1,
    PD_100K   = 2,
    PD_500K   = 3
  };

  // ctor: supply your I2C bus, 7-bit address (0x60–0x67), and resolution
  MCP47X6(TwoWire& bus, uint8_t addr, Resolution res);

  // Optional—called once in setup() after Wire.begin(...)
  void begin();

  // Configuration setters (pass true to write also to EEPROM)
  bool setVref(Vref v, bool toEEPROM=false);
  bool setGain(Gain g, bool toEEPROM=false);
  bool setPowerDown(PowerDown pd, bool toEEPROM=false);

  // Write raw code or voltage (instant or to EEPROM)
  bool writeCode(uint16_t code, bool toEEPROM=false);
  bool writeVoltage(float volts, bool toEEPROM=false);

  // Read back the current volatile code
  bool readCode(uint16_t &code);

  // True while EEPROM busy after a write
  bool busy();

  // If using writeVoltage(), choose what full-scale volts means
  void setVrefVoltage(float v) { _vrefVolt = v; }

private:
  TwoWire&   _wire;
  uint8_t    _addr;
  Resolution _res;
  Vref       _vref;
  Gain       _gain;
  PowerDown  _pd;
  float      _vrefVolt;
  uint8_t    _lastCfg;

  uint16_t clampCode(uint16_t c) const;
  bool     writeConfig(bool toEEPROM);
  bool     writeMem(uint16_t code, bool toEEPROM);
};

#endif  // MCP47X6_H
