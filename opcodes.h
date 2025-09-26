#ifndef OPCODES_6502
#define OPCODES_6502

typedef struct{
  void (*func)(context *c);
  char* name;
  void (*addr_mode)(context *c);
}opcode;

/* addressing moding functions */

void addr_implied(context *c){ //implied i
}

void addr_accumulator(context *c){ //accumulator A
}

void addr_imm(context *c){ //immediate #
  c->registers->PC += 1;
  c->ea = c->registers->PC;
}

void addr_pcr(context *c){ //program counter relative r
  c->registers->PC += 1;
  c->ea = c->registers->PC + RAM[c->registers->PC];
}

/* opcode implementations */
void OP_bcc(context *c){ //branch carry clear
  if((c->registers->P & 1) == 0){
    c->registers->PC = c->ea;
  }
}

void OP_bcs(context *c){ //branch carry set
  if((c->Registers->P & 1) != 0){
    c->registers->PC = c->ea;
  }
}

void OP_nop(context *c){
  //printf("hello from nop!\n");
  return;
}

void OP_ldx(context *c){
  c->registers->X = RAM[c->ea];
}

opcode opcodes[] = {
  {NULL, "brk", NULL}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {NULL, "php", NULL}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {NULL, "bpl", NULL}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {NULL, "clc", NULL}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {NULL, "jsr", NULL}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {NULL, "plp", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {NULL, "bmi", NULL}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {NULL, "sec", NULL}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {NULL, "rti", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {NULL, "pha", NULL}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "jmp", NULL}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "bvc", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {NULL, "cli", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "rts", NULL}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {NULL, "pla", NULL}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "jmp", NULL}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {NULL, "bvs", NULL}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {NULL, "sei", NULL}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {OP_nop, "nop", addr_implied}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sax", NULL}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, {NULL, "dey", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "txa", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, 
  {OP_bcc, "bcc", addr_pcr}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, {NULL, "tya", NULL}, {NULL, "sta", NULL}, {NULL, "txs", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, 
  {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {OP_ldx, "ldx", addr_imm}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, {NULL, "tay", NULL}, {NULL, "lda", NULL}, {NULL, "tax", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, 
  {OP_bcs, "bcs", addr_pcr}, {NULL, "lda", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, {NULL, "clv", NULL}, {NULL, "lda", NULL}, {NULL, "tsx", NULL}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, 
  {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "iny", NULL}, {NULL, "cmp", NULL}, {NULL, "dex", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {NULL, "bne", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "cld", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {NULL, "inx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, 
  {NULL, "beq", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {NULL, "sed", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}
};

#endif
