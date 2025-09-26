#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "registers.h"
#include "pins.h"
#include "opcodes.h"

void print_registers(context *c){
  printf("X: %x\nY: %x\n", c->registers[X], c->registers[Y]);
}

void fetch_data(void){
  data_bus = RAM[address_bus];
}

void reset(context *c){
  address_bus = 0xFFFC;
  fetch_data();
  c->registers[PC+1] = data_bus;
  
  address_bus = 0xFFFD;
  fetch_data();
  c->registers[PC] = data_bus;
}

void step(context *c){ //step through instructions
  address_bus = ((uint16_t)c->registers[PC] << 8) + c->registers[PC+1];
  fetch_data();  
  printf("0x%04X : 0x%02X %s\n", address_bus, data_bus, opcodes[data_bus].name);


  if(opcodes[data_bus].func != NULL){
    (*opcodes[data_bus].addr_mode)(c); //set addressing mode
    (*opcodes[data_bus].func)(c); //call function associated with opcode
  }

  c->registers[PC+1] += 1;
}

void load_into_pc(context *c, uint16_t v){
  c->registers[PC] = (v >> 8) & 0xff;
  c->registers[PC+1] = v & 0xff;
}

void load_uint16_into_ram(uint16_t addr, uint16_t val){
  RAM[addr] = (val >> 8) & 0xff;
  RAM[addr+1] = val & 0xff;
}

int main(int argc, char* argv[]){

  context c = {
    .registers = {0},
    .ea = 0
  };
  
  RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  
  memset(RAM, 0xea, 65535); //init ram with only 0xea 

  load_uint16_into_ram(0xFFFC, 0x0080);

  RAM[0x8000] = 0xa2;
  RAM[0x8001] = 0x41;
  
  reset(&c);
  step(&c);  
  step(&c);
  step(&c);
  step(&c);
  print_registers(&c);
  
}
