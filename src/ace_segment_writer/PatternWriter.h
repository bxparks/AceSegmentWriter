/*
MIT License

Copyright (c) 2021 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACE_SEGMENT_WRITER_PATTERN_WRITER_H
#define ACE_SEGMENT_WRITER_PATTERN_WRITER_H

#include <stdint.h>
#include <Arduino.h> // pgm_read_byte()

namespace ace_segment {

// List of patterns for certain convenient symbols

/** Digit 0 */
const uint8_t kPattern0 = 0b00111111;

/** Digit 1 */
const uint8_t kPattern1 = 0b00000110;

/** Digit 2 */
const uint8_t kPattern2 = 0b01011011;

/** Digit 3 */
const uint8_t kPattern3 = 0b01001111;

/** Digit 4 */
const uint8_t kPattern4 = 0b01100110;

/** Digit 5 */
const uint8_t kPattern5 = 0b01101101;

/** Digit 6 */
const uint8_t kPattern6 = 0b01111101;

/** Digit 7 */
const uint8_t kPattern7 = 0b00000111;

/** Digit 8 */
const uint8_t kPattern8 = 0b01111111;

/** Digit 9 */
const uint8_t kPattern9 = 0b01101111;

/** Digit A. Also used for the "A" in "AM". */
const uint8_t kPatternA = 0b01110111;

/** Digit B */
const uint8_t kPatternB = 0b01111100;

/** Digit C. Also used for "Celcius". */
const uint8_t kPatternC = 0b00111001;

/** Digit D */
const uint8_t kPatternD = 0b01011110;

/** Digit E */
const uint8_t kPatternE = 0b01111001;

/** Digit F. Also used for "Farenheit". */
const uint8_t kPatternF = 0b01110001;

/** Space. All blank. */
const uint8_t kPatternSpace = 0b00000000;

/** Minus sign. */
const uint8_t kPatternMinus = 0b01000000;

/** The superscript degree symbol for temperature. */
const uint8_t kPatternDeg = 0b01100011;

/** The "P" character for "PM". */
const uint8_t kPatternP = 0b01110011;

/**
 * Write LED segment patterns to the underlying LedModule. Other 'Writer'
 * classes provide additional functionality on top of this class (e.g.
 * NumberWriter, ClockWriter, TemperatureWriter, CharWriter, StringWriter).
 *
 * This class is stateless and does not contain any virtual functions. If the
 * method calls are made on the PatternWriter object directly, the compiler can
 * optimize away the indirection and call LedModule methods directly.
 *
 * @tparam T_LED_MODULE the class of the underlying LED Module, often LedModule
 *    but other classes with the same generic public methods can be substituted
 */
template <typename T_LED_MODULE>
class PatternWriter {
  public:
    /**
     * Constructor.
     * @param ledModule an instance of LedModule or one of its subclasses
     */
    explicit PatternWriter(T_LED_MODULE& ledModule) : mLedModule(ledModule) {}

    /** Return the underlying LedModule. */
    T_LED_MODULE& ledModule() const { return mLedModule; }

    /** Return the number of digits supported by this display instance. */
    uint8_t getNumDigits() const { return mLedModule.getNumDigits(); }

    /**
     * Write the pattern for a given pos. If the pos is out of bounds, the
     * method does nothing.
     */
    void writePatternAt(uint8_t pos, uint8_t pattern) {
      if (pos >= mLedModule.getNumDigits()) return;
      mLedModule.setPatternAt(pos, pattern);
    }

    /**
     * Write the array of `patterns` of length `len`, starting at `pos`. If an
     * element of patterns attempts to write to a digit beyond the last digit of
     * the LED module, nothing happens.
     *
     * The default implementation calls writePatternAt(), which should be
     * sufficient in most cases. Subclasses can override if needed.
     */
    void writePatternsAt(uint8_t pos, const uint8_t patterns[], uint8_t len) {
      for (uint8_t i = 0; i < len; i++) {
        if (pos >= mLedModule.getNumDigits()) break;
        mLedModule.setPatternAt(pos++, patterns[i]);
      }
    }

    /**
     * Write the array of `patterns` of length `len`, which are stored in flash
     * memory through PROGMEM, starting at `pos`. If an element of patterns
     * attempts to write to a digit beyond the last digit of the LED module,
     * nothing happens.
     *
     * The default implementation calls writePatternAt(), which should be
     * sufficient in most cases. Subclasses can override if needed.
     */
    void writePatternsAt_P(uint8_t pos, const uint8_t patterns[], uint8_t len) {
      for (uint8_t i = 0; i < len; i++) {
        if (pos >= mLedModule.getNumDigits()) break;
        uint8_t pattern = pgm_read_byte(patterns + i);
        mLedModule.setPatternAt(pos++, pattern);
      }
    }

    /**
     * Write the decimal point for the pos. Clock LED modules will attach the
     * colon segment to one of the decimal points.
     */
    void writeDecimalPointAt(uint8_t pos, bool state = true) {
      if (pos >= mLedModule.getNumDigits()) return;
      uint8_t pattern = mLedModule.getPatternAt(pos);
      if (state) {
        pattern |= 0x80;
      } else {
        pattern &= ~0x80;
      }
      mLedModule.setPatternAt(pos, pattern);
    }

    /** Clear the entire display. */
    void clear() { clearToEnd(0); }

    /** Clear the display from `pos` to the end. */
    void clearToEnd(uint8_t pos) {
      for (uint8_t i = pos; i < mLedModule.getNumDigits(); ++i) {
        mLedModule.setPatternAt(i, 0);
      }
    }

  private:
    // disable copy-constructor and assignment operator
    PatternWriter(const PatternWriter&) = delete;
    PatternWriter& operator=(const PatternWriter&) = delete;

  private:
    T_LED_MODULE& mLedModule;
};

} // ace_segment

#endif
