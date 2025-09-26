#ifndef REGISTERS_6502
#define REGISTERS_6502

//register name -> offset (assuming theyre all in a contigious memory location of size REG_SIZE)
//register_name_size -> size (in bytes)

#define IR 0 //instruction register
#define IR_size 1

#define TCU 1 //Timing control unit
#define TCU_SIZE 1

#define ALU 2 //Arithmetic and logic unit
#define ALU_SIZE 1

#define A 3 //Accumulator register
#define A_SIZE 1

#define X 4 //index register
#define X_SIZE 1

#define Y 5 //index register
#define Y_SIZE 1

#define P 6 //processor status register (flag)
#define P_SIZE 1

#define PC 7 //program counter
#define PC_SIZE 2

#define S 9 //stack pointer
#define S_SIZE 1

#define REG_SIZE 10


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
