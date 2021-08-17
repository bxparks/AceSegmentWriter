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

#ifndef ACE_SEGMENT_WRITER_NUMBER_WRITER_H
#define ACE_SEGMENT_WRITER_NUMBER_WRITER_H

#include <stdint.h>
#include <AceCommon.h> // decToBcd()
#include "PatternWriter.h"

namespace ace_segment {

/** Total number of characters in the kHexCharPatterns[] set. */
const uint8_t kNumHexCharPatterns = 18;

/** Bit pattern map for hex characters. */
extern const uint8_t kHexCharPatterns[kNumHexCharPatterns];

/**
 * The type of the character set supported by many methods in the NumberWriter
 * class usually containing the string "HexChar" in its name. This custom
 * character set is not ASCII to save flash memory. It is a restricted set that
 * starts with 0 and goes up to 0xF to support hexadecimal digits. In addition,
 * the character set adds few more characters for convenience:
 *
 *  * kHexCharSpace = 0x10 = space character
 *  * kHexCharMinus = 0x11 = minus character
 *
 * The `hexchar_t` typedef is an alias for `uint8_t` and unfortunately, C++
 * will not prevent mixing a normal `char` or `uint8_t` with a `hexchar_t`.
 * However, it does make the documentation of the various methods more
 * self-explanatory.
 */
typedef uint8_t hexchar_t;

/** The code point in kHexCharPatterns[] corresponding to a space character. */
const hexchar_t kHexCharSpace = 0x10;

/** The code point in kHexCharPatterns[] corresponding to a minus character. */
const hexchar_t kHexCharMinus = 0x11;

/**
 * The NumberWriter supports converting decimal and hexadecimal numbers to
 * segment patterns expected by LedModule. The character set includes 0 to F,
 * and a few other characters which should be self-explanatory: kHexCharSpace
 * and kHexCharMinus.
 *
 * @tparam T_LED_MODULE the class of the underlying LED Module, often LedModule
 *    but other classes with the same generic public methods can be substituted
 */
template <typename T_LED_MODULE>
class NumberWriter {
  public:
    /** Constructor. */
    explicit NumberWriter(T_LED_MODULE& ledModule) :
        mPatternWriter(ledModule)
    {}

    /** Get the underlying LedModule. */
    T_LED_MODULE& ledModule() { return mPatternWriter.ledModule(); }

    /** Get the underlying PatternWriter. */
    PatternWriter<T_LED_MODULE>& patternWriter() { return mPatternWriter; }

    /**
     * Write the hex character `c` at position `pos`. If `c` falls outside the
     * valid range of the kHexCharPatterns set, a `kHexCharSpace` character is
     * printed instead.
     */
    void writeHexCharAt(uint8_t pos, hexchar_t c) {
      writeInternalHexCharAt(
          pos, ((uint8_t) c < kNumHexCharPatterns) ? c : kHexCharSpace);
    }

    /**
     * Write 2 hex characters given in the arguments. A convenience function
     * that seems to occur frequently when writing time or date components.
     */
    void writeHexChars2At(uint8_t pos, hexchar_t c, hexchar_t d) {
      writeHexCharAt(pos++ ,c);
      writeHexCharAt(pos, d);
    }

    /** Write the `len` hex characters given by `s` at LED `pos`. */
    void writeHexCharsAt(uint8_t pos, const hexchar_t s[], uint8_t len) {
      for (uint8_t i = 0; i < len; ++i) {
        writeHexCharAt(pos++, s[i]);
      }
    }

    /**
     * Write a 2-digit BCD number at position, which involves just printing the
     * number as a hexadecimal number. For example, 0x12 is printed as "12", but
     * 0x1A is printed as "1 ".
     */
    void writeBcd2At(uint8_t pos, uint8_t bcd) {
      uint8_t high = (bcd & 0xF0) >> 4;
      uint8_t low = (bcd & 0x0F);
      if (high > 9) high = kHexCharSpace;
      if (low > 9) low = kHexCharSpace;
      writeHexChars2At(pos, high, low);
    }

    /**
     * Write a 2-digit decimal number at position digit, right justified
     * with the given `padPattern` (default kPattern0). If the number is greater
     * than 100, then print "  " (2 spaces). Useful for month, day, hour,
     * minute, second.
     *
     * @param pos LED digit position
     * @param d decimal value to write
     * @param padPattern leading pattern, usually kPatternSpace or kPattern0
     */
    void writeDec2At(uint8_t pos, uint8_t d, uint8_t padPattern = kPattern0) {
      if (d >= 100) {
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
      } else {
        uint8_t tens = d / 10;
        uint8_t ones = d - 10 * tens;
        if (tens == 0) {
          mPatternWriter.writePatternAt(pos++, padPattern);
        } else {
          writeHexCharAt(pos++, tens);
        }
        writeHexCharAt(pos, ones);
      }
    }

    /**
     * Write the 4 digit decimal number `dd` at pos, right justified, padded
     * with a the `padPattern` (default kPattern0). If the number is greater
     * than 10000, print 4 spaces.
     */
    void writeDec4At(uint8_t pos, uint16_t dd, uint8_t padPattern = kPattern0) {
      if (dd >= 10000) {
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
        mPatternWriter.writePatternAt(pos++, kPatternSpace);
      } else {
        uint8_t high = dd / 100;
        uint8_t low = dd - high * 100;
        if (high == 0) {
          mPatternWriter.writePatternAt(pos++, padPattern);
          mPatternWriter.writePatternAt(pos++, padPattern);
          writeDec2At(pos, low, padPattern);
        } else {
          writeDec2At(pos, high, padPattern);
          pos += 2;
          writeDec2At(pos, low, kPattern0);
        }
      }
    }

    /** Write the 2-digit (8-bit) hexadecimal byte 'b' at pos. */
    void writeHexByteAt(uint8_t pos, uint8_t b) {
      uint8_t low = (b & 0x0F);
      b >>= 4;
      uint8_t high = (b & 0x0F);

      // We can use writeInternalHexCharAt() because low and high are guaranteed
      // to be a validate hex character.
      writeInternalHexCharAt(pos++, high);
      writeInternalHexCharAt(pos++, low);
    }

    /** Write the 4 digit (16-bit) hexadecimal word at pos. */
    void writeHexWordAt(uint8_t pos, uint16_t w) {
      uint8_t low = (w & 0xFF);
      uint8_t high = (w >> 8) & 0xFF;
      writeHexByteAt(pos, high);
      writeHexByteAt(pos + 2, low);
    }

    /**
     * Write the 16-bit unsigned number `num` as a decimal number at pos. The
     * lead padding character is always kPatternSpace.
     *
     * @param pos start position of the number
     * @param num unsigned decimal number, 0-65535
     * @param boxSize size of box. This is meant to be similar to the "%-5d" or
     *    "%5d" specifier to the printf() function. The actual digits may spill
     *    over the recommended box size.
     *    * 0 means no boxing, printing from left
     *    * > 0 means right justified inside box
     *    * < 0 means left justified inside box
     *
     * @return number of characters actually written, even if the characters
     *    bled over the end of the LED segments
     */
    uint8_t writeUnsignedDecimalAt(
        uint8_t pos,
        uint16_t num,
        int8_t boxSize = 0) {

      const uint8_t bufSize = 5;
      hexchar_t buf[bufSize];
      uint8_t start = toDecimal(num, buf, bufSize);

      return writeHexCharsInsideBoxAt(
          pos, &buf[start], bufSize - start, boxSize);
    }

    /** Same as writeUnsignedDecimalAt() but prepends a '-' sign if negative. */
    uint8_t writeSignedDecimalAt(
        uint8_t pos,
        int16_t num,
        int8_t boxSize = 0) {

      // Even -32768 turns into +32768, which is exactly what we want
      bool negative = num < 0;
      uint16_t absNum = negative ? -num : num;

      const uint8_t bufSize = 6;
      hexchar_t buf[bufSize];
      uint8_t start = toDecimal(absNum, buf, bufSize);
      if (negative) {
        buf[--start] = kHexCharMinus;
      }

      return writeHexCharsInsideBoxAt(
          pos, &buf[start], bufSize - start, boxSize);
    }

    /**
     * Write the decimal point for the pos. Clock LED modules will attach the
     * colon segment to one of the decimal points.
     */
    void writeDecimalPointAt(uint8_t pos, bool state = true) {
      mPatternWriter.writeDecimalPointAt(pos, state);
    }

    /** Clear the entire display. */
    void clear() { clearToEnd(0); }

    /**
     * Clear the display from `pos` to the end. Useful after calling
     * writeSignedDecimalAt() and writeUnsignedDecimalAt() which prints a
     * variable number of digits.
     */
    void clearToEnd(uint8_t pos) { mPatternWriter.clearToEnd(pos); }

  private:
    // disable copy-constructor and assignment operator
    NumberWriter(const NumberWriter&) = delete;
    NumberWriter& operator=(const NumberWriter&) = delete;

    /** Similar to writeHexCharAt() without performing bounds check. */
    void writeInternalHexCharAt(uint8_t pos, hexchar_t c) {
      uint8_t pattern = pgm_read_byte(&kHexCharPatterns[(uint8_t) c]);
      mPatternWriter.writePatternAt(pos, pattern);
    }

    /** Similar to writeHexCharsAt() without performing bounds check. */
    void writeInternalHexCharsAt(uint8_t pos, const hexchar_t s[],
        uint8_t len) {
      for (uint8_t i = 0; i < len; ++i) {
        writeInternalHexCharAt(pos++, s[i]);
      }
    }

    /**
     * Print the hex characters in `s` inside a recommended box of size
     * `boxSize` at position `pos`. If the number of digits is bigger, the
     * digits may spill over outside the box.
     *
     * @param boxSize if negative, left justified; if postive, right justified
     */
    uint8_t writeHexCharsInsideBoxAt(
        uint8_t pos,
        const hexchar_t s[],
        uint8_t len,
        int8_t boxSize) {

      uint8_t absBoxSize = (boxSize < 0) ? -boxSize : boxSize;

      // if the box is too small, print normally
      if (len >= absBoxSize) {
        writeInternalHexCharsAt(pos, s, len);
        return len;
      }

      // Print either left justified or right justified inside box
      uint8_t padSize = absBoxSize - len;
      if (boxSize < 0) {
        // left justified
        writeInternalHexCharsAt(pos, s, len);
        pos += len;
        while (padSize--) writeInternalHexCharAt(pos++, kHexCharSpace);
      } else {
        // right justified
        while (padSize--) writeInternalHexCharAt(pos++, kHexCharSpace);
        writeInternalHexCharsAt(pos, s, len);
      }

      return absBoxSize;
    }

    /**
     * Convert the integer num to an array of HexChar in the provided buf, with
     * the least significant digit going to buf[bufSize-1], and then working
     * backwards to the most significant digit.
     *
     * @param num number to convert
     * @param buf buffer of hex characters
     * @param bufSize must be 5 or larger (largest uint16_t is 65535, plus
     *    an optional sign bit if called from a signed version)
     *
     * @return index into buf that points to the start of the converted number,
     * e.g. for a single digit number, the returned value will be `bufSize-1`.
     */
    static uint8_t toDecimal(uint16_t num, hexchar_t buf[], uint8_t bufSize) {
      uint8_t pos = bufSize;
      while (true) {
        if (num < 10) {
          buf[--pos] = num;
          break;
        }
        uint16_t quot = num / 10;
        buf[--pos] = num - quot * 10;
        num = quot;
      }
      return pos;
    }

  private:
    PatternWriter<T_LED_MODULE> mPatternWriter;
};

}

#endif
