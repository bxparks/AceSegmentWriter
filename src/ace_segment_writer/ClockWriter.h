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

#ifndef ACE_SEGMENT_WRITER_CLOCK_WRITER_H
#define ACE_SEGMENT_WRITER_CLOCK_WRITER_H

#include <stdint.h>
#include "PatternWriter.h"
#include "NumberWriter.h"

namespace ace_segment {

/**
 * The ClockWriter writes "hh:mm" and "yyyy" to the LedModule. A few other
 * characters are supported.
 */
template <typename T_LED_MODULE>
class ClockWriter {
  public:
    /**
     * Constructor.
     *
     * @param numberWriter instance of NumberWriter<T_LED_MODULE>
     * @param colonDigit The digit which has the colon (":") character,
     *    mapped to bit 7 (i.e. 'H' segment). In many 4-digit LED clock
     *    display modules, this is digit 1 (counting from the left, 0-based,
     *    so the second digit from the left).
     */
    explicit ClockWriter(
        NumberWriter<T_LED_MODULE>& numberWriter,
        uint8_t colonDigit = 1
    ) :
        mNumberWriter(numberWriter),
        mColonDigit(colonDigit)
    {}

    /** Get the underlying LedModule. */
    T_LED_MODULE& ledModule() { return mNumberWriter.ledModule(); }

    /** Get the underlying PatternWriter. */
    PatternWriter<T_LED_MODULE>& patternWriter() {
      return mNumberWriter.patternWriter();
    }

    /** Get the underlying NumberWriter. */
    NumberWriter<T_LED_MODULE>& numberWriter() { return mNumberWriter; }

    /** Reset cursor to home. */
    void home() { mNumberWriter.home(); }

    /**
     * Write the hour and minute in 24-hour format (i.e. leading 0), and the
     * colon in one-shot, assuming the LED module is a 4-digit clock module.
     * This is a convenience function.
     */
    void writeHourMinute24(uint8_t hh, uint8_t mm) {
      mNumberWriter.writeDec2(hh);
      mNumberWriter.writeDec2(mm);
      writeColon();
    }

    /**
     * Write the hour and minute in 12-hour format (i.e. leading space), and the
     * colon in one-shot, assuming the LED module is a 4-digit clock module.
     * This is a convenience function.
     */
    void writeHourMinute12(uint8_t hh, uint8_t mm) {
      mNumberWriter.writeDec2(hh, kPatternSpace);
      mNumberWriter.writeDec2(mm);
      writeColon();
    }

    /**
     * Write the colon symbol between 'hh' and 'mm'.
     *
     * @param state Set to false to turn off the colon.
     */
    void writeColon(bool state = true) {
      mNumberWriter.setDecimalPointAt(mColonDigit, state);
    }

    /** Clear the entire display. */
    void clear() { mNumberWriter.clear(); }

    /** Clear the display from `pos` to the end. */
    void clearToEnd() { mNumberWriter.clearToEnd(); }

  private:
    // disable copy-constructor and assignment operator
    ClockWriter(const ClockWriter&) = delete;
    ClockWriter& operator=(const ClockWriter&) = delete;

    NumberWriter<T_LED_MODULE>& mNumberWriter;
    uint8_t const mColonDigit;
};

} // ace_segment

#endif
