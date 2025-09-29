#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"

typedef struct{
  char *infile;
}cmd_flags;

//TODO: move this somewhere else its so ugly
opcode opcodes[] = {
  {NULL, "brk", NULL}, {NULL, "ora", NULL}, {OP_nop, "nop1", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop2", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {OP_php, "php", addr_stack}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {OP_nop, "nop3", addr_implied}, {OP_nop, "nop4", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {OP_bpl, "bpl", addr_pcr}, {NULL, "ora", NULL}, {OP_nop, "nop5", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop6", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, {OP_clc, "clc", addr_implied}, {NULL, "ora", NULL}, {OP_nop, "nop7", addr_implied}, {NULL, "slo", NULL}, {OP_nop, "nop8", addr_implied}, {NULL, "ora", NULL}, {NULL, "asl", NULL}, {NULL, "slo", NULL}, 
  {NULL, "jsr", NULL}, {NULL, "and", NULL}, {OP_nop, "nop9", addr_implied}, {NULL, "rla", NULL}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {OP_plp, "plp", addr_stack}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {OP_nop, "nop10", addr_implied}, {NULL, "bit", NULL}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {OP_bmi, "bmi", addr_pcr}, {NULL, "and", NULL}, {OP_nop, "nop11", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop12", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, {OP_sec, "sec", addr_implied}, {NULL, "and", NULL}, {OP_nop, "nop13", addr_implied}, {NULL, "rla", NULL}, {OP_nop, "nop14", addr_implied}, {NULL, "and", NULL}, {NULL, "rol", NULL}, {NULL, "rla", NULL}, 
  {NULL, "rti", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop15", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop16", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {OP_pha, "pha", addr_stack}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {OP_nop, "nop17", addr_implied}, {OP_jmp, "jmp", addr_abs}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "bvc", NULL}, {NULL, "eor", NULL}, {OP_nop, "nop18", addr_implied}, {NULL, "sre", NULL}, {OP_nop, "nop19", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, {NULL, "cli", NULL}, {NULL, "eor", NULL}, {OP_phx, "phx", addr_stack}, {NULL, "sre", NULL}, {OP_nop, "nop21", addr_implied}, {NULL, "eor", NULL}, {NULL, "lsr", NULL}, {NULL, "sre", NULL}, 
  {NULL, "rts", NULL}, {NULL, "adc", NULL}, {OP_nop, "nop22", addr_implied}, {NULL, "rra", NULL}, {OP_stz, "stz", addr_zp}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {NULL, "pla", NULL}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {OP_nop, "nop24", addr_implied}, {OP_jmp, "jmp", addr_abs_indirect}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {OP_bvs, "bvs", addr_pcr}, {NULL, "adc", NULL}, {OP_nop, "nop25", addr_implied}, {NULL, "rra", NULL}, {OP_stz, "stz", addr_zp_idx_x}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, {OP_sei, "sei", addr_implied}, {NULL, "adc", NULL}, {OP_ply, "ply", addr_stack}, {NULL, "rra", NULL}, {OP_jmp, "jmp", addr_abs_idx_indirect}, {NULL, "adc", NULL}, {NULL, "ror", NULL}, {NULL, "rra", NULL}, 
  {OP_bra, "bra", addr_pcr}, {OP_sta, "sta", addr_zp_idx_indirect}, {OP_nop, "nop29", addr_implied}, {NULL, "sax", NULL}, {OP_sty, "sty", addr_zp}, {OP_sta, "sta", addr_zp}, {OP_stx, "stx", addr_zp}, {NULL, "sax", NULL}, {NULL, "dey", NULL}, {OP_nop, "nop30", addr_implied}, {NULL, "txa", NULL}, {OP_nop, "nop31", addr_implied}, {OP_sty, "sty", addr_abs}, {OP_sta, "sta", addr_abs}, {OP_stx, "stx", addr_abs}, {NULL, "sax", NULL}, 
  {OP_bcc, "bcc", addr_pcr}, {OP_sta, "sta", addr_zp_idx_y_indirect}, {OP_sta, "sta", addr_zp_indirect}, {OP_nop, "nop33", addr_implied}, {OP_sty, "sty", addr_zp_idx_x}, {OP_sta, "sta", addr_zp_idx_x}, {OP_stx, "stx", addr_zp_idx_y}, {NULL, "sax", NULL}, {NULL, "tya", NULL}, {OP_sta, "sta", addr_abs_y}, {NULL, "txs", NULL}, {OP_nop, "nop34", addr_implied}, {OP_stz, "stz", addr_abs}, {OP_sta, "sta", addr_abs_x}, {OP_stz, "stz", addr_abs_x}, {OP_nop, "nop37", addr_implied}, 
  {OP_ldy, "ldy", addr_imm}, {OP_lda, "lda", addr_zp_idx_indirect}, {OP_ldx, "ldx", addr_imm}, {NULL, "lax", NULL}, {OP_ldy, "ldy", addr_zp}, {OP_lda, "lda", addr_zp}, {OP_ldx, "ldx", addr_zp}, {NULL, "lax", NULL}, {NULL, "tay", NULL}, {OP_lda, "lda", addr_imm}, {NULL, "tax", NULL}, {OP_nop, "nop38", addr_implied}, {OP_ldy, "ldy", addr_abs}, {OP_lda, "lda", addr_abs}, {OP_ldx, "ldx", addr_abs}, {NULL, "lax5", NULL}, 
  {OP_bcs, "bcs", addr_pcr}, {OP_lda, "lda", addr_zp_idx_y_indirect}, {OP_lda, "lda", addr_zp_indirect}, {NULL, "lax2", NULL}, {OP_ldy, "ldy", addr_zp_idx_x}, {OP_lda, "lda", addr_zp_idx_x}, {OP_ldx, "ldx", addr_zp_idx_y}, {NULL, "lax4", NULL}, {NULL, "clv", NULL}, {OP_lda, "lda", addr_abs_y}, {NULL, "tsx", NULL}, {NULL, "lax1", NULL}, {OP_ldy, "ldy", addr_abs_x}, {OP_lda, "lda", addr_abs_x}, {OP_ldx, "ldx", addr_abs_y}, {NULL, "lax3", NULL}, 
  {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {OP_nop, "nop40", addr_implied}, {NULL, "dcp", NULL}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "iny", NULL}, {NULL, "cmp", NULL}, {NULL, "dex", NULL}, {OP_nop, "nop41", addr_implied}, {NULL, "cpy", NULL}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {OP_bne, "bne", addr_pcr}, {NULL, "cmp", NULL}, {OP_nop, "nop42", addr_implied}, {NULL, "dcp", NULL}, {OP_nop, "nop43", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, {NULL, "cld", NULL}, {NULL, "cmp", NULL}, {OP_phx, "phx", addr_stack}, {NULL, "dcp", NULL}, {OP_nop, "nop45", addr_implied}, {NULL, "cmp", NULL}, {NULL, "dec", NULL}, {NULL, "dcp", NULL}, 
  {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop46", addr_implied}, {NULL, "isb", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {NULL, "inx", NULL}, {NULL, "sbc", NULL}, {OP_nop, "nop47", addr_implied}, {NULL, "sbc", NULL}, {NULL, "cpx", NULL}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, 
  {OP_beq, "beq", addr_pcr}, {NULL, "sbc", NULL}, {OP_nop, "nop48", addr_implied}, {NULL, "isb", NULL}, {OP_nop, "nop49", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}, {OP_sed, "sed", addr_implied}, {NULL, "sbc", NULL}, {OP_plx, "plx", addr_stack}, {NULL, "isb", NULL}, {OP_nop, "nop51", addr_implied}, {NULL, "sbc", NULL}, {NULL, "inc", NULL}, {NULL, "isb", NULL}
};


void print_registers(context *c){
  printf("X: %x\nY: %x\nA: %x\nflags: %x\n",
	 c->registers->X, c->registers->Y, c->registers->A, c->registers->P);
}
void print_stack_addr(context *c, uint16_t addr){
  if(addr >= STACK_BOTTOM && addr <= STACK_TOP)
    printf("0x%04x : 0x%02x\n", addr, c->RAM[addr]);
}

void reset(context *c){
  c->registers->PC = ((uint16_t)c->RAM[0xFFFC]) << 8;
  c->registers->PC += c->RAM[0xFFFD];
  c->registers->S = 0xFF;
}

int step(context *c){ //step through instructions returns negative value when 0xbb is encountered
  uint16_t current_opcode = c->RAM[c->registers->PC];

  if(current_opcode == 0xbb) return -1;
  
  printf("0x%04X : 0x%02X %s\n", c->registers->PC, current_opcode, opcodes[current_opcode].name);  
  
  if(opcodes[current_opcode].func != NULL){
    (*opcodes[current_opcode].addr_mode)(c); //set addressing mode
    (*opcodes[current_opcode].func)(c); //call function associated with opcode
  }
  return 1;
}


int main(int argc, char* argv[]){
  size_t mount_point = 0x8000;
  size_t memory_size = 0xffff; 
  cmd_flags flags;
  cpu_registers r = {0};
  context c = {
    .registers = &r,
    .ea = 0
  };
  
  c.RAM = (uint8_t *)mmap(NULL, 65536, PROT_READ|PROT_WRITE|PROT_EXEC,
			MAP_PRIVATE|MAP_ANON ,-1, 0);
  memset(c.RAM, 0xea, memory_size); //init ram with only 0xea (nop)

  //read command line arguments
  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "-f") == 0 && argc >= i+1){
      flags.infile = argv[++i];
    }
  }
  //check infile
  if(flags.infile == NULL){
    fprintf(stderr, "Usage: %s -f [filename]\n", argv[0]);
    return 1;
  }
  //open infile
  FILE *fptr = fopen(flags.infile, "r");
  if (fptr == NULL) {
    fprintf(stderr, "File I/O Error: Failed to open file %s\n", flags.infile);
    return 1;
  }
  //get and check file size
  fseek(fptr, 0L, SEEK_END);  
  long file_len = ftell(fptr);
  rewind(fptr); //reset file cursor
  if(file_len >= (memory_size-mount_point)){
    fprintf(stderr, "File I/O Error: File too large (max 65kb)\n");
    return 1;
  }
  //read binary file into memory
  fread(&c.RAM[mount_point], file_len, 1, fptr);
  
  c.RAM[0xFFFC] = 0x80;
  c.RAM[0xFFFD] = 0x00;

  
  printf("-------------- program start --------------\n");
  reset(&c);
  int q = step(&c);
  while(q > 0 && c.registers->PC < memory_size){
    q = step(&c);
  }
  printf("-------------- program complete --------------\n");
  printf("0x%04x : 0x%02x\n", 0xff, c.RAM[0xff]);
  //print_stack_addr(&c, STACK_TOP);
  print_registers(&c);
}
