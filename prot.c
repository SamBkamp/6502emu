#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "prot.h"

/*

  Dumping grounds for random main helper functions:
  print_registers
  print_stack_addr
  load_file
  read_cmd_line

 */

void license(){
  printf("Copyright (c) 2025 Sam Bonnekamp under the MIT license. Read included License.txt or mit-license.org\n");
}

void print_registers(context *c){
  printf("X: 0x%x\nY: 0x%x\nA: 0x%x\nS: 0x%x\n",
	 c->registers->X, c->registers->Y, c->registers->A, c->registers->S );
  //format flags
  printf("flags:\n N | V | 1 | B | D | I | Z | C |\n");
  for(int i = 7; i >= 0; i--){
    printf(" %d |",
	   (c->registers->P & (1 << i)) >> i);    
  }
  printf("\n");
}
void print_stack_addr(context *c, uint16_t addr){
  if(addr >= STACK_BOTTOM && addr <= STACK_TOP)
    printf("0x%04x : 0x%02x\n", addr, c->RAM[addr]);
}


//im not sure how much overhead logging adds, Im sure we could a --no-logging flag that wraps the printf in an 'if', though I wonder if assinging current_opcode and _pc could be factored out better for that case specifically


//loads file filename into dest
int load_file(void *dest, char* filename, size_t max_size){
  //open infile
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "File I/O Error: Failed to open file %s\n", filename);
    return 0;
  }
  //get and check file size
  fseek(fp, 0L, SEEK_END);  
  int file_len = ftell(fp);
  if(file_len > max_size){
    fprintf(stderr, "File I/O Error: File too large (max %zub)\n", max_size);
    return 0;
  }
  rewind(fp); //reset file cursor  
  //read binary file into memory
  fread(dest, 1, file_len, fp);

  return file_len;
}

cmd_flags read_cmd_line(int argc, char** argv){
  cmd_flags flags;
  //read command line arguments
  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "-f") == 0 && argc >= i+1){
      flags.infile = argv[++i];
    }
  }
  return flags;
}

