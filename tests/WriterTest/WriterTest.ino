#line 2 "WriterTest.ino"

/*
 * MIT License
 * Copyright (c) 2018 Brian T. Park
 */

#include <stdarg.h>
#include <Arduino.h>
#include <AUnit.h>
#include <AceSegment.h>
#include <ace_segment/testing/TestableLedModule.h>
#include <AceSegmentWriter.h>

using aunit::TestRunner;
using aunit::TestOnce;
using namespace ace_segment;
using namespace ace_segment::testing;

const uint8_t NUM_DIGITS = 4;

// TestableModule allocates 1 more byte than NUM_DIGITS to test buffer overflow.
TestableLedModule<NUM_DIGITS> ledModule;

PatternWriter<LedModule> patternWriter(ledModule);
NumberWriter<LedModule> numberWriter(patternWriter);
ClockWriter<LedModule> clockWriter(numberWriter);
TemperatureWriter<LedModule> temperatureWriter(numberWriter);
CharWriter<LedModule> charWriter(patternWriter);
StringWriter<LedModule> stringWriter(charWriter);

// ----------------------------------------------------------------------
// Tests for PatternWriter.
// ----------------------------------------------------------------------

class PatternWriterTest : public TestOnce {
  protected:
    void setup() override {
      patternWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    uint8_t* mPatterns;
};

testF(PatternWriterTest, writePattern) {
  patternWriter.writePattern(kPattern0);
  assertEqual(kPattern0, mPatterns[0]);
}

testF(PatternWriterTest, pos) {
  assertEqual(0, patternWriter.pos());
  patternWriter.pos(2);
  assertEqual(2, patternWriter.pos());
  patternWriter.writePattern(kPattern0);
  assertEqual(kPattern0, mPatterns[2]);
}

// ----------------------------------------------------------------------
// Tests for CharWriter.
// ----------------------------------------------------------------------

class CharWriterTest : public TestOnce {
  protected:
    void setup() override {
      charWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    uint8_t* mPatterns;
};

testF(CharWriterTest, writeChar) {
  charWriter.writeChar('0');
  assertEqual(kPattern0, mPatterns[0]);
  charWriter.writeChar('1');
  assertEqual(kPattern1, mPatterns[1]);
  charWriter.writeChar('2');
  assertEqual(kPattern2, mPatterns[2]);
  charWriter.writeChar('3');
  assertEqual(kPattern3, mPatterns[3]);

  // This should not write to mPatterns[4]
  mPatterns[4] = 0;
  charWriter.writeChar('a');
  assertEqual(0, mPatterns[4]);
}

// ----------------------------------------------------------------------
// Tests for StringWriter.
// ----------------------------------------------------------------------

class StringWriterTest : public TestOnce {
  protected:
    void setup() override {
      stringWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    void assertPatternsEqual(int n, ...) {
      va_list args;
      va_start(args, n);
      for (int i = 0; i < n; i++) {
        uint8_t pattern = va_arg(args, int);
        assertEqual(pattern, mPatterns[i]);
      }
      va_end(args);
    }

    uint8_t* mPatterns;
};

testF(StringWriterTest, writeString) {
  stringWriter.writeString(".1.2.3");

  // Should be (".", "1.", "2.", "3") as the 4 digits
  assertPatternsEqual(
    4,
    kPatternSpace | 0x80,
    kPattern1 | 0x80,
    kPattern2 | 0x80,
    kPattern3
  );
}

// ----------------------------------------------------------------------
// Tests for NumberWriter.
// ----------------------------------------------------------------------

class NumberWriterTest : public TestOnce {
  protected:
    void setup() override {
      numberWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    uint8_t* mPatterns;
};

testF(NumberWriterTest, writeDigit) {
  numberWriter.writeDigit(0);
  assertEqual(kPattern0, mPatterns[0]);

  numberWriter.writeDigit(0);
  numberWriter.setDecimalPointAt(1);
  assertEqual(kPattern0 | 0x80, mPatterns[1]);

  numberWriter.setDecimalPointAt(1, false);
  assertEqual(kPattern0, mPatterns[1]);
}

testF(NumberWriterTest, writeDigit_invalid_char_becomes_space) {
  numberWriter.writeDigit(255);
  assertEqual(kPatternSpace, mPatterns[0]);
}

testF(NumberWriterTest, writeDigit_outOfBounds_writes_nothing) {
  mPatterns[4] = 0;  // TestableLedModule allocates an extra byte to test this

  numberWriter.writeDigit(kDigitMinus);
  numberWriter.setDecimalPointAt(4);
  assertEqual(0, mPatterns[4]);
}

testF(NumberWriterTest, writeDigits) {
  const digit_t CHARS[] = {2, 3};
  numberWriter.writeDigits(CHARS, 2);

  assertEqual(kPattern2, mPatterns[0]);
  assertEqual(kPattern3, mPatterns[1]);
}

testF(NumberWriterTest, writeBcd) {
  numberWriter.writeBcd(0x12);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);

  numberWriter.writeBcd(0x34);
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);
}

testF(NumberWriterTest, writeDec2_pad_zero) {
  numberWriter.writeDec2(1);
  assertEqual(kPattern0, mPatterns[0]);
  assertEqual(kPattern1, mPatterns[1]);

  numberWriter.writeDec2(12);
  assertEqual(kPattern1, mPatterns[2]);
  assertEqual(kPattern2, mPatterns[3]);

  // Number is too big, so 2 spaces are written.
  numberWriter.clear();
  numberWriter.writeDec2(123, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
}

testF(NumberWriterTest, writeDec2_pad_space) {
  numberWriter.writeDec2(1, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPattern1, mPatterns[1]);

  numberWriter.clear();
  numberWriter.writeDec2(12, kPatternSpace);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);

  // Number is too big, so 2 spaces are written.
  numberWriter.clear();
  numberWriter.writeDec2(123, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
}

testF(NumberWriterTest, writeDec4_pad_zero) {
  numberWriter.writeDec4(1);
  assertEqual(kPattern0, mPatterns[0]);
  assertEqual(kPattern0, mPatterns[1]);
  assertEqual(kPattern0, mPatterns[2]);
  assertEqual(kPattern1, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(12);
  assertEqual(kPattern0, mPatterns[0]);
  assertEqual(kPattern0, mPatterns[1]);
  assertEqual(kPattern1, mPatterns[2]);
  assertEqual(kPattern2, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(123);
  assertEqual(kPattern0, mPatterns[0]);
  assertEqual(kPattern1, mPatterns[1]);
  assertEqual(kPattern2, mPatterns[2]);
  assertEqual(kPattern3, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(1234);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);

  // Number is too big, so 4 spaces are written.
  numberWriter.clear();
  numberWriter.writeDec4(12345);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
  assertEqual(kPatternSpace, mPatterns[2]);
  assertEqual(kPatternSpace, mPatterns[3]);
}

testF(NumberWriterTest, writeDec4_pad_space) {
  numberWriter.writeDec4(1, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
  assertEqual(kPatternSpace, mPatterns[2]);
  assertEqual(kPattern1, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(12, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
  assertEqual(kPattern1, mPatterns[2]);
  assertEqual(kPattern2, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(123, kPatternSpace);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPattern1, mPatterns[1]);
  assertEqual(kPattern2, mPatterns[2]);
  assertEqual(kPattern3, mPatterns[3]);

  numberWriter.clear();
  numberWriter.writeDec4(1234, kPatternSpace);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);

  // Number is too big, so 4 spaces are written.
  numberWriter.clear();
  numberWriter.writeDec4(12345);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
  assertEqual(kPatternSpace, mPatterns[2]);
  assertEqual(kPatternSpace, mPatterns[3]);
}

testF(NumberWriterTest, writeHexByte) {
  numberWriter.writeHexByte(0x1F);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPatternF, mPatterns[1]);
}

testF(NumberWriterTest, writeHexWord) {
  numberWriter.writeHexWord(0x12AB);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);
  assertEqual(kPatternA, mPatterns[2]);
  assertEqual(kPatternB, mPatterns[3]);
}

testF(NumberWriterTest, writeUnsignedDecimal) {
  uint8_t written = numberWriter.writeUnsignedDecimal(123);
  assertEqual(3, written);
  assertEqual(kPattern1, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);
  assertEqual(kPattern3, mPatterns[2]);

  // Even if it goes off the end of the LED module, the return value is the
  // number of characters that would have been written if the LED module was
  // long enough.
  written = numberWriter.writeUnsignedDecimal(123);
  assertEqual(3, written);
}

testF(NumberWriterTest, writeUnsignedDecimal_boxed) {
  uint8_t written = numberWriter.writeUnsignedDecimal(34, 4);
  assertEqual(4, written);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternSpace, mPatterns[1]);
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);

  written = numberWriter.writeUnsignedDecimal(34, 2);
  assertEqual(2, written);
}

testF(NumberWriterTest, writeSignedDecimal) {
  uint8_t written = numberWriter.writeSignedDecimal(-23);
  assertEqual(3, written);
  assertEqual(kPatternMinus, mPatterns[0]);
  assertEqual(kPattern2, mPatterns[1]);
  assertEqual(kPattern3, mPatterns[2]);
}

testF(NumberWriterTest, writeSignedDecimalAt_boxed) {
  uint8_t written = numberWriter.writeSignedDecimal(-12, 4);
  assertEqual(4, written);
  assertEqual(kPatternSpace, mPatterns[0]);
  assertEqual(kPatternMinus, mPatterns[1]);
  assertEqual(kPattern1, mPatterns[2]);
  assertEqual(kPattern2, mPatterns[3]);
}

// ----------------------------------------------------------------------
// Tests for ClockWriter.
// ----------------------------------------------------------------------

class ClockWriterTest: public TestOnce {
  protected:
    void setup() override {
      clockWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    uint8_t* mPatterns;
};

testF(ClockWriterTest, writeColon) {
  clockWriter.numberWriter().writeDec2(12);
  clockWriter.numberWriter().writeDec2(34);

  // no colon by default
  assertEqual(kPattern2, mPatterns[1]);

  // turn on colon
  clockWriter.writeColon(true); // turns it on
  assertEqual(kPattern2 | 0x80, mPatterns[1]);
}

testF(ClockWriterTest, writeHourMinute12) {
  clockWriter.writeHourMinute12(2, 34);
  assertEqual(kPatternSpace, mPatterns[0]); // space
  assertEqual(kPattern2 | 0x80, mPatterns[1]); // colon on by default
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);
}

testF(ClockWriterTest, writeHourMinute24) {
  clockWriter.writeHourMinute24(2, 34);
  assertEqual(kPattern0, mPatterns[0]); // zero
  assertEqual(kPattern2 | 0x80, mPatterns[1]); // colon on by default
  assertEqual(kPattern3, mPatterns[2]);
  assertEqual(kPattern4, mPatterns[3]);
}

// ----------------------------------------------------------------------
// Tests for TemperatureWriter.
// ----------------------------------------------------------------------

class TemperatureWriterTest: public TestOnce {
  protected:
    void setup() override {
      temperatureWriter.clear();
      mPatterns = ledModule.getPatterns();
    }

    uint8_t* mPatterns;
};

testF(TemperatureWriterTest, writeTempDeg) {
  uint8_t written = temperatureWriter.writeTempDeg(-9);
  assertEqual(3, written);
  assertEqual(kPatternMinus, mPatterns[0]); // -
  assertEqual(kPattern9, mPatterns[1]); // 9
  assertEqual(kPatternDeg, mPatterns[2]); // deg

  temperatureWriter.clear();
  written = temperatureWriter.writeTempDeg(-9 /*temp*/, 4 /*boxSize*/);
  assertEqual(4, written);
  assertEqual(kPatternSpace, mPatterns[0]); // space
  assertEqual(kPatternMinus, mPatterns[1]); // -
  assertEqual(kPattern9, mPatterns[2]); // 9
  assertEqual(kPatternDeg, mPatterns[3]); // deg
}

testF(TemperatureWriterTest, writeTempDegC) {
  uint8_t written = temperatureWriter.writeTempDegC(-9 /*temp*/, 4 /*boxSize*/);
  assertEqual(4, written);
  assertEqual(kPatternMinus, mPatterns[0]); // -
  assertEqual(kPattern9, mPatterns[1]); // 9
  assertEqual(kPatternDeg, mPatterns[2]); // deg
  assertEqual(kPatternC, mPatterns[3]); // C
}

testF(TemperatureWriterTest, writeTempDegF) {
  uint8_t written = temperatureWriter.writeTempDegF(-9 /*temp*/, 4 /*boxSize*/);
  assertEqual(4, written);
  assertEqual(kPatternMinus, mPatterns[0]); // -
  assertEqual(kPattern9, mPatterns[1]); // 9
  assertEqual(kPatternDeg, mPatterns[2]); // deg
  assertEqual(kPatternF, mPatterns[3]); // F
}

//-----------------------------------------------------------------------------

void setup() {
#if ! defined(EPOXY_DUINO)
  delay(1000); // Wait for stability on some boards, otherwise garage on Serial
#endif

  Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
  while (!Serial); // Wait until Serial is ready - Leonardo/Micro

#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif
}

void loop() {
  TestRunner::run();
}
