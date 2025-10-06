[![C CI](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml/badge.svg?branch=main)](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml)
[![Example Binaries](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml/badge.svg)](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml)
# 65C02 Emu

This is a simple 65C02 (modern 6502) emulator. Lets see if I bit off more than I can chew

Usage:
```
./s65C02 -f <bin>
```

My designs are based on [this spec](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf) documentation from Western Design Center

## Running the emulator

You can compile this emulator with `make`. There is also a `make clean` to clean up executable and object files and `make cleanobj` if you want to get rid of leftover object files.

There are also pre-compiled binaries for linux on the [releases](https://github.com/SamBkamp/6502emu/releases) page

### Compiling for this emulator

I suggest compiling binaries for this emulator with [vasm](http://sun.hasenbraten.de/vasm/). For now you can (and should) end your program with 0xbb; this unused opcode signals to the emulator that execution has ended. You can do this with `.byte $bb` (make sure you compile with -dotdir). This quitting mechanism will change with the first Beta release though. 

## How it works

You can read more information about the inner workings of the emulator in the [wiki](https://github.com/SamBkamp/6502emu/wiki)

## TODO
- [ ] helper functions for common flag setting (set n flag to b7 of operand/register & set z flag)
- [ ] virtual LED pins
- [ ] cycle accuracy (+ memory page implementation / cross page read penalty)
- [ ] implement remaining illegal opcodes
- [ ] more example programs

#### Unimplemented Opcodes
  - slo
  - rla
  - sre
  - rra
  - sax
  - lax
  - dcp
  - isb