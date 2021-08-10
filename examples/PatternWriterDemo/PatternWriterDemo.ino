/*
 * Demo of PatternWriter class on a 4-digit LED module using the TM1637 chip.
 * PatternWriter provides a only a thin additional layer of functionality on top
 * of LedModule, but other Writer classes build upon this class.
 */

#include <Arduino.h>
#include <AceTMI.h> // SimpleTmiInterface
#include <AceSegment.h> // Tm1637Module
#include <AceSegmentWriter.h> // PatternWriter

using ace_tmi::SimpleTmiInterface;
using ace_segment::LedModule;
using ace_segment::Tm1637Module;
using ace_segment::PatternWriter;

// Replace these with the PIN numbers of your dev board.
const uint8_t CLK_PIN = A0;
const uint8_t DIO_PIN = 9;
const uint8_t NUM_DIGITS = 4;

// Many TM1637 LED modules contain 10 nF capacitors on their DIO and CLK lines
// which are unreasonably high. This forces a 100 microsecond delay between
// bit transitions. If you remove those capacitors, you can set this as low as
// 1-5 micros.
const uint8_t DELAY_MICROS = 100;

using TmiInterface = SimpleTmiInterface;
TmiInterface tmiInterface(DIO_PIN, CLK_PIN, DELAY_MICROS);
Tm1637Module<TmiInterface, NUM_DIGITS> ledModule(tmiInterface);

PatternWriter<LedModule> patternWriter(ledModule);

// LED segment patterns.
const uint8_t NUM_PATTERNS = 10;
const uint8_t PATTERNS[NUM_PATTERNS] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
};

void setup() {
  delay(1000);

  tmiInterface.begin();
  ledModule.begin();

  // Write "01.2" on the LED module. On clock LED modules, the decimal point
  // will be rendered as a colon so the output will look like "01:2"
  patternWriter.writePatternAt(0, PATTERNS[0]);
  patternWriter.writePatternAt(1, PATTERNS[1]);
  patternWriter.writePatternAt(2, PATTERNS[2]);
  patternWriter.writeDecimalPointAt(1);
  patternWriter.clearToEnd(3);

  ledModule.setBrightness(2);

  ledModule.flush();
}

void loop() {}

