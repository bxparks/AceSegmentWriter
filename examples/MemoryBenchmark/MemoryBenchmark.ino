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
#define FEATURE_NUMBER_WRITER_WRITE_FLOAT 3
#define FEATURE_CLOCK_WRITER 4
#define FEATURE_TEMPERATURE_WRITER 5
#define FEATURE_CHAR_WRITER 6
#define FEATURE_STRING_WRITER 7
#define FEATURE_STRING_SCROLLER 8
#define FEATURE_LEVEL_WRITER 9

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

    uint8_t size() const { return NUM_DIGITS; }

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

    void setDecimalPointAt(uint8_t pos, bool state = true) {
      if (pos >= size()) return;
      if (state) {
        mPatterns[pos] |= 0x80;
      } else {
        mPatterns[pos] &= ~0x80;
      }
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
  PatternWriter<StubModule> patternWriter(stubModule);
  NumberWriter<StubModule> numberWriter(patternWriter);

#elif FEATURE == FEATURE_NUMBER_WRITER_WRITE_FLOAT
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  NumberWriter<StubModule> numberWriter(patternWriter);

#elif FEATURE == FEATURE_CLOCK_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  NumberWriter<StubModule> numberWriter(patternWriter);
  ClockWriter<StubModule> clockWriter(numberWriter);

#elif FEATURE == FEATURE_TEMPERATURE_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  NumberWriter<StubModule> numberWriter(patternWriter);
  TemperatureWriter<StubModule> temperatureWriter(numberWriter);

#elif FEATURE == FEATURE_CHAR_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  CharWriter<StubModule> charWriter(patternWriter);

#elif FEATURE == FEATURE_STRING_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  CharWriter<StubModule> charWriter(patternWriter);
  StringWriter<StubModule> stringWriter(charWriter);

#elif FEATURE == FEATURE_STRING_SCROLLER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  CharWriter<StubModule> charWriter(patternWriter);
  StringScroller<StubModule> stringScroller(charWriter);

#elif FEATURE == FEATURE_LEVEL_WRITER
  StubModule stubModule;
  PatternWriter<StubModule> patternWriter(stubModule);
  LevelWriter<StubModule> levelWriter(patternWriter);

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

  stubModule.setPatternAt(0, disableCompilerOptimization);
  disableCompilerOptimization = stubModule.getPatternAt(0);

#if FEATURE == FEATURE_BASELINE
  // do nothing

#elif FEATURE == FEATURE_PATTERN_WRITER
  patternWriter.writePattern(disableCompilerOptimization);

#elif FEATURE == FEATURE_NUMBER_WRITER
  numberWriter.writeUnsignedDecimal(disableCompilerOptimization);

#elif FEATURE == FEATURE_NUMBER_WRITER_WRITE_FLOAT
  numberWriter.writeFloat((float) disableCompilerOptimization);

#elif FEATURE == FEATURE_CLOCK_WRITER
  clockWriter.writeHourMinute24(10, disableCompilerOptimization);

#elif FEATURE == FEATURE_TEMPERATURE_WRITER
  temperatureWriter.writeTempDegC(
      disableCompilerOptimization /*temp*/, 4 /*boxSize*/);

#elif FEATURE == FEATURE_CHAR_WRITER
  charWriter.writeChar(disableCompilerOptimization);

#elif FEATURE == FEATURE_STRING_WRITER
  stringWriter.writeString(F("Hello"));

#elif FEATURE == FEATURE_STRING_SCROLLER
  stringScroller.initScrollLeft(F("Hello"));
  stringScroller.scrollLeft();

#elif FEATURE == FEATURE_LEVEL_WRITER
  levelWriter.writeLevel(disableCompilerOptimization);

#else
  #error Unknown FEATURE
#endif

  disableCompilerOptimization = stubModule.getPatternAt(0);
}
