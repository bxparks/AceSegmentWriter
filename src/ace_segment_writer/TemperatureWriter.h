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

#ifndef ACE_SEGMENT_WRITER_TEMPERATURE_WRITER_H
#define ACE_SEGMENT_WRITER_TEMPERATURE_WRITER_H

#include <stdint.h>
#include "PatternWriter.h"
#include "NumberWriter.h"

namespace ace_segment {

/**
 * The TemperatureWriter supports writing integer temperature values in Celcius
 * or Farenheit. Negative values are supported.
 *
 * @tparam T_LED_MODULE the class of the underlying LED Module, often LedModule
 *    but other classes with the same generic public methods can be substituted
 */
template <typename T_LED_MODULE>
class TemperatureWriter {
  public:
    /**
     * Constructor.
     *
     * @param ledModule instance of LedModule
     */
    explicit TemperatureWriter(T_LED_MODULE& ledModule) :
        mNumberWriter(ledModule)
    {}

    /** Get the underlying LedModule. */
    T_LED_MODULE& ledModule() { return mNumberWriter.ledModule(); }

    /** Get the underlying PatternWriter. */
    PatternWriter<T_LED_MODULE>& patternWriter() {
      return mNumberWriter.patternWriter();
    }

    /** Reset cursor to home. */
    void home() { mNumberWriter.home(); }

    /**
     * Write signed integer temperature without deg or unit within the boxSize.
     */
    uint8_t writeTemp(int16_t temp, int8_t boxSize = 0) {
      return mNumberWriter.writeSignedDecimal(temp, boxSize);
    }

    /**
     * Write integer temperature with degree symbol.
     *
     * The caller is responsible for verifying that the temp value fits inside
     * the `boxSize`. For example, if boxSize is 2, then the range of temp value
     * is [-9, 99]. If the boxSize is too small, the digits will bleed to the
     * right of the box.
     *
     * @return number of digits written, including any '-' or space characters
     */
    uint8_t writeTempDeg(int16_t temp, int8_t boxSize = 0) {
      uint8_t written = mNumberWriter.writeSignedDecimal(
          temp, boxSize >= 1 ? boxSize - 1 : 0);
      patternWriter().writePattern(kPatternDeg);
      return written + 1;
    }

    /**
     * Write integer temperature with degree symbol and 'C' symbol.
     */
    uint8_t writeTempDegC(int16_t temp, int8_t boxSize = 0) {
      uint8_t written = mNumberWriter.writeSignedDecimal(
          temp, boxSize >= 2 ? boxSize - 2 : 0);
      patternWriter().writePattern(kPatternDeg);
      patternWriter().writePattern(kPatternC);
      return written + 2;
    }

    /**
     * Write integer temperature with degree symbol and 'F' symbol.
     */
    uint8_t writeTempDegF(int16_t temp, int8_t boxSize = 0) {
      uint8_t written = mNumberWriter.writeSignedDecimal(
          temp, boxSize >= 2 ? boxSize - 2 : 0);
      patternWriter().writePattern(kPatternDeg);
      patternWriter().writePattern(kPatternF);
      return written + 2;
    }

    /** Clear the entire display. */
    void clear() { mNumberWriter.clear(); }

    /** Clear the display from `pos` to the end. */
    void clearToEnd() { mNumberWriter.clearToEnd(); }

  private:
    // disable copy-constructor and assignment operator
    TemperatureWriter(const TemperatureWriter&) = delete;
    TemperatureWriter& operator=(const TemperatureWriter&) = delete;

    NumberWriter<T_LED_MODULE> mNumberWriter;
};

} // ace_segment

#endif
