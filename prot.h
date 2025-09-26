
#ifndef PROT_6502
#define PROT_6520
//prototypes for global functions and variables

typedef struct{
  uint8_t A; //accumulator
  uint8_t Y; //index Y
  uint8_t X; //index X
  uint16_t PC; //program counter
  uint8_t S; //Stack pointer
  uint8_t P; //flags
}cpu_registers;

typedef struct{
  cpu_registers *registers;
  uint16_t ea; //effective address
}context;

uint8_t *RAM;

#endif
