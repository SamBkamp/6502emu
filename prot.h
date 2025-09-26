
#ifndef PROT_6502
#define PROT_6520
//prototypes for global functions and variables

uint8_t registers[10]; //all cpu registers
uint8_t *RAM;

void reset(void);
void fetch_data(void);
void print_registers();
void step(void);
void load_into_pc(uint16_t v);
void load_uint16_into_ram(uint16_t addr, uint16_t val);

#endif
