#include <stdint.h>
#include <stdio.h>


void screen_write(uint16_t address, uint8_t data){
  putchar(data);
}
