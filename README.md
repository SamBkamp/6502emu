[![C CI](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml/badge.svg?branch=main)](https://github.com/SamBkamp/6502emu/actions/workflows/c-CI.yml)
[![Example Binaries](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml/badge.svg)](https://github.com/SamBkamp/6502emu/actions/workflows/example_bin.yml)
# 65C02 Emu

This is a simple 65C02 (modern 6502) emulator. Lets see if I bit off more than I can chew

Usage:
```
./s65C02 -f <bin>
```

## Compilation

```bash
make
```
you can reset the directory with
```bash
make clean
```
and you can remove all uneeded object files after compilation with
```bash
make cleanobj
```

There are also pre-compiled binaries for linux on the [releases](https://github.com/SamBkamp/6502emu/releases) page

## How it works

You can read more information about the inner workings of the emulator in the [wiki](https://github.com/SamBkamp/6502emu/wiki)

## TODO
- [ ] better address decoding logic for bus functions / vChip registration
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