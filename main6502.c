#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"
#include "opcode_table.h"
#include "helper.h"


int main(int argc, char* argv[]){
  size_t memory_size = 0xffff; 
  cpu_registers r = {0};
  context c = {
    .registers = &r,
    .ea = 0,
    .RAM = (uint8_t *)mmap(NULL, memory_size, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0)
  };

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
