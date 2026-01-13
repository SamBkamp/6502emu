#include <stdint.h>
#include <stdio.h>

#define RW_MASK 0x40
#define RS_MASK 0x20
#define E_MASK 0x80

#define PORTB 0x6000
#define PORTA 0x6001

FILE *outfile;

uint8_t last_print = 0; //implementation detail for logging file, no relation to actual chip

//abstraction for the via
uint8_t portb_latch;

//its simple and fast enough for foss work
uint8_t get_msb(uint8_t instruction){
  uint8_t msb_pos = 0;

  while(instruction >>= 1)
    msb_pos++;

  return msb_pos;
}


uint8_t *screen_init(){
  outfile = fopen("screenout.txt", "w");
  if(!outfile) return NULL;
  return (uint8_t *)outfile;
}

void screen_write(uint16_t address, uint8_t data){
  //uint8_t lo_byte = (address & 0xff);
  //uint8_t hi_byte = (address & 0xff00)>>8;

  //latch data if sent to port b
  if(address == PORTB){
    portb_latch = data;
    return;
  }

  //data sent to port a and and enable off
  if(address == PORTA && (data & E_MASK) == 0) return;

  //data sent to port a and enable on

  //write to screen
  if((data & RS_MASK) > 0
     && (data & RW_MASK) == 0
     && (data & E_MASK) > 0){
    fprintf(outfile, "%c", portb_latch);
    last_print = '\n';
    return;
  }

  //rs and rw low, lcd instruction
  if((data & RS_MASK) == 0
     && (data & RW_MASK) == 0
     && (data & E_MASK) > 0){
    //decode instruction
    uint8_t msb_pos = get_msb(portb_latch);
    switch(msb_pos){
    case 0:
      fprintf(outfile, "%c--display cleared--\n", last_print);
      break;
    case 1:
      fprintf(outfile, "%c--returned home--\n", last_print);
      break;
    case 2:
      fprintf(outfile, "%c--entry mode set--\n", last_print);
      break;
    case 3:
      fprintf(outfile, "%c--Display on/off control--\n", last_print);
      break;
    case 4:
      fprintf(outfile, "%c--cursor or display shift--\n", last_print);
      break;
    case 5:
      fprintf(outfile, "%c--function set--\n", last_print);
      break;
    case 6:
      fprintf(outfile, "%c--set CGRAM address--\n", last_print);
      break;
    case 7:
      fprintf(outfile, "%c--set DDRAM address--\n", last_print);
      break;
    default:
      fprintf(outfile, "%c--unknown instruction 0x%x--", last_print, portb_latch);
      break;
    }    
    return;
  }
}

uint8_t screen_read(uint16_t address){
  return 0; //not busy always
}
