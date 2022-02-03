/*
 * Demo of StringWriter class on a 4-digit LED module using the TM1637 chip.
 */

#include <Arduino.h>
#include <AceTMI.h> // SimpleTmi1637Interface
#include <AceSegment.h> // Tm1637Module
#include <AceSegmentWriter.h> // StringWriter

using ace_tmi::SimpleTmi1637Interface;
using ace_segment::LedModule;
using ace_segment::Tm1637Module;
using ace_segment::CharWriter;
using ace_segment::StringWriter;

// Replace these with the PIN numbers of your dev board.
const uint8_t CLK_PIN = A0;
const uint8_t DIO_PIN = 9;
const uint8_t NUM_DIGITS = 4;

// Many TM1637 LED modules contain 10 nF capacitors on their DIO and CLK lines
// which are unreasonably high. This forces a 100 microsecond delay between
// bit transitions. If you remove those capacitors, you can set this as low as
// 1-5 micros.
const uint8_t DELAY_MICROS = 100;

using TmiInterface = SimpleTmi1637Interface;
TmiInterface tmiInterface(DIO_PIN, CLK_PIN, DELAY_MICROS);
Tm1637Module<TmiInterface, NUM_DIGITS> ledModule(tmiInterface);

CharWriter<LedModule> charWriter(ledModule);
StringWriter<LedModule> stringWriter(charWriter);

void setup() {
  delay(1000);

  tmiInterface.begin();
  ledModule.begin();

  // Write the string "the " to the LED module.
  stringWriter.writeStringAt(0, F("the "));

  ledModule.setBrightness(2);

  ledModule.flush();
}

void loop() {}

