/*
 * Demo of NumberWriter class on a 4-digit LED module using the TM1637 chip.
 */

#include <Arduino.h>
#include <AceTMI.h> // SimpleTmi1637Interface
#include <AceSegment.h> // Tm1637Module
#include <AceSegmentWriter.h> // NumberWriter

using ace_tmi::SimpleTmi1637Interface;
using ace_segment::LedModule;
using ace_segment::Tm1637Module;
using ace_segment::PatternWriter;
using ace_segment::NumberWriter;

// Replace these with the PIN numbers of your dev board.
const uint8_t CLK_PIN = D5;
const uint8_t DIO_PIN = D7;
const uint8_t NUM_DIGITS = 4;

// Many TM1637 LED modules contain 10 nF capacitors on their DIO and CLK lines
// which are unreasonably high. This forces a 100 microsecond delay between
// bit transitions. If you remove those capacitors, you can set this as low as
// 1-5 micros.
const uint8_t DELAY_MICROS = 100;

using TmiInterface = SimpleTmi1637Interface;
TmiInterface tmiInterface(DIO_PIN, CLK_PIN, DELAY_MICROS);
Tm1637Module<TmiInterface, NUM_DIGITS> ledModule(tmiInterface);
PatternWriter<LedModule> patternWriter(ledModule);
NumberWriter<LedModule> numberWriter(patternWriter);

void setup() {
  delay(1000);

  tmiInterface.begin();
  ledModule.begin();

  ledModule.setBrightness(2);
}

void loop() {
  // Write "1234" decimal to the LED module.
  numberWriter.clear();
  numberWriter.writeDec4(1234);
  ledModule.flush();
  delay(500);

  // Write "-123" decimal to the LED module.
  numberWriter.clear();
  numberWriter.writeSignedDecimal(-123);
  ledModule.flush();
  delay(500);

  // Write "42", right justified in a box of 3 digits.
  numberWriter.clear();
  numberWriter.writeUnsignedDecimal(42, 3);
  ledModule.flush();
  delay(500);

  // Write "890A" hexadecimal number to the LED module.
  numberWriter.clear();
  numberWriter.writeHexWord(0x890A);
  ledModule.flush();
  delay(500);

  // Write "-1.23" floating point number.
  numberWriter.clear();
  numberWriter.writeFloat(-1.23);
  ledModule.flush();
  delay(500);

}
