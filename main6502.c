#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"
#include "opcode_table.h"

typedef struct{
  char *infile;
}cmd_flags;

void license(){
  printf("Copyright (c) 2025 Sam Bonnekamp under the MIT license. Read included License.txt or mit-license.org\n");
}

void print_registers(context *c){
  printf("X: 0x%x\nY: 0x%x\nA: 0x%x\nS: 0x%x\n",
	 c->registers->X, c->registers->Y, c->registers->A, c->registers->S );
  //format flags
  printf("flags:\n N | V | 1 | B | D | I | Z | C |\n");
  for(int i = 7; i >= 0; i--){
    printf(" %d |",
	   (c->registers->P & (1 << i)) >> i);    
  }
  printf("\n");
}
void print_stack_addr(context *c, uint16_t addr){
  if(addr >= STACK_BOTTOM && addr <= STACK_TOP)
    printf("0x%04x : 0x%02x\n", addr, c->RAM[addr]);
}

void reset(context *c){
  c->registers->PC = ((uint16_t)c->RAM[0xFFFD]) << 8;
  c->registers->PC += c->RAM[0xFFFC];
  c->registers->S = 0xFF;
  c->registers->P = 0x3C;
}

//im not sure how much overhead logging adds, Im sure we could a --no-logging flag that wraps the printf in an 'if', though I wonder if assinging current_opcode and _pc could be factored out better for that case specifically
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
  cmd_flags flags;
  cpu_registers r = {0};
  context c = {
    .registers = &r,
    .ea = 0
  };
  
  c.RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);

  //read command line arguments
  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "-f") == 0 && argc >= i+1){
      flags.infile = argv[++i];
    }
  }
  //check infile
  if(flags.infile == NULL){
    fprintf(stderr, "Usage: %s -f [filename]\n", argv[0]);
    return 1;
  }
  //open infile
  FILE *fptr = fopen(flags.infile, "r");
  if (fptr == NULL) {
    fprintf(stderr, "File I/O Error: Failed to open file %s\n", flags.infile);
    return 1;
  }
  //get and check file size
  fseek(fptr, 0L, SEEK_END);  
  long file_len = ftell(fptr);
  rewind(fptr); //reset file cursor
  if(file_len > memory_size+1){
    fprintf(stderr, "File I/O Error: File too large (max %zub)\n", memory_size);
    return 1;
  }
  //read binary file into memory
  fread(c.RAM, 1, file_len, fptr);
 

  license();
  
  c.registers->Y = (2025-1900);
  c.registers->X = 10;
  c.registers->A = 1;
  
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
  //print_stack_addr(&c, STACK_BOTTOM + c.registers->S + 2);
  //print_stack_addr(&c, STACK_BOTTOM + c.registers->S + 1);
  print_registers(&c);
}
