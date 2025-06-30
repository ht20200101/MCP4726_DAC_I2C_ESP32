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
