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
#include "modules/screen.h"

#define BUCKETS 256 //how many address buckets
#define BUCKET_SIZE 256 //each address bucket size
#define BUCKET_LOG 15 //log2 BUCKET_SIZE, for easy division to turn buckets into array offsets

uint8_t *address_space;
chip *chips[BUCKETS];
cmd_flags flags;


void bus_write(uint16_t address, uint8_t data){
  uint16_t a = address - (address%BUCKET_SIZE); //round down to closest bucket
  return (*chips[a>>8]->chip_write)(address, data);
}
uint8_t bus_read(uint16_t address){
  uint16_t a = address - (address%BUCKET_SIZE);
  return (*chips[a>>8]->chip_read)(address);
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
  if(flags.logging_level > 1
     && (c->registers->PC - current_pc > 1 || current_opcode == 0)
     && current_opcode != 0x40){
    printf("0x%04X : 0x%02X %s 0x%x\n",
	   current_pc,
	   current_opcode,
	   opcodes[current_opcode].name,
	   c->final_addr);
  }else if (flags.logging_level > 1){
    printf("0x%04X : 0x%02X %s\n",
	   current_pc,
	   current_opcode,
	   opcodes[current_opcode].name);
  }

  return 1;
}



int main(int argc, char* argv[]){
  cpu_registers r = {0};
  context c = {
    .registers = &r,
    .ea = 0,
  };

  flags = read_cmd_line(argc, argv);


  // INIT EXTERNAL CHIPS
  
  chip RAM= (chip){
    .name = "RAM",
    .chip_read = RAM_read,
    .chip_write = RAM_write,
    .chip_init = RAM_init
  };
  chip ROM = (chip){
    .name = "ROM",
    .chip_read = ROM_read,
    .chip_write = ROM_write,
    .chip_init = ROM_init
  };
  chip screen_chip = (chip){
    .name = "screen",
    .chip_read = NULL,
    .chip_write = screen_write,
    .chip_init = NULL
  };
  for(int i = 0; i < 0x8000; i+=256){
    chips[i>>8] = &RAM;
  }
  for(int i = 0x8000; i < 0x10000; i+=256){
    chips[i>>8] = &RAM;
  }
  chips[0x200>>8] = &screen_chip;
  uint8_t *add = (*ROM.chip_init)();  
  (*RAM.chip_init)();
  //END INIT EXTNERNAL CHIPS

  //load file into memory
  if(load_file(add, flags.infile, 32768+1) == 0)
    return 1;
  //print license
  license();
  reset(&c);
  int q = step(&c);
  int max_step = 100;
  int steps = 0;
  while(q > 0 && c.registers->PC < 65535 && steps < max_step){
    q = step(&c);
    steps++;
  }
  if(flags.logging_level > 0){
      printf("-------------- program complete --------------\n");
      print_registers(&c);
  }
}
