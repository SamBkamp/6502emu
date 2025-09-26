#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"

void print_registers(context *c){
  printf("X: %x\nY: %x\nflags: %x\n", c->registers->X, c->registers->Y, c->registers->P);
}

void reset(context *c){
  c->registers->PC = ((uint16_t)RAM[0xFFFC]) << 8;
  c->registers->PC += RAM[0xFFFD];
}

void step(context *c){ //step through instructions
  uint16_t current_opcode = RAM[c->registers->PC];
  printf("0x%04X : 0x%02X %s\n", c->registers->PC, current_opcode, opcodes[current_opcode].name);

  if(opcodes[current_opcode].func != NULL){
    (*opcodes[current_opcode].addr_mode)(c); //set addressing mode
    (*opcodes[current_opcode].func)(c); //call function associated with opcode
  }

  c->registers->PC += 1;
}


int main(int argc, char* argv[]){

  cpu_registers r = {0};
  
  context c = {
    .registers = &r,
    .ea = 0
  };
  
  RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  
  memset(RAM, 0xea, 65535); //init ram with only 0xea 

  RAM[0xFFFC] = 0x80;
  RAM[0xFFFD] = 0x00;

  RAM[0x8000] = 0xa2;
  RAM[0x8001] = 0x41;
  RAM[0x8002] = 0x90;
  RAM[0x8003] = 0x3;

  //c.registers->P = 1;
  reset(&c);
  step(&c);  
  step(&c);
  step(&c);
  step(&c);
  print_registers(&c);
  
}
