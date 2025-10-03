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

#define BIT_0_MASK 0x1
#define BIT_1_MASK 0x2
#define BIT_2_MASK 0x4
#define BIT_3_MASK 0x8
#define BIT_4_MASK 0x10
#define BIT_5_MASK 0x20
#define BIT_6_MASK 0x40
#define BIT_7_MASK 0x80


#define STACK_BOTTOM 0x0100
#define STACK_TOP 0x01ff

#define IRQB_VEC 0xFFFE //low byte
#define RESB_VEC 0xFFFC //low byte

typedef struct{
  char* infile;
}cmd_flags;

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

void license();
void print_registers(context *c);
void print_stack_addr(context *c, uint16_t addr);
int load_file(void *dest, char* filename, size_t max_size);
cmd_flags read_cmd_line(int argc, char** argv);
#endif
