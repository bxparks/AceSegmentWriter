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

**Version**: AceSegmentWriter 0.2

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
| baseline                        |    274/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    350/   18 |    76/    7 |
| NumberWriter                    |    534/   18 |   260/    7 |
| ClockWriter                     |    550/   19 |   276/    8 |
| TemperatureWriter               |    718/   18 |   444/    7 |
| CharWriter                      |    518/   21 |   244/   10 |
| StringWriter                    |    640/   23 |   366/   12 |
| StringScroller                  |    700/   29 |   426/   18 |
| LevelWriter                     |    434/   18 |   160/    7 |
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
| baseline                        |    470/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    548/   18 |    78/    7 |
| NumberWriter                    |    720/   18 |   250/    7 |
| ClockWriter                     |    748/   19 |   278/    8 |
| TemperatureWriter               |    906/   18 |   436/    7 |
| CharWriter                      |    716/   21 |   246/   10 |
| StringWriter                    |    838/   23 |   368/   12 |
| StringScroller                  |    916/   29 |   446/   18 |
| LevelWriter                     |    634/   18 |   164/    7 |
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
| baseline                        |   3486/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |   3526/  158 |    40/    7 |
| NumberWriter                    |   3698/  158 |   212/    7 |
| ClockWriter                     |   3726/  159 |   240/    8 |
| TemperatureWriter               |   3884/  158 |   398/    7 |
| CharWriter                      |   3694/  161 |   208/   10 |
| StringWriter                    |   3816/  163 |   330/   12 |
| StringScroller                  |   3894/  169 |   408/   18 |
| LevelWriter                     |   3612/  158 |   126/    7 |
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
| baseline                        |  21432/ 3544 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  21464/ 3548 |    32/    4 |
| NumberWriter                    |  21584/ 3548 |   152/    4 |
| ClockWriter                     |  21576/ 3552 |   144/    8 |
| TemperatureWriter               |  21704/ 3548 |   272/    4 |
| CharWriter                      |  21620/ 3556 |   188/   12 |
| StringWriter                    |  21724/ 3560 |   292/   16 |
| StringScroller                  |  21716/ 3568 |   284/   24 |
| LevelWriter                     |  21536/ 3548 |   104/    4 |
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
| PatternWriter                   | 256752/26788 |    36/   -4 |
| NumberWriter                    | 256992/26788 |   276/   -4 |
| ClockWriter                     | 257024/26796 |   308/    4 |
| TemperatureWriter               | 257136/26788 |   420/   -4 |
| CharWriter                      | 256928/26796 |   212/    4 |
| StringWriter                    | 257160/26804 |   444/   12 |
| StringScroller                  | 257096/26812 |   380/   20 |
| LevelWriter                     | 256832/26788 |   116/   -4 |
+--------------------------------------------------------------+

```

NOTE:

* The `-4` bytes of RAM is probably an artifact of compiler optimizations that
  were triggered when additional code was added to the
  `MemoryBenchmark.ino` program compared to the `baseline` condition.

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.6

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        | 198040/13124 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   | 198068/13132 |    28/    8 |
| NumberWriter                    | 198212/13132 |   172/    8 |
| ClockWriter                     | 198268/13132 |   228/    8 |
| TemperatureWriter               | 198340/13132 |   300/    8 |
| CharWriter                      | 198236/13140 |   196/   16 |
| StringWriter                    | 198356/13140 |   316/   16 |
| StringScroller                  | 198316/13148 |   276/   24 |
| LevelWriter                     | 198132/13132 |    92/    8 |
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
| baseline                        |  10224/ 4160 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  10256/ 4164 |    32/    4 |
| NumberWriter                    |  10420/ 4164 |   196/    4 |
| ClockWriter                     |  10384/ 4168 |   160/    8 |
| TemperatureWriter               |  10576/ 4164 |   352/    4 |
| CharWriter                      |  10412/ 4172 |   188/   12 |
| StringWriter                    |  10536/ 4176 |   312/   16 |
| StringScroller                  |  10512/ 4184 |   288/   24 |
| LevelWriter                     |  10344/ 4164 |   120/    4 |
+--------------------------------------------------------------+

```

