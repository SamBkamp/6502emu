#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"
#include "opcode_table.h"


void reset(context *c){
  c->registers->PC = ((uint16_t)c->RAM[0xFFFD]) << 8;
  c->registers->PC += c->RAM[0xFFFC];
  c->registers->S = 0xFF;
  c->registers->P = 0x3C;
}

int step(context *c){ 
  if(c->RAM[c->registers->PC] == 0xbb) return -1; //custom opcode
  
  uint8_t current_opcode = c->RAM[c->registers->PC]; //only used for logging
  uint16_t current_pc = c->registers->PC; //only used for logging  
  if(opcodes[current_opcode].func != NULL){
    (*opcodes[current_opcode].addr_mode)(c); //set addressing mode
    (*opcodes[current_opcode].func)(c); //call function associated with opcode
  }
  //↓ this is so ugly please im sure this can be done better
  //for cases where opcode doesn't use data from program (eg NOP)
  if(c->registers->PC - current_pc > 1)
    printf("0x%04X : 0x%02X %s 0x%x\n", current_pc, current_opcode, opcodes[current_opcode].name, c->final_addr);
  else
    printf("0x%04X : 0x%02X %s\n", current_pc, current_opcode, opcodes[current_opcode].name);

  return 1;
}



int main(int argc, char* argv[]){
  size_t memory_size = 0xffff; 
  cpu_registers r = {0};
  context c = {
    .registers = &r,
    .ea = 0,
    .RAM = (uint8_t *)mmap(NULL, memory_size, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0)
  };

  //check mmap
  if (c.RAM == (void *)-1){
    fprintf(stderr, "mmap failed\n");
    return 1;
  }
  cmd_flags flags = read_cmd_line(argc, argv);

  //check if infile supplied
  if(flags.infile == NULL){
    fprintf(stderr, "Usage: %s -f [filename]\n", argv[0]);
    return 1;
  }
  //load file into memory
  if(load_file(c.RAM, flags.infile, memory_size) == 0)
    return 1;
  //print license
  license();
  printf("-------------- program start --------------\n");
  reset(&c);
  int q = step(&c);
  int max_step = 100;
  int steps = 0;
  while(q > 0 && c.registers->PC < memory_size && steps < max_step){    
    q = step(&c);
    steps++;    
  }
  printf("-------------- program complete --------------\n");
  printf("0x%04x : 0x%02x\n", 0x80ff, c.RAM[0x80ff]);
  print_registers(&c);
  munmap(c.RAM, memory_size); //release heap memory
}
