# 65C02 Emu

This is a simple 65C02 (modern 6502) emulator. Lets see if I bit off more than I can chew

My designs are based on [this spec](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf) documentation from Western Design Center

## Compilation

You can compile this emulator with `make`, if you are writing code for this project I suggest you run `make dev` instead. There is also a `make clean` to clean up executable files.

## How it works

This emulator seeks to emulate a 65C02 connected via data and address bus to a 65k RAM chip.

The "chip" starts with a reset function that reads an address at 0xFFFC/D which it will jump to and begin execution. Execution starts in the `step()` function where it reads the opcode at the program counter, then uses a look-up table declared at the top of `main6502.c` to call 2 functions. First, the addressing mode function, these functions format the addressing mode for the opcode to process. Then the opcode function is called. Each opcode and addressing mode corresponds to a function call declared in `opcodes.h` and defined in `opcodes.c`.

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

## Implementation progress:

#### Addressing modes:
- [ ] Absolute A
- [ ] Absolute Indexed Indirect (a,x)
- [ ] Absolute Indexed with X a,x
- [ ] Absolute Indexed Indirect (a,x)
- [ ] Absolute Indirect (a)
- [x] Accumulator A
- [x] Immediate Addressing #
- [x] Implied i
- [x] Program Counter Relative r
- [x] Stack s
- [x] Zero Page zp
- [ ] Zero Page Indexed Indirect (zp,x)
- [ ] Zero Page Indexed with X zp,x
- [ ] Zero Page Indexed with Y zp, y
- [ ] Zero Page Indirect (zp)
- [ ] Zero Page Indirect Indexed with Y (zp), y

#### op codes implemented:
- [x] NOP
- [x] All Branching call
- [x] All Status flag change calls
- [x] All stack push/pull calls
- [x] LDX