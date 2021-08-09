/*
 * A program which determines the flash and static memory consumption of various
 * AceSegmentWriter classes.
 */

#include <Arduino.h>
#include <AceSegmentWriter.h>
using namespace ace_segment;

// DO NOT MODIFY THIS LINE. This will be overwritten by collect.sh on each
// iteration, incrementing from 0 to N. The Arduino IDE will compile the
// program, then the script will extract the flash and static memory usage
// numbers printed out by the Arduino compiler. The numbers will be printed on
// the STDOUT, which then can be saved to a file specific for a particular
// hardware platform, e.g. "nano.txt" or "esp8266.txt".
#define FEATURE 0

// List of features of AceSegment that we want to gather memory usage numbers.
#define FEATURE_BASELINE 0
#define FEATURE_PATTERN_WRITER 1
#define FEATURE_NUMBER_WRITER 2
#define FEATURE_CLOCK_WRITER 3
#define FEATURE_TEMPERATURE_WRITER 4
#define FEATURE_CHAR_WRITER 5
#define FEATURE_STRING_WRITER 6
#define FEATURE_STRING_SCROLLER 7
#define FEATURE_LEVEL_WRITER 8

// A volatile integer to prevent the compiler from optimizing away the entire
// program.
volatile int disableCompilerOptimization = 0;

// Common to all FEATURES
const uint8_t NUM_DIGITS = 4;

// This StubModule has the same public interface as AceSegment's LedModule
// class, without inheriting from LedModule. The Writer classes are templatized,
// so we can just use the StubModule. This means that this program does not
// depend on the AceSegment library at all, and we can isolate the benchmarks to
// just the Writer classes in this library.
class StubModule {
  public:
    StubModule() {}

    uint8_t getNumDigits() const { return NUM_DIGITS; }

    void setPatternAt(uint8_t pos, uint8_t pattern) {
      mPatterns[pos] = pattern;
    }

    uint8_t getPatternAt(uint8_t pos) const {
      return mPatterns[pos];
    }

    void setBrightness(uint8_t brightness) {
      mBrightness = brightness;
    }

    uint8_t getBrightness() const {
      return mBrightness;
    }

  private:
    uint8_t mPatterns[NUM_DIGITS];
    uint8_t mBrightness;
};

#if FEATURE == FEATURE_STUB_MODULE
  StubModule stubModule;

#elif FEATURE == FEATURE_PATTERN_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);

#elif FEATURE == FEATURE_NUMBER_WRITER
  StubModule stubModule;
  NumberWriter<StubModule> numberWriter(stubModule);

#elif FEATURE == FEATURE_CLOCK_WRITER
  StubModule stubModule;
  ClockWriter<StubModule> clockWriter(stubModule);

#elif FEATURE == FEATURE_TEMPERATURE_WRITER
  StubModule stubModule;
  TemperatureWriter<StubModule> temperatureWriter(stubModule);

#elif FEATURE == FEATURE_CHAR_WRITER
  StubModule stubModule;
  CharWriter<StubModule> charWriter(stubModule);

#elif FEATURE == FEATURE_STRING_WRITER
  StubModule stubModule;
  CharWriter<StubModule> charWriter(stubModule);
  StringWriter<StubModule> stringWriter(charWriter);

#elif FEATURE == FEATURE_STRING_SCROLLER
  StubModule stubModule;
  CharWriter<StubModule> charWriter(stubModule);
  StringScroller<StubModule> stringScroller(charWriter);

#elif FEATURE == FEATURE_LEVEL_WRITER
  StubModule stubModule;
  LevelWriter<StubModule> levelWriter(stubModule);

#else
  #error Unknown FEATURE

#endif

// TeensyDuino seems to pull in malloc() and free() when a class with virtual
// functions is used polymorphically. This causes the memory consumption of
// FEATURE_BASELINE (which normally has no classes defined, so does not include
// malloc() and free()) to be artificially small which throws off the memory
// consumption calculations for all subsequent features. Let's define a
// throw-away class and call its method for all FEATURES, including BASELINE.
#if defined(TEENSYDUINO)
  class FooClass {
    public:
      virtual void doit() {
        disableCompilerOptimization = 0;
      }
  };

  FooClass* foo;
#endif

void setup() {
#if defined(TEENSYDUINO)
  foo = new FooClass();
#endif
}

void loop() {
#if defined(TEENSYDUINO)
  foo->doit();
#endif

  stubModule.setPatternAt(0, 0xff);

#if FEATURE == FEATURE_BASELINE
  // do nothing

#elif FEATURE == FEATURE_PATTERN_WRITER
  patternWriter.writePatternAt(0, 0x3C);

#elif FEATURE == FEATURE_NUMBER_WRITER
  numberWriter.writeUnsignedDecimalAt(0, 42);

#elif FEATURE == FEATURE_CLOCK_WRITER
  clockWriter.writeHourMinute(10, 45);

#elif FEATURE == FEATURE_TEMPERATURE_WRITER
  temperatureWriter.writeTempDegCAt(0, 22 /*temp*/, 4 /*boxSize*/);

#elif FEATURE == FEATURE_CHAR_WRITER
  charWriter.writeCharAt(0, 'a');

#elif FEATURE == FEATURE_STRING_WRITER
  stringWriter.writeStringAt(0, "Hello");

#elif FEATURE == FEATURE_STRING_SCROLLER
  stringScroller.initScrollLeft("Hello");
  stringScroller.scrollLeft();

#elif FEATURE == FEATURE_LEVEL_WRITER
  levelWriter.writeLevel(3);

#else
  #error Unknown FEATURE
#endif

  disableCompilerOptimization = stubModule.getPatternAt(0);
}
