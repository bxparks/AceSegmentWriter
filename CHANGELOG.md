# Changelog

* Unreleased
* v0.2 (2021-08-17)
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
* v0.1 (2021-08-10)
    * Initial release.
* (2021-08-09)
    * Initial extraction of [AceSegment](https://github.com/bxparks/AceSegment).
