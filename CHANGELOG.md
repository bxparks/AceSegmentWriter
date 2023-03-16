# Changelog

* Unreleased
* 0.4 (2023-03-15)
    * **Breaking Change**
        * `PatternWriter`
            * `PatternWriter` is now stateful and holds the current `pos`.
            * `writePattern()` writes the pattern at the current position, and
              automatically increments.
            * Various `writeXxxAt()` replaced with simpler `writeXxx()` methods.
            * Added `PatternWriter::home()`, `PatternWriter::clear()`, and
              `PatternWriter::clearToEnd()`.
        * `NumberWriter`
            * Rename `hexchar_t` to `digit_t`.
            * Rename `kHexCharXxx` constants to `kDigitXxx`.
            * Rename `writeHexCharXxxAt()` to `writeDigitXxx()`
            * Rename `writeDecXxAt()` to `writeDecXx()`
            * Rename `writeUnsignedDecimalAt()` to `writeUnsignedDecimal()`
            * Rename `writeSignedDecimalAt()` to `writeSignedDecimal()`
        * `CharWriter`
            * Rename `writeCharAt()` to `writeChar()`.
        * `TemperatureWriter`
            * Rename `writeXxxAt()` to `writeXxx()`
* 0.3 (2022-02-02)
    * Upgrade to AceTMI v0.5.
        * Rename `SimpleTmiInterface` to `SimpleTmi1637Interface`.
        * Rename `SimpleTmiFastInterface` to `SimpleTmi1637FastInterface`.
    * Upgrade tool chain and regenerate `MemoryBenchmark` and `AutoBenchmark`.
        * Arduino IDE from 1.8.13 to 1.8.19
        * Arduino CLI from 0.14.0 to 0.19.2
        * Arduino AVR Core from 1.8.3 to 1.8.4
        * STM32duino from 2.0.0 to 2.2.0
        * ESP8266 Core from 2.7.4 to 3.0.2
        * ESP32 Core from 1.0.6 to 2.0.2
        * Teensyduino from 1.53 to 1.56
    * Downgrade SAMD21 boards to new "Tier 3: May work but unsupported" tier.
* 0.2 (2021-08-17)
    * Consolidate commonly used `kPatternXxx` constants into `PatternWriter.h`
      instead of being distributed across different classes.
    * Replace `writeHourMinute()` into `writeHourMinute12()` (12-hour format
      with prepending space), and `writeHourMinute24()` (24-hour format with
      prepending 0).
    * Move `writeBcd2At()`, `writeDec2At()` and `writeDec4At()` from
      `ClockWriter` to `NumberWriter`.
    * Add `padPattern` parameter to `writeDec2At()` and `writeDec4At()` with
      the default of `kPattern0` to get leading 0 characters.
    * Remove `writeUnsignedDecimal2At()` since it has the same functionality as
      `writeDec2At()`.
    * First public release.
* 0.1 (2021-08-10)
    * Initial release.
* (2021-08-09)
    * Initial extraction of [AceSegment](https://github.com/bxparks/AceSegment).
