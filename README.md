# 65C02 Emu

This is a simple 65C02 (modern 6502) emulator. Lets see if I bit off more than I can chew

My designs are based on [this spec](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf) documentation from Western Design Center

# BE WARNED!

I began this project on 26/Sep/2025 so if youre reading this, this project is likely still its infantile stage and code organisation can change rapidly and randomly until ive settled into a project structure I like

## Implementation progress:

#### Addressing modes:
- [ ] Absolute A
- [ ] Absolute Indexed Indirect (a,x)
- [ ] Absolute Indexed with X a,x
- [ ] Absolute Indexed Indirect (a,x)
- [ ] Absolute Indirect (a)
- [ ] Accumulator A
- [x] Immediate Addressing #
- [x] Implied i
- [ ] Program Counter Relative r
- [ ] Stack s
- [ ] Zero Page zp
- [ ] Zero Page Indexed Indirect (zp,x)
- [ ] Zero Page Indexed with X zp,x
- [ ] Zero Page Indexed with Y zp, y
- [ ] Zero Page Indirect (zp)
- [ ] Zero Page Indirect Indexed with Y (zp), y

#### op codes implemented:
- [x] NOP