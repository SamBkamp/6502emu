#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "registers.h"
#include "pins.h"

uint8_t registers[REG_SIZE]; //all cpu registers
uint8_t *RAM;

void fetch_data(void){
  data_bus = RAM[address_bus];
}


void reset(void){
  address_bus = 0xFFFC;
  fetch_data();
  registers[PC] = data_bus;
  address_bus = 0xFFFC;
  fetch_data();
  registers[PC+1] = data_bus;
}

void step(void){ //step through instructions
  address_bus = ((uint16_t)registers[PC] << 8) + registers[PC+1];
  fetch_data();
  
  printf("0x%04x : %p\n", address_bus, data_bus);
  registers[PC+1] += 1;
}

void load_into_pc(uint16_t v){
  registers[PC] = (v >> 8) & 0xff;
  registers[PC+1] = v & 0xff;
}

int main(int argc, char* argv[]){
  
  RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  
  memset(RAM, 0xea, 65535); //init ram with only 0xea 
   
  reset();
  step();  
  step();
  step();
  step();

  
}
