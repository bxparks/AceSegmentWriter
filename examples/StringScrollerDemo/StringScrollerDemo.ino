/*
 * Demo of StringScroller class on a 4-digit LED module using the TM1637 chip.
 */

#include <Arduino.h>
#include <AceTMI.h> // SimpleTmi1637Interface
#include <AceSegment.h> // Tm1637Module
#include <AceSegmentWriter.h> // StringScroller

using ace_tmi::SimpleTmi1637Interface;
using ace_segment::LedModule;
using ace_segment::Tm1637Module;
using ace_segment::CharWriter;
using ace_segment::StringScroller;

// Replace these with the PIN numbers of your dev board.
const uint8_t CLK_PIN = A0;
const uint8_t DIO_PIN = 9;
const uint8_t NUM_DIGITS = 4;

// Many TM1637 LED modules contain 10 nF capacitors on their DIO and CLK lines
// which are unreasonably high. This forces a 100 microsecond delay between
// bit transitions. If you remove those capacitors, you can set this as low as
// 1-5 micros.
const uint8_t DELAY_MICROS = 100;

using TmiInterface = SimpleTmi1637Interface;
TmiInterface tmiInterface(DIO_PIN, CLK_PIN, DELAY_MICROS);
Tm1637Module<TmiInterface, NUM_DIGITS> ledModule(tmiInterface);

CharWriter<LedModule> charWriter(ledModule);
StringScroller<LedModule> stringScroller(charWriter);

static const char SCROLL_STRING[] PROGMEM =
"the quick brown fox jumps over the lazy dog, "
"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ";

// Scroll left. Then scroll right.
void scrollString() {
  static bool isInit = false;
  static bool scrollLeft = true;

  if (! isInit) {
    if (scrollLeft) {
      stringScroller.initScrollLeft(
          (const __FlashStringHelper*) SCROLL_STRING);
    } else {
      stringScroller.initScrollRight(
          (const __FlashStringHelper*) SCROLL_STRING);
    }
    isInit = true;
  }

  bool isDone;
  if (scrollLeft) {
    isDone = stringScroller.scrollLeft();
  } else {
    isDone = stringScroller.scrollRight();
  }

  if (isDone) {
    scrollLeft = !scrollLeft;
    isInit = false;
  }
}

const uint16_t SCROLL_DELAY = 300;

void scrollLoop() {
  static uint16_t lastUpdateMillis = millis();

  uint16_t nowMillis = millis();
  if ((uint16_t) (nowMillis - lastUpdateMillis) >= SCROLL_DELAY) {
    lastUpdateMillis = nowMillis;
    scrollString();
    ledModule.flush();
  }
}

void setup() {
  delay(1000);

  tmiInterface.begin();
  ledModule.begin();

  ledModule.setBrightness(2);

  ledModule.flush();
}

void loop() {
  scrollLoop();
}

