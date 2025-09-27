#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"

//TODO: move this somewhere else its so ugly
opcode opcodes[] = {
  {NULL, "brk", NULL}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {NULL, "php", NULL}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {OP_bpl, "bpl", addr_pcr}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {OP_clc, "clc", addr_implied}, {NULL, "ora", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {NULL, "jsr", NULL}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {NULL, "plp", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {OP_bmi, "bmi", addr_pcr}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {OP_sec, "sec", addr_implied}, {NULL, "and", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {NULL, "rti", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {NULL, "pha", NULL}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "jmp", NULL}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "bvc", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {NULL, "cli", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "rts", NULL}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {NULL, "pla", NULL}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "jmp", NULL}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {OP_bvs, "bvs", addr_pcr}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {OP_sei, "sei", addr_implied}, {NULL, "adc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "rra", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {OP_bra, "bra", addr_pcr}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sax", NULL}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, {NULL, "dey", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "txa", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, 
  {OP_bcc, "bcc", addr_pcr}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "sty", NULL}, {NULL, "sta", NULL}, {NULL, "stx", NULL}, {NULL, "sax", NULL}, {NULL, "tya", NULL}, {NULL, "sta", NULL}, {NULL, "txs", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, {NULL, "sta", NULL}, {OP_nop, "nop", addr_implied}, {OP_nop, "nop", addr_implied}, 
  {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {OP_ldx, "ldx", addr_imm}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, {NULL, "tay", NULL}, {NULL, "lda", NULL}, {NULL, "tax", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, 
  {OP_bcs, "bcs", addr_pcr}, {NULL, "lda", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, {NULL, "clv", NULL}, {NULL, "lda", NULL}, {NULL, "tsx", NULL}, {NULL, "lax", NULL}, {NULL, "ldy", NULL}, {NULL, "lda", NULL}, {NULL, "ldx", NULL}, {NULL, "lax", NULL}, 
  {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "iny", NULL}, {NULL, "cmp", NULL}, {NULL, "dex", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {OP_bne, "bne", addr_pcr}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "cld", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "dcp", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {NULL, "inx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, 
  {OP_beq, "beq", addr_pcr}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {OP_sed, "sed", addr_implied}, {NULL, "sbc", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "isb", NULL}, {OP_nop, "nop", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}
};


void print_registers(context *c){
  printf("X: %x\nY: %x\nflags: %x\n", c->registers->X, c->registers->Y, c->registers->P);
}
void print_stack_addr(context *c, uint16_t addr){
  if(addr >= STACK_BOTTOM && addr <= STACK_TOP)
    printf("0x%04x : 0x%02x\n", addr, c->RAM[addr]);
}

void reset(context *c){
  c->registers->PC = ((uint16_t)c->RAM[0xFFFC]) << 8;
  c->registers->PC += c->RAM[0xFFFD];
  c->registers->S = 0xFF;
}

void step(context *c){ //step through instructions
  uint16_t current_opcode = c->RAM[c->registers->PC];
  printf("0x%04X : 0x%02X %s\n", c->registers->PC, current_opcode, opcodes[current_opcode].name);

  if(opcodes[current_opcode].func != NULL){
    (*opcodes[current_opcode].addr_mode)(c); //set addressing mode
    (*opcodes[current_opcode].func)(c); //call function associated with opcode
  }

}


int main(int argc, char* argv[]){

  cpu_registers r = {0};
  
  context c = {
    .registers = &r,
    .ea = 0
  };
  
  c.RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  
  memset(c.RAM, 0xea, 65535); //init ram with only 0xea 

  c.RAM[0xFFFC] = 0x80;
  c.RAM[0xFFFD] = 0x00;
  
  c.RAM[0x8000] = 0xa2;
  c.RAM[0x8001] = 0x41;
  c.RAM[0x8002] = 0x90;
  c.RAM[0x8003] = 0x3;
  


  //c.registers->P = 0x2;
  reset(&c);
  step(&c);  
  step(&c);
  step(&c);
  step(&c);
  print_registers(&c);
  
}
