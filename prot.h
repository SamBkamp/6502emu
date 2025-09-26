
#ifndef PROT_6502
#define PROT_6520
//prototypes for global functions and variables

typedef struct{
  uint8_t registers[10]; //all registers
  uint16_t ea; //effective address
}context;

uint8_t *RAM;

void load_into_pc(context *c, uint16_t v);
void load_uint16_into_ram(uint16_t addr, uint16_t val);

#endif
