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

**Version**: AceSegmentWriter 0.5

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
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SpenceKonde/ATTinyCore 1.5.2

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |    274/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    376/   19 |   102/    8 |
| NumberWriter                    |    584/   21 |   310/   10 |
| NumberWriter::writeFloat()      |   2102/   31 |  1828/   20 |
| ClockWriter                     |    630/   24 |   356/   13 |
| TemperatureWriter               |    750/   23 |   476/   12 |
| CharWriter                      |    568/   24 |   294/   13 |
| StringWriter                    |    676/   26 |   402/   15 |
| StringScroller                  |    768/   32 |   494/   21 |
| LevelWriter                     |    478/   21 |   204/   10 |
+--------------------------------------------------------------+

```

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Arduino AVR Boards 1.8.5

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |    470/   11 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |    574/   19 |   104/    8 |
| NumberWriter                    |    758/   21 |   288/   10 |
| NumberWriter::writeFloat()      |   2476/   45 |  2006/   34 |
| ClockWriter                     |    834/   24 |   364/   13 |
| TemperatureWriter               |    954/   23 |   484/   12 |
| CharWriter                      |    768/   24 |   298/   13 |
| StringWriter                    |    864/   26 |   394/   15 |
| StringScroller                  |    980/   32 |   510/   21 |
| LevelWriter                     |    678/   21 |   208/   10 |
+--------------------------------------------------------------+

```

### Sparkfun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |   3486/  151 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |   3552/  159 |    66/    8 |
| NumberWriter                    |   3736/  161 |   250/   10 |
| NumberWriter::writeFloat()      |   5432/  185 |  1946/   34 |
| ClockWriter                     |   3812/  164 |   326/   13 |
| TemperatureWriter               |   3932/  163 |   446/   12 |
| CharWriter                      |   3746/  164 |   260/   13 |
| StringWriter                    |   3850/  166 |   364/   15 |
| StringScroller                  |   3958/  172 |   472/   21 |
| LevelWriter                     |   3656/  161 |   170/   10 |
+--------------------------------------------------------------+

```

### STM32 Blue Pill

* STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* STM32duino 2.4.0

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |  21408/ 3564 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  21456/ 3572 |    48/    8 |
| NumberWriter                    |  21572/ 3576 |   164/   12 |
| NumberWriter::writeFloat()      |  25084/ 3576 |  3676/   12 |
| ClockWriter                     |  21648/ 3584 |   240/   20 |
| TemperatureWriter               |  21692/ 3580 |   284/   16 |
| CharWriter                      |  21620/ 3584 |   212/   20 |
| StringWriter                    |  21716/ 3588 |   308/   24 |
| StringScroller                  |  21736/ 3596 |   328/   32 |
| LevelWriter                     |  21532/ 3576 |   124/   12 |
+--------------------------------------------------------------+

```

### ESP8266

* NodeMCU 1.0, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP8266 Boards 3.0.2

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        | 260121/27900 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   | 260173/27912 |    52/   12 |
| NumberWriter                    | 260397/27920 |   276/   20 |
| NumberWriter::writeFloat()      | 261741/27920 |  1620/   20 |
| ClockWriter                     | 260477/27928 |   356/   28 |
| TemperatureWriter               | 260541/27920 |   420/   20 |
| CharWriter                      | 260365/27928 |   244/   28 |
| StringWriter                    | 260501/27928 |   380/   28 |
| StringScroller                  | 260533/27936 |   412/   36 |
| LevelWriter                     | 260253/27920 |   132/   20 |
+--------------------------------------------------------------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP32 Boards 2.0.7

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        | 229425/21984 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   | 229489/21992 |    64/    8 |
| NumberWriter                    | 229629/22000 |   204/   16 |
| NumberWriter::writeFloat()      | 230561/22000 |  1136/   16 |
| ClockWriter                     | 229697/22008 |   272/   24 |
| TemperatureWriter               | 229761/22000 |   336/   16 |
| CharWriter                      | 229661/22008 |   236/   24 |
| StringWriter                    | 229769/22008 |   344/   24 |
| StringScroller                  | 229793/22016 |   368/   32 |
| LevelWriter                     | 229569/22000 |   144/   16 |
+--------------------------------------------------------------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Teensyduino 1.57
* Compiler options: "Faster"

```
+--------------------------------------------------------------+
| functionality                   |  flash/  ram |       delta |
|---------------------------------+--------------+-------------|
| baseline                        |  10108/ 4160 |     0/    0 |
|---------------------------------+--------------+-------------|
| PatternWriter                   |  10156/ 4168 |    48/    8 |
| NumberWriter                    |  10336/ 4172 |   228/   12 |
| NumberWriter::writeFloat()      |  13916/ 4172 |  3808/   12 |
| ClockWriter                     |  10476/ 4180 |   368/   20 |
| TemperatureWriter               |  10532/ 4176 |   424/   16 |
| CharWriter                      |  10328/ 4180 |   220/   20 |
| StringWriter                    |  10440/ 4184 |   332/   24 |
| StringScroller                  |  10456/ 4192 |   348/   32 |
| LevelWriter                     |  10244/ 4172 |   136/   12 |
+--------------------------------------------------------------+

```

