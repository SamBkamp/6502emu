#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>

#include "registers.h"
#include "pins.h"

uint8_t registers[REG_SIZE]; //all cpu registers
uint8_t *RAM;

void reset(void){
  registers[PC] = RAM[0xFFFC];
  registers[PC+1] = RAM[0xFFFD];
}

void step(void){ //step through instructions
  printf("pc: 0x%02x%02x\n", registers[PC], registers[PC+1]);
}

int main(int argc, char* argv[]){
  
  RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
		     MAP_PRIVATE|MAP_ANON ,-1, 0);

  data_bus[0] = 0xea;
  
  RAM[0xFFFC] = data_bus[0];
  RAM[0xFFFD] = data_bus[0];

  reset();
  step();

  
}
