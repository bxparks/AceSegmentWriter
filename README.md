# AceSegmentWriter

[![AUnit Tests](https://github.com/bxparks/AceSegmentWriter/actions/workflows/aunit_tests.yml/badge.svg)](https://github.com/bxparks/AceSegmentWriter/actions/workflows/aunit_tests.yml)

This is a companion library to the
[AceSegment](https://github.com/bxparks/AceSegment) library. It provides 
higher-level convenience classes to write decimal numbers, hex numbers,
temperature, clock digits, characters, and strings to seven segment LED modules.
Initially, this code was part of the AceSegment library and depended directly on
the `LedModule` class of that library. Later the direct dependency to the
`LedModule` class was removing by converting the Writer classes into C++
templates. The classes now depend on the generic `T_LED_MODULE` template type
which is assumed to implement the public methods defined by the `LedModule` in
the `AceSegment` library. There is no longer a direct compile-time dependency to
the AceSegment library.

**Version**: 0.1.0 (2021-08-09)

**Changelog**: [CHANGELOG.md](CHANGELOG.md)

## Table of Contents

* [Installation](#Installation)
    * [Source Code](#SourceCode)
    * [Dependencies](#Dependencies)
* [Documentation](#Documentation)
    * [Examples](#Examples)
* [High Level Overview](#HighLevelOverview)
    * [Classes and Types](#ClassesAndTypes)
    * [Dependency Diagram](#DependencyDiagram)
    * [Digit and Segment Addressing](#DigitAndSegmentAddressing)
* [Usage](#Usage)
    * [Include Header and Namespace](#HeaderAndNamespace)
    * [T_LED_MODULE](#T_LED_MODULE)
    * [PatternWriter](#PatternWriter)
    * [NumberWriter](#NumberWriter)
    * [ClockWriter](#ClockWriter)
    * [TemperatureWriter](#TemperatureWriter)
    * [CharWriter](#CharWriter)
    * [StringWriter](#StringWriter)
    * [LevelWriter](#LevelWriter)
    * [StringScroller](#StringScroller)
* [Resource Consumption](#ResourceConsumption)
    * [Flash And Static Memory](#FlashAndStaticMemory)
* [System Requirements](#SystemRequirements)
    * [Hardware](#Hardware)
    * [Tool Chain](#ToolChain)
    * [Operating System](#OperatingSystem)
* [Bugs And Limitations](#BugsAndLimitations)
* [Alternative Libraries](#AlternativeLibraries)
* [License](#License)
* [Feedback and Support](#FeedbackAndSupport)
* [Authors](#Authors)

<a name="Installation"></a>
## Installation

The latest stable release will eventually be available in the Arduino IDE
Library Manager. Search for "AceSegmentWriter". Click install. (It is not there
yet.)

The development version can be installed by cloning the
[GitHub repository](https://github.com/bxparks/AceSegmentWriter), checking out
the `develop` branch, then manually copying over the contents to the
`./libraries` directory used by the Arduino IDE. (The result is a directory
named `./libraries/AceSegmentWriter`.)

The `master` branch contains the stable release.

<a name="SourceCode"></a>
### Source Code

The source files are organized as follows:
* `src/AceSegmentWriter.h` - main header file
* `src/ace_segment_writer/` - implementation files
* `tests/` - unit tests which require [AUnit](https://github.com/bxparks/AUnit)
* `examples/` - example sketches
* `docs/` - contains the doxygen docs and additional manual docs

<a name="Dependencies"></a>
### Dependencies

This library library has a direct, compile-time dependency on:

* AceCommon (https://github.com/bxparks/AceCommon)

The following libraries are recommended, and they are added in the `depends`
property of `library.properties` so that they are automatically installed by the
Arduino Library Manager for convenience. However, client applications are not
required to use these. Other libraries with the same interface and functionality
can be substituted if desired.

* AceSegment (https://github.com/bxparks/AceSegment)
* AceSPI (https://github.com/bxparks/AceSPI)
* AceTMI (https://github.com/bxparks/AceTMI)
* AceWire (https://github.com/bxparks/AceWire)

The unit tests depend on:

* AUnit (https://github.com/bxparks/AUnit)

Some of the examples may depend on the following which may need to be installed
manually:

* AceButton (https://github.com/bxparks/AceButton)

<a name="Documentation"></a>
## Documentation

* this `README.md` file
* [Doxygen docs](https://bxparks.github.io/AceSegmentWriter/html)
    * On Github pages.

<a name="Examples"></a>
### Examples

The following example sketches are provided:

* Basic
* Intermediate
    * [WriterTester.ino](examples/WriterTester)
        * demo of the various `src/writer` classes
        * depends on AceButton (https://github.com/bxparks/AceButton) library
        * uses 2 buttons for "single step" debugging mode
* Benchmarks
    * [MemoryBenchmark.ino](examples/MemoryBenchmark): determines the size of
      the various components of the library
    * Normally, I write an `AutoBenchmark.ino program to determine the CPU
      speed of my library code. But these Writer classes simply right into the
      in-memory buffer provided by the underying `T_LED_MODULE` class, so the
      execution time is too fast to be easily measured and does not seem worth
      the effort.

<a name="HighLevelOverview"></a>
## High Level Overview

<a name="ClassesAndTypes"></a>
### Classes and Types

Here are the classes and types in the library:

* `T_LED_MODULE`
    * Generic type for classes in this library. It is assumed to implement the
      public methods of the `LedModule` class of the AceSegment library.
* `PatternWriter`
    * Class that knows how to write segment bit patterns to a `T_LED_MODULE`.
    * Other Writer classes are built upon this by composition.
* `NumberWriter`
    * A class that writes integers in decimal or hexadecimal format to the
      `T_LED_MODULE`.
    * A few additional characters are supported: `kCharSpace`, `kCharMinus`
* `ClockWriter`
    * A class that writes a clock string "hh:mm" to `T_LED_MODULE`.
    * A few additional symbols are supported: `kCharSpace`, `kCharMinus` and
      `kPatternA` ("A" for AM) and `kPatternP` ("P" for PM).
* `TemperatureWriter`
    * A class that writes temperatures with a degrees symbol or optionally
      with "C" or "F" symbol.
* `CharWriter`
    * A class that convert an ASCII character represented by a `char` (code
      0-127) to a bit pattern used by `SegmentDriver` class.
    * Not all ASCII characters can be rendered on a seven segment display
      legibly but the `CharWriter` tries its best.
* `StringWriter`
    * A class that prints strings of `char` to a `CharWriter`, which in
      turns, prints to the `T_LED_MODULE`.
* `StringScoller`
    * Scroll a string left and right.
* `LevelWriter`
    * Display specified number of bars from left to right, 2 bars per digit.

<a name="DependencyDiagram"></a>
### Dependency Diagram

The conceptual dependency diagram among these classes looks something like this:

```
             StringScroller
             StringWriter
                   |
                   V
PatternWriter  CharWriter NumberWriter ClockWriter TemperatureWriter LevelWriter
         \          \           |     /                /                /
          -------    -------    |    / ----------------       ----------
                 \          \   |   / /                      /
                  ---------\ \  |  / / /---------------------
                            v v v v v v
                           T_LED_MODULE
                                |
                                | (optionally)
                                v
                           <AceSegment.h>
                                |
                                v
                            <AceWire.h>
                            <AceTMI.h>
                            <AceSPI.h>

```

(The actual dependency among various classes is a bit more complicated than this
diagram.)

<a name="DigitAndSegmentAddressing"></a>
### Digit and Segment Addressing

The `T_LED_MODULE` type must support the following conventions for addressing
the digits and segments:

* digits start at position 0 on the left and increase to the right
* segments are assigned bits 0 to 7 of an unsigned byte (type `uint8_t`) with
  segment `a` assigned to bit 0, segment `g` assigned to bit 6, and the optional
  decimal point assigned to bit 7

```
+------------+  +------------+           +------------+
|   aaaa     |  |   aaaa     |           |   aaaa     |
|  f    b    |  |  f    b    |           |  f    b    |
|  f    b    |  |  f    b    |           |  f    b    |
|   gggg     |  |   gggg     |  *  *  *  |   gggg     |
|  e    c    |  |  e    c    |           |  e    c    |
|  e    c    |  |  e    c    |           |  e    c    |
|   dddd  dp |  |   dddd  dp |           |   dddd  dp |
+------------+  +------------+           +------------+
  Digit 0         Digit 1                 Digit {N-1}


Segment: dp g f e d c b a
   Bit:  7  6 5 4 3 2 1 0
```

Some 4-digit LED modules are designed to be used in a clock to display the hour
and minute components of the time as `HH:MM`. In these modules, it is common for
the decimal point on Digit 1 to be replaced with the `colon` segment between
Digit 1 and Digit 2. In these modules, sometimes the decimal points for the
other digits work normally, but sometimes, the remaining decimal points do not
work at all.

<a name="Usage"></a>
## Usage

<a name="HeaderAndNamespace"></a>
### Include Header and Namespace

Only a single header file `AceSegmentWriter.h` is required to use this library.
To prevent name clashes with other libraries that the calling code may use, all
classes are defined in the `ace_segment` namespace. (This is the same namespace
used by the AceSegment library to make it easier to use them together.) To use
the code without prepending the `ace_segment::` prefix, use the `using`
directive:

```C++
#include <Arduino.h>
#include <AceSegmentWriter.h>
using namespace ace_segment;
```

<a name="T_LED_MODULE"></a>
### `T_LED_MODULE`

All Writer classes directly (or indirectly) wrap around an underlying LED module
class with the generic `T_LED_MODULE` type. It is assumed to implement the same
public methods of the
[LedModule](https://github.com/bxparks/AceSegment/blob/develop/src/ace_segment/LedModule.h)
class from AceSegment. However, the Writer classes in this library are
implemented as C++ templates, so the `T_LED_MODULE` type does **not** need to
inherit from the `LedModule` class. The public methods of `T_LED_MODULE`
should look like this:

```C++
class LedModule {
  public:
    uint8_t getNumDigits() const;
    void setPatternAt(uint8_t pos, uint8_t pattern);
    uint8_t getPatternAt(uint8_t pos) const;
    void setBrightness(uint8_t brightness);
    void getBrightness() const;
};
```

<a name="PatternWriter"></a>
### PatternWriter

The `PatternWriter` class is the most basic wrapper around an `T_LED_MODULE`
object, and provides more convenient interfaces to writing to the LED module. It
provides the following features on top of `T_LED_MODULE`:

* Validation is performed on the digit `pos` parameter. If the `pos` is not
  valid, then the method returns immediately without performing any action.
* Entire strings (both normal strings and `PROGMEM` strings) can be written
  to the led module.
* The `writeDecimalPointAt()` convenience function adds a decimal point at the
  specified `pos` location.
* The `clear()` and `clearToEnd()` functions provide ways to clear the LED
  display.

The public methods of the class looks like this (not all public methods shown):

```C++
template <typename T_LED_MODULE>
class PatternWriter {
  public:
    explicit PatternWriter(T_LED_MODULE& ledModule);

    uint8_t getNumDigits() const;

    void writePatternAt(uint8_t pos, uint8_t pattern);
    void writePatternsAt(uint8_t pos, const uint8_t patterns[], uint8_t len);
    void writePatternsAt_P(uint8_t pos, const uint8_t patterns[], uint8_t len);
    void writeDecimalPointAt(uint8_t pos, bool state = true);

    void clear();
    void clearToEnd(uint8_t pos);
};
```

The decimal point is stored as bit 7 (the most significant bit) of the `uint8_t`
byte for a given digit. This bit is cleared by the other `writePatternAt()` or
`writePatternsAt()` functions. So the `writeDecimalPointAt()` should be called
**after** the other write methods are called.

```C++
PatternWriter patternWriter(ledModule);
```

<a name="NumberWriter"></a>
### NumberWriter

The `NumberWriter` can print integers to the `T_LED_MODULE` using decimal (0-9) or
hexadecimal (0-9A-F) formats. On platforms that support it (AVR and ESP8266),
the bit mapping table is stored in flash memory to conserve static memory.

The public methods of this class looks something like this:

```C++
namespace ace_segment {

const uint8_t kNumHexCharPatterns = 18;
extern const uint8_t kHexCharPatterns[kNumHexCharPatterns];

typedef uint8_t hexchar_t;
const hexchar_t kHexCharSpace = 0x10;
const hexchar_t kHexCharMinus = 0x11;

template <typename T_LED_MODULE>
class NumberWriter {
  public:
    typedef uint8_t hexchar_t;
    static const hexchar_t kCharSpace = 0x10;
    static const hexchar_t kCharMinus = 0x11;

    explicit NumberWriter(T_LED_MODULE& ledModule);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();

    void writeHexCharAt(uint8_t pos, hexchar_t c);
    void writeHexCharsAt(uint8_t pos, hexchar_t [], uint8_t len);

    void writeHexByteAt(uint8_t pos, uint8_t b);
    void writeHexWordAt(uint8_t pos, uint16_t w);

    void writeUnsignedDecimalAt(uint8_t pos, uint16_t num, int8_t boxSize = 0);
    void writeSignedDecimalAt(uint8_t pos, int16_t num, int8_t boxSize = 0);
    void writeUnsignedDecimal2At(uint8_t pos, uint8_t num);

    void clear();
    void clearToEnd(uint8_t pos);
};

}
```

The `hexchar_t` type semantically represents the character set supported by this
class. It is implemented as an alias for `uint8_t`, which unfortunately means
that the C++ compiler will not warn about mixing this type with another
`uint8_t`. The range of this character set is from `[0,15]` plus 2 additional
symbols, so `[0,17]`:

* `NumberWriter::kCharSpace`
* `NumberWriter::kCharMinus`

![NumberWriter](docs/writers/number_writer_hex.jpg)

![NumberWriter](docs/writers/number_writer_decimal.jpg)

<a name="ClockWriter"></a>
### ClockWriter

There are special, 4 digit,  seven segment LED displays which replace the
decimal point with the colon symbol ":" between the 2 digits on either side so
that it can display a time in the format "hh:mm".

The public methods of this class look like this:

```C++
namespace ace_segment {

const uint8_t kPatternA = 0b01110111;
const uint8_t kPatternP = 0b01110011;

template <typename T_LED_MODULE>
class ClockWriter {
  public:
    explicit ClockWriter(T_LED_MODULE& ledModule, uint8_t colonDigit = 1);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();
    NumberWriter<T_LED_MODULE>& numberWriter();

    void writeCharAt(uint8_t pos, hexchar_t c);
    void writeChar2At(uint8_t pos, hexchar_t c0, hexchar_t c1);

    void writeBcd2At(uint8_t pos, uint8_t bcd);
    void writeDec2At(uint8_t pos, uint8_t d);
    void writeDec4At(uint8_t pos, uint16_t dd);

    void writeHourMinute(uint8_t hh, uint8_t mm);
    void writeColon(bool state = true);
};

}
```

You can write the letters `A` and `P` using the underlying `patternWriter()`:

```C++
uint8_t pos = ...;
ClockWriter clockWriter(...);
clockWriter.patternWriter().writePatternAt(pos, ClockWriter::kPatternA);
```

![ClockWriter](docs/writers/clock_writer.jpg)

<a name="TemperatureWriter"></a>
### TemperatureWriter

This class supports writing out temperatures in degrees Celsius or Fahrenheit.
The public methods of this class looks something like this:

```C++
namespace ace_segment {

const uint8_t kPatternDegree = 0b01100011;
const uint8_t kPatternC = 0b00111001;
const uint8_t kPatternF = 0b01110001;

template <typename T_LED_MODULE>
class TemperatureWriter {
  public:
    explicit TemperatureWriter(T_LED_MODULE& ledModule);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();

    uint8_t writeTempAt(uint8_t pos, int16_t temp, boxSize = 0);
    uint8_t writeTempDegAt(uint8_t pos, int16_t temp, boxSize = 0);
    uint8_t writeTempDegCAt(uint8_t pos, int16_t temp, boxSize = 0);
    uint8_t writeTempDegFAt(uint8_t pos, int16_t temp, boxSize = 0);
};

}
```

![TemperatureWriter-Celsius](docs/writers/temperature_writer_celsius.jpg)

![TemperatureWriter-Fahrenheit](docs/writers/temperature_writer_fahrenheit.jpg)

<a name="CharWriter"></a>
### CharWriter

It is possible to represent many of the ASCII characters in the range `[0,127]`
on a seven-segment LED display, although some of the characters will necessarily
be crude given the limited number of segments. The `CharWriter` contains a
[mapping of ASCII](https://github.com/dmadison/LED-Segment-ASCII) characters
to seven-segment bit patterns. On platforms that support it (AVR and
ESP8266), the bit pattern array is stored in flash memory to conserve static
memory.

The public methods of this class looks like this:

```C++
namespace ace_segment {

const uint8_t kPatternUnknown = 0b00000000;

const uint8_t kNumCharPatterns = 128;
extern const uint8_t kCharPatterns[kNumCharPatterns];

template <typename T_LED_MODULE>
class CharWriter {
  public:
    explicit CharWriter(
        T_LED_MODULE& ledModule,
        const uint8_t charPatterns[] = kCharPatterns,
        uint8_t numChars = kNumChars
    );

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();

    void writeCharAt(uint8_t pos, char c);

    uint8_t getNumChars() const;
    uint8_t getPattern(char c) const;
};

}
```

![CharWriter](docs/writers/char_writer.jpg)

<a name="StringWriter"></a>
### StringWriter

A `StringWriter` is a class that builds on top of the `CharWriter`. It knows how
to write entire strings into the LED display. The public methods look like:

```C++
template <typename T_LED_MODULE>
class StringWriter {
  public:
    explicit StringWriter(CharWriter<T_LED_MODULE>& charWriter);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();
    CharWriter<T_LED_MODULE>& charWriter();

    uint8_t writeStringAt(uint8_t pos, const char* cs, uint8_t numChar = 255);

    uint8_t writeStringAt(uint8_t pos, const __FlashStringHelper* fs,
        uint8_t numChar = 255);

    void clear();
    void clearToEnd(uint8_t pos);
};
```

The implementation of `writeStringAt()` is straightforward except for the
handling of a decimal point. A seven segment LED digit contains a small LED for
the decimal point. Instead of taking up an entire digit for a single '.'
character, we can collapse the '.' character into the decimal point indicator of
the previous character on the left.

The optional `numChar` parameter limits the number of characters in the string
to write. The default value is 255 which is expected to be larger than the
largest LED module that will be used with the AceSegment and AceSegmentWriter
libraries, so the default value will print the entire string.

The actual number of LED digits written is returned by `writeStringAt()`. For
example, writing `"1.2"` returns 2 because the decimal point was merged into the
previous digit and only 2 digits are written.

The `clearToEnd()` method clears the LED display from the given `pos` to the end
of the display.

The following sequence of calls will write the given string and clear all digits
after the end of the string:

```C++
CharWriter charWriter(ledModule);
StringWriter stringWriter(charWriter);

uint8_t written = stringWriter.writeStringAt(0, s);
stringWriter.clearToEnd(written);
```

![StringWriter](docs/writers/string_writer.jpg)

<a name="LevelWriter"></a>
### LevelWriter

A `LevelWriter` writes a specified number of vertical bars (2 vertical
bar per digit) to the LED display, emulating a level meter LED module.

```C++
namespace ace_segment {

const uint8_t kPatternLevelLeft = 0b00110000;
const uint8_t kPatternLevelRight = 0b00000110;

template <typename T_LED_MODULE>
class LevelWriter {
  public:
    explicit LevelWriter(T_LED_MODULE& ledModule);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();

    uint8_t getMaxLevel() const;
    void writeLevel(uint8_t level);
};

}
```

There are 2 vertical bars available per per digit. So the maximum level
supported by a 4-digit LED module is 8, and an 8-digit LED module supports a
maximum level of 16.

![LevelWriter](docs/writers/level_writer.jpg)

<a name="StringScroller"></a>
### StringScroller

A `StringScroller` is a class that builds on top of the `CharWriter`. It can
scroll strings to the left and right. The public methods look like:

```C++
template <typename T_LED_MODULE>
class StringScroller {
  public:
    explicit StringScroller(CharWriter<T_LED_MODULE>& charWriter);

    T_LED_MODULE& ledModule();
    PatternWriter<T_LED_MODULE>& patternWriter();
    CharWriter<T_LED_MODULE>& charWriter();

    void initScrollLeft(const char* s);
    void initScrollLeft(const __FlashStringHelper* s);
    bool scrollLeft();

    void initScrollRight(const char* s);
    void initScrollRight(const __FlashStringHelper* s);
    bool scrollRight();
};
```

To scroll a string to the left, initialize the string using `initScrollLeft()`,
then call `scrollLeft()` to shift one position to the left. The string scrolls
into the first digit on the right. When the scrolling is finished, the display
becomes blank, and the `scrollLeft()` method returns `true` to indicate
`isDone`.

Similarly to scroll to the right, initialize the string using
`initScrollRight()`, then call `scrollRight()` to shift to the right. The string
scrolls into the first digit on the left. When the scrolling is finished, the
display becomes blank, and the `scrollRight()` method returns `true` to indicate
`isDone`.

<a name="ResourceConsumption"></a>
## Resource Consumption

<a name="FlashAndStaticMemory"></a>
### Flash And Static Memory

For the most part, the user pays only for the feature that is being used. For
example, if the `CharWriter` (which consumes about 300 bytes of flash on AVR) is
not used, it is not loaded into the program.

The full details are given in
[examples/MemoryBenchmark](examples/MemoryBenchmark). Here are 2 samples of
the flash and static memory consumptions.

**Arduino Nano (ATmega328)**

```
```

**ESP8266**

```
```

<a name="SystemRequirements"></a>
## System Requirements

<a name="Hardware"></a>
### Hardware

This library has Tier 1 support on the following boards:

* Arduino Nano (16 MHz ATmega328P)
* SparkFun Pro Micro (16 MHz ATmega32U4)
* SAMD21 M0 Mini (48 MHz ARM Cortex-M0+)
* STM32 Blue Pill (STM32F103C8, 72 MHz ARM Cortex-M3)
* NodeMCU 1.0 (ESP-12E module, 80MHz ESP8266)
* WeMos D1 Mini (ESP-12E module, 80 MHz ESP8266)
* ESP32 dev board (ESP-WROOM-32 module, 240 MHz dual core Tensilica LX6)
* Teensy 3.2 (72 MHz ARM Cortex-M4)

Tier 2 support can be expected on the following boards, mostly because I don't
test these as often:

* ATtiny85 (8 MHz ATtiny85)
* Arduino Pro Mini (16 MHz ATmega328P)
* Teensy LC (48 MHz ARM Cortex-M0+)
* Mini Mega 2560 (Arduino Mega 2560 compatible, 16 MHz ATmega2560)

The following boards are **not** supported:

* Any platform using the ArduinoCore-API
  (https://github.com/arduino/ArduinoCore-api).
    * For example, Nano Every, MKRZero, and Raspberry Pi Pico RP2040.

<a name="ToolChain"></a>
### Tool Chain

* [Arduino IDE 1.8.13](https://www.arduino.cc/en/Main/Software)
* [Arduino CLI 0.14.0](https://arduino.github.io/arduino-cli)
* [SpenceKonde ATTinyCore 1.5.2](https://github.com/SpenceKonde/ATTinyCore)
* [Arduino AVR Boards 1.8.3](https://github.com/arduino/ArduinoCore-avr)
* [Arduino SAMD Boards 1.8.9](https://github.com/arduino/ArduinoCore-samd)
* [SparkFun AVR Boards 1.1.13](https://github.com/sparkfun/Arduino_Boards)
* [SparkFun SAMD Boards 1.8.3](https://github.com/sparkfun/Arduino_Boards)
* [STM32duino 2.0.0](https://github.com/stm32duino/Arduino_Core_STM32)
* [ESP8266 Arduino 2.7.4](https://github.com/esp8266/Arduino)
* [ESP32 Arduino 1.0.6](https://github.com/espressif/arduino-esp32)
* [Teensyduino 1.53](https://www.pjrc.com/teensy/td_download.html)

<a name="OperatingSystem"></a>
### Operating System

I use Ubuntu 20.04 for the vast majority of my development. I expect that the
library will work fine under MacOS and Windows, but I have not explicitly tested
them.

<a name="BugsAndLimitations"></a>
## Bugs and Limitations

* The `NumberWriter` class does not support floating point numbers.
    * The primary reason is that I almost never use floating point numbers on
      embedded microcontrollers. Most embedded processors do not have hardware
      FPU, so floating point operations are implemented using software, which
      consumes significant amounts of memory and CPU cycles.
    * The second reason is that floating point formatting is very complex. There
      are numerous options to consider. For example: left justified, right
      justified, left pad with space, left padding with zeros, right padding
      with space, right padding with zeros, specifying the number of digits
      after the decimal point, and formating using scientific notation.
    * With so many different formatting options to consider, the easiest
      solution might be to defer this problem to the `vsnprintf()` function, to
      convert a float to a string, then render that string on the LED module.
      Except that on 8-bit AVR processors, the `vnsprintf()` function does not
      support floating point numbers.
    * The other potential solution is to use the `Print::print()` function to
      print a float to a string buffer, such as the `PrintStr<N>` class in
      [AceCommon](https://github.com/bxparks/AceCommon), then print the string
      to the LED module. This might be the most practical solution on an Arduino
      platform.
    * In any case, I think the code for printing floating point numbers should
      not go into the `NumberWriter` class, but into a new class called
      something like `FloatWriter`. The `FloatWriter` class could pull in a
      `NumberWriter` object and build on top of it.

<a name="AlternativeLibraries"></a>
## Alternative Libraries

Tracked using [AceSegment Wiki](https://github.com/bxparks/AceSegment/wiki).

<a name="License"></a>
## License

[MIT License](https://opensource.org/licenses/MIT)

<a name="FeedbackAndSupport"></a>
## Feedback and Support

If you have any questions, comments and other support questions about how to
use this library, use the
[GitHub Discussions](https://github.com/bxparks/AceSegmentWriter/discussions)
for this project. If you have bug reports or feature requests, file a ticket in
[GitHub Issues](https://github.com/bxparks/AceSegmentWriter/issues). I'd love to
hear about how this software and its documentation can be improved. I can't
promise that I will incorporate everything, but I will give your ideas serious
consideration.

Please refrain from emailing me directly unless the content is sensitive. The
problem with email is that I cannot reference the email conversation when other
people ask similar questions later.

<a name="Authors"></a>
## Authors

Created by Brian T. Park (brian@xparks.net).
