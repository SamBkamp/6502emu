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

This emulator seeks to emulate a 65C02 connected via data and address bus to a 32k ROM chip (at 0x8000-0xFFFF) and a 32k RAM chip (0x0000-0x7FFF). You can change, extend and implement your own chips fairly easily.

The "chip" starts with a reset function that reads an address at 0xFFFC/D which it will jump to and begin execution. Execution starts in the `step()` function where it reads the opcode at the program counter, then uses a look-up table declared in `opcode_table.h` to call 2 functions. First, the addressing mode function, these functions format the addressing mode for the opcode to process. Then the opcode function is called. Each opcode and addressing mode corresponds to a function call declared in `opcodes.h` and defined in `opcodes.c`.

There exists a CPU context/state struct defined in the main function. It contains `registers` and `ea`. `c.registers` is a pointer to a cpu_registers struct (all relevant structs are typedef'd in `prot.h`) and contains all the registers. `ea` is a 16-bit unsigned integer referring to a calculated effective address. This is how the addressing functions pass their calculations onto the opcodes.

the data bus and address bus are emulated in the functions bus_write and bus_read. They function both as the bus on the 6502 and as an address decoder to put the address and data info to the correct virtual chip.

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