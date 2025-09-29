#ifndef PROT_6502
#define PROT_6520
//prototypes for global functions and variables

#define FLAGS_C_MASK 0x1
#define FLAGS_Z_MASK 0x2
#define FLAGS_I_MASK 0x4
#define FLAGS_D_MASK 0x8
#define FLAGS_B_MASK 0x10
#define FLAGS_V_MASK 0x40
#define FLAGS_N_MASK 0x80


#define STACK_BOTTOM 0x0100
#define STACK_TOP 0x01ff


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
  uint8_t *RAM; //ram address
  uint16_t final_addr; //for logging
}context;


typedef struct{
  void (*func)(context *c);
  char* name;
  void (*addr_mode)(context *c);
}opcode;


/*

  flags register:
  | N | V | 1 | B | D | I | Z | C |
  ^MSB                         LSB^

  N = negative (1 = neg)
  V = Overflow (1 = true)
  1 = undef.
  B = BRK command (1 = BRK, 0 = IRQB)
  D = Decimal mode (1 = true)
  I = IRQB disable (1 = disable)
  Z = zero (1 = true)
  C = carry (1 = true)

  source: https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf
 */

#endif
