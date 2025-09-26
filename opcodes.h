#ifndef OPCODES_6502
#define OPCODES_6502

typedef struct{
  void (*func)(void);
  char* name;
}opcode;

void OP_nop(void){
  printf("hello from nop!\n");
  return;
}


void OP_ldx(void){
  registers[PC+1] += 1;
  address_bus += 1;
  fetch_data();
  registers[X] = data_bus;
}

opcode opcodes[] = {
  {NULL, "brk"}, {NULL, "ora"}, {OP_nop, "nop"}, {NULL, "slo"}, {OP_nop, "nop"}, {NULL, "ora"}, {NULL, "asl"}, {NULL, "slo"}, {NULL, "php"}, {NULL, "ora"}, {NULL, "asl"}, {OP_nop, "nop"}, {OP_nop, "nop"}, {NULL, "ora"}, {NULL, "asl"}, {NULL, "slo"}, 
{NULL, "bpl"}, {NULL, "ora"}, {OP_nop, "nop"}, {NULL, "slo"}, {OP_nop, "nop"}, {NULL, "ora"}, {NULL, "asl"}, {NULL, "slo"}, {NULL, "clc"}, {NULL, "ora"}, {OP_nop, "nop"}, {NULL, "slo"}, {OP_nop, "nop"}, {NULL, "ora"}, {NULL, "asl"}, {NULL, "slo"}, 
{NULL, "jsr"}, {NULL, "and"}, {OP_nop, "nop"}, {NULL, "rla"}, {NULL, "bit"}, {NULL, "and"}, {NULL, "rol"}, {NULL, "rla"}, {NULL, "plp"}, {NULL, "and"}, {NULL, "rol"}, {OP_nop, "nop"}, {NULL, "bit"}, {NULL, "and"}, {NULL, "rol"}, {NULL, "rla"}, 
{NULL, "bmi"}, {NULL, "and"}, {OP_nop, "nop"}, {NULL, "rla"}, {OP_nop, "nop"}, {NULL, "and"}, {NULL, "rol"}, {NULL, "rla"}, {NULL, "sec"}, {NULL, "and"}, {OP_nop, "nop"}, {NULL, "rla"}, {OP_nop, "nop"}, {NULL, "and"}, {NULL, "rol"}, {NULL, "rla"}, 
{NULL, "rti"}, {NULL, "eor"}, {OP_nop, "nop"}, {NULL, "sre"}, {OP_nop, "nop"}, {NULL, "eor"}, {NULL, "lsr"}, {NULL, "sre"}, {NULL, "pha"}, {NULL, "eor"}, {NULL, "lsr"}, {OP_nop, "nop"}, {NULL, "jmp"}, {NULL, "eor"}, {NULL, "lsr"}, {NULL, "sre"}, 
{NULL, "bvc"}, {NULL, "eor"}, {OP_nop, "nop"}, {NULL, "sre"}, {OP_nop, "nop"}, {NULL, "eor"}, {NULL, "lsr"}, {NULL, "sre"}, {NULL, "cli"}, {NULL, "eor"}, {OP_nop, "nop"}, {NULL, "sre"}, {OP_nop, "nop"}, {NULL, "eor"}, {NULL, "lsr"}, {NULL, "sre"}, 
{NULL, "rts"}, {NULL, "adc"}, {OP_nop, "nop"}, {NULL, "rra"}, {OP_nop, "nop"}, {NULL, "adc"}, {NULL, "ror"}, {NULL, "rra"}, {NULL, "pla"}, {NULL, "adc"}, {NULL, "ror"}, {OP_nop, "nop"}, {NULL, "jmp"}, {NULL, "adc"}, {NULL, "ror"}, {NULL, "rra"}, 
{NULL, "bvs"}, {NULL, "adc"}, {OP_nop, "nop"}, {NULL, "rra"}, {OP_nop, "nop"}, {NULL, "adc"}, {NULL, "ror"}, {NULL, "rra"}, {NULL, "sei"}, {NULL, "adc"}, {OP_nop, "nop"}, {NULL, "rra"}, {OP_nop, "nop"}, {NULL, "adc"}, {NULL, "ror"}, {NULL, "rra"}, 
{OP_nop, "nop"}, {NULL, "sta"}, {OP_nop, "nop"}, {NULL, "sax"}, {NULL, "sty"}, {NULL, "sta"}, {NULL, "stx"}, {NULL, "sax"}, {NULL, "dey"}, {OP_nop, "nop"}, {NULL, "txa"}, {OP_nop, "nop"}, {NULL, "sty"}, {NULL, "sta"}, {NULL, "stx"}, {NULL, "sax"}, 
{NULL, "bcc"}, {NULL, "sta"}, {OP_nop, "nop"}, {OP_nop, "nop"}, {NULL, "sty"}, {NULL, "sta"}, {NULL, "stx"}, {NULL, "sax"}, {NULL, "tya"}, {NULL, "sta"}, {NULL, "txs"}, {OP_nop, "nop"}, {OP_nop, "nop"}, {NULL, "sta"}, {OP_nop, "nop"}, {OP_nop, "nop"}, 
{NULL, "ldy"}, {NULL, "lda"}, {OP_ldx, "ldx"}, {NULL, "lax"}, {NULL, "ldy"}, {NULL, "lda"}, {OP_ldx, "ldx"}, {NULL, "lax"}, {NULL, "tay"}, {NULL, "lda"}, {NULL, "tax"}, {OP_nop, "nop"}, {NULL, "ldy"}, {NULL, "lda"}, {OP_ldx, "ldx"}, {NULL, "lax"}, 
{NULL, "bcs"}, {NULL, "lda"}, {OP_nop, "nop"}, {NULL, "lax"}, {NULL, "ldy"}, {NULL, "lda"}, {OP_ldx, "ldx"}, {NULL, "lax"}, {NULL, "clv"}, {NULL, "lda"}, {NULL, "tsx"}, {NULL, "lax"}, {NULL, "ldy"}, {NULL, "lda"}, {OP_ldx, "ldx"}, {NULL, "lax"}, 
{NULL, "cpy"}, {NULL, "cmp"}, {OP_nop, "nop"}, {NULL, "dcp"}, {NULL, "cpy"}, {NULL, "cmp"}, {NULL, "dec"}, {NULL, "dcp"}, {NULL, "iny"}, {NULL, "cmp"}, {NULL, "dex"}, {OP_nop, "nop"}, {NULL, "cpy"}, {NULL, "cmp"}, {NULL, "dec"}, {NULL, "dcp"}, 
{NULL, "bne"}, {NULL, "cmp"}, {OP_nop, "nop"}, {NULL, "dcp"}, {OP_nop, "nop"}, {NULL, "cmp"}, {NULL, "dec"}, {NULL, "dcp"}, {NULL, "cld"}, {NULL, "cmp"}, {OP_nop, "nop"}, {NULL, "dcp"}, {OP_nop, "nop"}, {NULL, "cmp"}, {NULL, "dec"}, {NULL, "dcp"}, 
{NULL, "cpx"}, {NULL, "sbc"}, {OP_nop, "nop"}, {NULL, "isb"}, {NULL, "cpx"}, {NULL, "sbc"}, {NULL, "inc"}, {NULL, "isb"}, {NULL, "inx"}, {NULL, "sbc"}, {OP_nop, "nop"}, {NULL, "sbc"}, {NULL, "cpx"}, {NULL, "sbc"}, {NULL, "inc"}, {NULL, "isb"}, 
{NULL, "beq"}, {NULL, "sbc"}, {OP_nop, "nop"}, {NULL, "isb"}, {OP_nop, "nop"}, {NULL, "sbc"}, {NULL, "inc"}, {NULL, "isb"}, {NULL, "sed"}, {NULL, "sbc"}, {OP_nop, "nop"}, {NULL, "isb"}, {OP_nop, "nop"}, {NULL, "sbc"}, {NULL, "inc"}, {NULL, "isb"}
};

#endif
