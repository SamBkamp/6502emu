#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"
#include "opcode_table.h"
#include "modules/ROM.h"

uint8_t *address_space;
chip chips[2];



void bus_write(uint16_t address, uint8_t data){
  if(address >= 0x8000)
    (*chips[0].chip_write)(address-0x8000, data);
  else
    address_space[address] = data;
}
uint8_t bus_read(uint16_t address){
  if(address >= 0x8000)
    return (*chips[0].chip_read)(address-0x8000);
  else    
    return address_space[address];
}

void reset(context *c){
  c->registers->PC = ((uint16_t)bus_read(0xFFFD)) << 8;
  c->registers->PC += bus_read(0xFFFC);
  c->registers->S = 0xFF;
  c->registers->P = 0x3C;
}

int step(context *c){ 
  if(bus_read(c->registers->PC) == 0xbb) return -1; //custom opcode
  
  uint8_t current_opcode = bus_read(c->registers->PC); //only used for logging
  uint16_t current_pc = c->registers->PC; //only used for logging  
  if(opcodes[current_opcode].func != NULL){
    (*opcodes[current_opcode].addr_mode)(c); //set addressing mode
    (*opcodes[current_opcode].func)(c); //call function associated with opcode
  }
  //↓ this is so ugly please im sure this can be done better
  //for cases where opcode doesn't use data from program (eg NOP) AND BRK AND NOT RTI
  if((c->registers->PC - current_pc > 1 || current_opcode == 0) && current_opcode != 0x40)
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
    .RAM = 0
  };
  address_space = mmap(NULL, memory_size, PROT_READ|PROT_WRITE|PROT_EXEC,
		       MAP_PRIVATE|MAP_ANON ,-1, 0);

  //check mmap
  if (address_space == (void *)-1){
    fprintf(stderr, "mmap failed\n");
    return 1;
  }
  
  cmd_flags flags = read_cmd_line(argc, argv);

  //check if infile supplied
  if(flags.infile == NULL){
    fprintf(stderr, "Usage: %s -f [filename]\n", argv[0]);
    return 1;
  }
  chips[0] = (chip){
    .name = "ROM",
    .chip_read = ROM_read,
    .chip_write = ROM_write,
    .chip_init = ROM_init
  };
  uint8_t *add = (*chips[0].chip_init)();
  //load file into memory
  if(load_file(add, flags.infile, 32768+1) == 0)
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
  printf("0x%04x : 0x%02x\n", 0x80ff, address_space[0x80ff]);
  print_registers(&c);
  munmap(address_space, memory_size); //release heap memory
}
