# Memory Benchmark

The `MemoryBenchmark.ino` collects the amount of flash and static memory
consumed by different implementations in the AceSegment library.

It compiles various code snippets which are controlled by the `FEATURE` macro
flag. The `collect.sh` edits this `FEATURE` flag programmatically, then runs the
Arduino IDE compiler on the program, and extracts the flash and static memory
usage into a text file (e.g. `nano.txt`).

The numbers shown below should be considered to be rough estimates. It is often
difficult to separate out the code size of the library from the overhead imposed
by the runtime environment of the processor. For example, it often seems like
the ESP8266 allocates flash memory in blocks of a certain quantity, so the
calculated flash size can jump around in unexpected ways.

**Version**: AceSegmentWriter 0.1.0

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules for several microcontrollers:

```
$ make benchmarks
```
produces the following files:

```
attiny.txt
nano.txt
micro.txt
samd.txt
stm32.txt
esp8266.txt
esp32.txt
teensy32.txt
```

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

Fortunately, we no longer need to run `generate_table.awk` for each `*.txt`
file. The process has been automated using the `generate_readme.py` script which
will be invoked by the following command:
```
$ make README.md
```

## Library Size Changes

**v0.1**

* Initial MemoryBenchmark after extracting from AceSegment v0.8.2.

## Results

The following shows the flash and static memory sizes of the `MemoryBenchmark`
program for various Writer classes.

* `NumberWriter`
* `ClockWriter`
* `TemperatureWriter`
* `CharWriter`
* `StringWriter`
* `StringScroller`
* `LevelWriter`

### ATtiny85

* 8MHz ATtiny85
* Arduino IDE 1.8.13
* SpenceKonde/ATTinyCore 1.5.2

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |    260/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    328/   18 |    68/    7 |
| NumberWriter                    |    370/   18 |   110/    7 |
| ClockWriter                     |    430/   19 |   170/    8 |
| TemperatureWriter               |    436/   18 |   176/    7 |
| CharWriter                      |    492/   21 |   232/   10 |
| StringWriter                    |    650/   29 |   390/   18 |
| StringScroller                  |    708/   35 |   448/   24 |
| LevelWriter                     |    352/   18 |    92/    7 |
+--------------------------------------------------------------+

```

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |    458/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    528/   18 |    70/    7 |
| NumberWriter                    |    578/   18 |   120/    7 |
| ClockWriter                     |    644/   19 |   186/    8 |
| TemperatureWriter               |    642/   18 |   184/    7 |
| CharWriter                      |    690/   21 |   232/   10 |
| StringWriter                    |    840/   29 |   382/   18 |
| StringScroller                  |    924/   35 |   466/   24 |
| LevelWriter                     |    556/   18 |    98/    7 |
+--------------------------------------------------------------+

```

### Sparkfun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |   3474/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |   3506/  158 |    32/    7 |
| NumberWriter                    |   3556/  158 |    82/    7 |
| ClockWriter                     |   3622/  159 |   148/    8 |
| TemperatureWriter               |   3620/  158 |   146/    7 |
| CharWriter                      |   3668/  161 |   194/   10 |
| StringWriter                    |   3796/  169 |   322/   18 |
| StringScroller                  |   3880/  175 |   406/   24 |
| LevelWriter                     |   3534/  158 |    60/    7 |
+--------------------------------------------------------------+

```

### SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* Sparkfun SAMD Core 1.8.3

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |   9948/    0 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |   9980/    0 |    32/    0 |
| NumberWriter                    |  10056/    0 |   108/    0 |
| ClockWriter                     |  10092/    0 |   144/    0 |
| TemperatureWriter               |  10068/    0 |   120/    0 |
| CharWriter                      |  10132/    0 |   184/    0 |
| StringWriter                    |  10248/    0 |   300/    0 |
| StringScroller                  |  10248/    0 |   300/    0 |
| LevelWriter                     |   9996/    0 |    48/    0 |
+--------------------------------------------------------------+

```

### STM32 Blue Pill

* STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 2.0.0

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |  21428/ 3544 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  21460/ 3548 |    32/    4 |
| NumberWriter                    |  21544/ 3548 |   116/    4 |
| ClockWriter                     |  21568/ 3552 |   140/    8 |
| TemperatureWriter               |  21552/ 3548 |   124/    4 |
| CharWriter                      |  21616/ 3556 |   188/   12 |
| StringWriter                    |  21720/ 3560 |   292/   16 |
| StringScroller                  |  21716/ 3568 |   288/   24 |
| LevelWriter                     |  21476/ 3548 |    48/    4 |
+--------------------------------------------------------------+

```

### ESP8266

* NodeMCU 1.0, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        | 256716/26792 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   | 256736/26788 |    20/   -4 |
| NumberWriter                    | 256912/26788 |   196/   -4 |
| ClockWriter                     | 256976/26796 |   260/    4 |
| TemperatureWriter               | 256928/26788 |   212/   -4 |
| CharWriter                      | 256928/26796 |   212/    4 |
| StringWriter                    | 257128/26820 |   412/   28 |
| StringScroller                  | 257064/26828 |   348/   36 |
| LevelWriter                     | 256784/26788 |    68/   -4 |
+--------------------------------------------------------------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.6

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        | 198032/13124 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   | 198056/13132 |    24/    8 |
| NumberWriter                    | 198140/13132 |   108/    8 |
| ClockWriter                     | 198256/13132 |   224/    8 |
| TemperatureWriter               | 198152/13132 |   120/    8 |
| CharWriter                      | 198220/13140 |   188/   16 |
| StringWriter                    | 198356/13140 |   324/   16 |
| StringScroller                  | 198300/13148 |   268/   24 |
| LevelWriter                     | 198092/13132 |    60/    8 |
+--------------------------------------------------------------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |  10892/ 4160 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  10924/ 4164 |    32/    4 |
| NumberWriter                    |  10956/ 4164 |    64/    4 |
| ClockWriter                     |  10988/ 4168 |    96/    8 |
| TemperatureWriter               |  10960/ 4164 |    68/    4 |
| CharWriter                      |  11080/ 4172 |   188/   12 |
| StringWriter                    |  11212/ 4176 |   320/   16 |
| StringScroller                  |  11184/ 4184 |   292/   24 |
| LevelWriter                     |  10936/ 4164 |    44/    4 |
+--------------------------------------------------------------+

```

