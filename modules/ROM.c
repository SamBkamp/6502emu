#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ROM.h"

uint8_t *ROM_address_space;

uint8_t *ROM_init(){
  ROM_address_space = mmap(NULL, 32768, PROT_READ|PROT_WRITE|PROT_EXEC,
		       MAP_PRIVATE|MAP_ANON ,-1, 0);

  //check mmap
  if (ROM_address_space == (void *)-1){
    return (uint8_t *)0;
  }
  return ROM_address_space;
}

uint8_t ROM_read(uint16_t address){
  return ROM_address_space[address];
}

void ROM_write(uint16_t address, uint8_t data){
  ;
}
