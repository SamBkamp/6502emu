[![C CI](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml/badge.svg?branch=main)](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml)
[![Example Binaries](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml/badge.svg)](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml)
# 65C02 Emu

This is a simple 65C02 (modern 6502) emulator. Lets see if I bit off more than I can chew

Usage:
```
./s65C02 -f <bin>
```

My designs are based on [this spec](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf) documentation from Western Design Center

## Compilation

You can compile this emulator with `make`. There is also a `make clean` to clean up executable and object files.

### Compiling for this emulator

I suggest compiling binaries for this emulator with [vasm](http://sun.hasenbraten.de/vasm/). For now you can (and should) end your program with 0xbb; this unused opcode signals to the emulator that execution has ended. You can do this with `.byte $bb` (make sure you compile with -dotdir). This quitting mechanism will change with the first Beta release though. 

## How it works

This emulator seeks to emulate a 65C02 connected via data and address bus to a 65k RAM chip.

The "chip" starts with a reset function that reads an address at 0xFFFC/D which it will jump to and begin execution. Execution starts in the `step()` function where it reads the opcode at the program counter, then uses a look-up table declared in `opcode_table.h` to call 2 functions. First, the addressing mode function, these functions format the addressing mode for the opcode to process. Then the opcode function is called. Each opcode and addressing mode corresponds to a function call declared in `opcodes.h` and defined in `opcodes.c`.

There exists a CPU context/state struct defined in the main function. It contains registers, ea and a pointer to the RAM (which is allocated with an mmap call). c.registers is a pointer to a cpu_registers struct (all relevant structs are typedef'd in `prot.h`) and contains all the registers. ea is a 16-bit unsigned integer referring to a calculated effective address. This is how the addressing functions pass their calculations onto the opcodes.

The address bus and data bus are not explicitly emulated, but you can imagine a read from the bus to look like:
```c
data_bus = cpu_context->RAM[address_bus]; //data bus read from ram
```
and a write to look like:
```c
cpu_context->RAM[address_bus] = data_bus; //ram write through busses
```
Its all abstracted away but you can still see its shadow in the design.

## TODO
- [ ] helper functions for common flag setting (set n flag to b7 of operand/register & set z flag)
- [ ] virtual LED pins
- [ ] cycle accuracy (+ memory page implementation / cross page read penalty)
- [ ] implement remaining illegal opcodes
- [ ] more example programs

## Implementation progress:

#### Addressing modes:

All implemented! :)

#### op codes implemented:

Unimplemented:
- Standard opcodes:
  - All implemented! :)
  
- Illegal Opcodes:
  - slo
  - rla
  - sre
  - rra
  - sax
  - lax
  - dcp
  - isb