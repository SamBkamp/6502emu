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
uint16_t load_file(void *dest, const char *filename, size_t max_size) {
    if (!filename) {
      fprintf(stderr, "File I/O Error: No filename provided.\n");
      return 0;
    }
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File I/O Error: Failed to open file %s\n", filename);
        return 0;
    }

    // get file size
    if (fseek(fp, 0L, SEEK_END) != 0) {
        perror("fseek");
        fclose(fp);
        return 0;
    }

    long len = ftell(fp);
    if (len < 0) {
        perror("ftell");
        fclose(fp);
        return 0;
    }

    if (len > 0x10000) { // larger than 6502 address space
        fprintf(stderr, "File I/O Error: File too large (max 65536b)\n");
        fclose(fp);
        return 0;
    }

    uint16_t file_len = (uint16_t)len;
    if (file_len > max_size) {
        fprintf(stderr, "File I/O Error: File too large (max %zub)\n", max_size);
        fclose(fp);
        return 0;
    }

    rewind(fp);

    size_t bytes_read = fread(dest, 1, file_len, fp);
    fclose(fp);

    if (bytes_read != file_len) {
        fprintf(stderr, "File I/O Error: Short read (%zu/%u)\n", bytes_read, file_len);
        return 0;
    }

    return file_len;
}

cmd_flags read_cmd_line(int argc, char** argv){
  cmd_flags flags = {0}; // in C everything has to be initialized to *something*
                        // otherwise it will contain garbage data until it is filled
                       // with something at runtime.
  flags.logging_level = 3; //def value
  //read command line arguments
  for(int i = 1; i < argc; i++){
    // we dont need to check argv[0] since its always the program name;
    // instead, start at argv[1]
    if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
        flags.infile = argv[++i];
    }
    if(strcmp(argv[i], "-log") == 0 && i + 1< argc){
      //TODO: add boundary checks and/or use lib function
      flags.logging_level = *argv[++i] - 0x30; //very rudimentary way to turn single digit char to int
    }
  }
  return flags;
}

