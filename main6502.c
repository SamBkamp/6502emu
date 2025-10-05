#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"
#include "opcode_table.h"
#include "modules/ROM.h"
#include "modules/RAM.h"

uint8_t *address_space;
chip chips[2];



void bus_write(uint16_t address, uint8_t data){
  uint16_t a = address%0x8000; //0x8000 is bucket size
  a = address-a; //round down to closest bucket (either 0x0 or 0x8000)
  a >>= 15;  //make a equal to MSB
  return (*chips[a].chip_write)(address-(0x8000*a), data);
}
uint8_t bus_read(uint16_t address){
  uint16_t a = address%0x8000;
  a = address-a;
  a >>= 15;  
  return (*chips[a].chip_read)(address-(0x8000*a));
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
  };
  
  cmd_flags flags = read_cmd_line(argc, argv);

  //check if infile supplied
  if(flags.infile == NULL){
    fprintf(stderr, "Usage: %s -f [filename]\n", argv[0]);
    return 1;
  }
  // INIT EXTERNAL CHIPS
  chips[0] = (chip){
    .name = "RAM",
    .chip_read = RAM_read,
    .chip_write = RAM_write,
    .chip_init = RAM_init
  };
  chips[1] = (chip){
    .name = "ROM",
    .chip_read = ROM_read,
    .chip_write = ROM_write,
    .chip_init = ROM_init
  };
  uint8_t *add = (*chips[1].chip_init)();
  (*chips[0].chip_init)();
  //END INIT EXTNERNAL CHIPS
  
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
  print_registers(&c);
  //munmap(address_space, memory_size); //release heap memory
}
