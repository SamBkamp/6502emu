#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include "RAM.h"

uint8_t *RAM_address_space;

uint8_t *RAM_init(){
  RAM_address_space = mmap(NULL, 32768, PROT_READ|PROT_WRITE|PROT_EXEC,
		       MAP_PRIVATE|MAP_ANON ,-1, 0);

  //check mmap
  if (RAM_address_space == (void *)-1){
    return (uint8_t *)0;
  }
  return RAM_address_space;
}

uint8_t RAM_read(uint16_t address){
  return RAM_address_space[address];
}

void RAM_write(uint16_t address, uint8_t data){  
  RAM_address_space[address] = data;
}
