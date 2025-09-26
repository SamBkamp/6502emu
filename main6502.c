#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "registers.h"
#include "pins.h"
#include "opcodes.h"

uint8_t registers[REG_SIZE]; //all cpu registers
uint8_t *RAM;

void print_registers(){
  printf("X: %x\nY: %x\n", registers[X], registers[Y]);
}

void fetch_data(void){
  data_bus = RAM[address_bus];
}


void reset(void){
  address_bus = 0xFFFC;
  fetch_data();
  registers[PC+1] = data_bus;
  
  address_bus = 0xFFFD;
  fetch_data();
  registers[PC] = data_bus;
}

void step(void){ //step through instructions
  print_registers();
  address_bus = ((uint16_t)registers[PC] << 8) + registers[PC+1];
  fetch_data();  
  printf("0x%04X : 0x%02X %s\n", address_bus, data_bus, opcodes_string[data_bus]);

  registers[PC+1] += 1;
}

void load_into_pc(uint16_t v){
  registers[PC] = (v >> 8) & 0xff;
  registers[PC+1] = v & 0xff;
}

void load_uint16_into_ram(uint16_t addr, uint16_t val){
  RAM[addr] = (val >> 8) & 0xff;
  RAM[addr+1] = val & 0xff;
}

int main(int argc, char* argv[]){
  
  RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  
  memset(RAM, 0xea, 65535); //init ram with only 0xea 

  load_uint16_into_ram(0xFFFC, 0x0080);

  RAM[0x8000] = 0xa2;
  
  reset();
  step();  
  step();
  step();
  step();

  
}
