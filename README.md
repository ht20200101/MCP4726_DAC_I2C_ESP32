  <h1>MCP47X6 Arduino Driver</h1>
  <p>
    A lightweight C++ driver for the Microchip MCP4706, MCP4716 &amp; MCP4726 family of I²C DACs.
    Designed for ESP32-S3 (or any Arduino-compatible) boards using the Arduino IDE.
  </p>

  <p>
    <a class="badge" href="https://github.com/yourusername/MCP47X6/releases/latest">
      <img src="https://img.shields.io/github/v/release/yourusername/MCP47X6" alt="Latest Release">
    </a>
    <a class="badge" href="https://github.com/yourusername/MCP47X6/blob/main/LICENSE">
      <img src="https://img.shields.io/github/license/yourusername/MCP47X6" alt="MIT License">
    </a>
  </p>

  <h2>Features</h2>
  <ul>
    <li>8-, 10- or 12-bit resolution</li>
    <li>Selectable reference: V<sub>DD</sub>, external VREF pin (buffered/unbuffered)</li>
    <li>Op-amp gain ×1 or ×2</li>
    <li>Three power-down pull-down resistor modes</li>
    <li>Volatile (instant) or EEPROM (persistent) writes</li>
    <li>Read back current DAC code and EEPROM busy status</li>
    <li>Accepts any TwoWire instance (e.g. <code>Wire</code>, <code>Wire1</code>)</li>
  </ul>

  <h2>Requirements</h2>
  <ul>
    <li>Arduino IDE (1.8.x or later)</li>
    <li>ESP32-S3 board definitions installed</li>
    <li>TwoWire (Wire.h)</li>
  </ul>

  <h2>Installation</h2>
  <ol>
    <li>Clone or download into your Arduino <code>libraries/</code> folder (or project folder):
      <pre><code>git clone https://github.com/yourusername/MCP47X6.git</code></pre>
    </li>
    <li>Ensure <code>MCP47X6.h</code> &amp; <code>MCP47X6.cpp</code> are alongside your sketch or in <code>libraries/MCP47X6/src/</code>.</li>
    <li>Include in your sketch:
      <pre><code>#include &lt;Wire.h&gt;
#include "MCP47X6.h"</code></pre>
    </li>
  </ol>

  <h2>Usage</h2>
  <h3>Initialize I²C</h3>
  <pre><code>void setup() {
  // ESP32-S3 default I2C pins: SDA=21, SCL=22
  Wire.begin(21, 22, 400000);
}</code></pre>

  <h3>Configure &amp; Write</h3>
  <pre><code>#include &lt;Wire.h&gt;
#include "MCP47X6.h"

MCP47X6 dac(Wire, 0x60, MCP47X6::RES12);

void setup() {
  Wire.begin(21, 22, 400000);
  dac.begin();
  dac.setVrefVoltage(3.3f);      // full scale = 3.3 V
  dac.setVref(MCP47X6::VREF_VDD);
  dac.setGain(MCP47X6::GAIN_1);
  dac.setPowerDown(MCP47X6::PD_NORMAL);

  // write raw code 2048 (~1.65 V) to volatile register
  dac.writeCode(2048, false);

  // write 2.5 V to EEPROM (persistent)
  dac.writeVoltage(2.5f, true);
}

void loop() {
  // your code
}</code></pre>

  <h2>API Reference</h2>
  <pre><code>// Constructor: bus, address (0x60–0x67), resolution (RES8/RES10/RES12)
MCP47X6(TwoWire& bus, uint8_t addr, MCP47X6::Resolution res);

// Optional—call after Wire.begin()
void begin();

// Config (toEEPROM = true writes to EEPROM)
bool setVref(Vref v, bool toEEPROM=false);
bool setGain(Gain g, bool toEEPROM=false);
bool setPowerDown(PowerDown pd, bool toEEPROM=false);

// Output (toEEPROM = true writes to EEPROM)
bool writeCode(uint16_t code, bool toEEPROM=false);
bool writeVoltage(float volts, bool toEEPROM=false);

// Read back current volatile code
bool readCode(uint16_t &code);

// Returns true while EEPROM busy
bool busy();

// Full-scale volts for writeVoltage()
void setVrefVoltage(float volts);
</code></pre>

  <h2>Contributing</h2>
  <ol>
    <li>Fork the repo</li>
    <li>Create a feature branch <code>git checkout -b feature/foo</code></li>
    <li>Commit your changes <code>git commit -am "Add foo"</code></li>
    <li>Push <code>git push origin feature/foo</code></li>
    <li>Open a Pull Request</li>
  </ol>

  <h2>License</h2>
  <p>
    This project is released under the <strong>MIT License</strong>.  
    See the <a href="LICENSE">LICENSE</a> file for full details.
  </p>

