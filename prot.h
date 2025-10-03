#ifndef PROT_6502
#define PROT_6520
//prototypes for global functions and variables

#define FLAGS_C_MASK 0x1
#define FLAGS_Z_MASK 0x2
#define FLAGS_I_MASK 0x4
#define FLAGS_D_MASK 0x8
#define FLAGS_B_MASK 0x10
#define FLAGS_V_MASK 0x40
#define FLAGS_N_MASK 0x80

#define BIT_0_MASK 0x1
#define BIT_1_MASK 0x2
#define BIT_2_MASK 0x4
#define BIT_3_MASK 0x8
#define BIT_4_MASK 0x10
#define BIT_5_MASK 0x20
#define BIT_6_MASK 0x40
#define BIT_7_MASK 0x80


#define STACK_BOTTOM 0x0100
#define STACK_TOP 0x01ff

#define IRQB_VEC 0xFFFE //low byte
#define RESB_VEC 0xFFFC //low byte

typedef struct{
  char* infile;
}cmd_flags;

typedef struct{
  uint8_t A; //accumulator
  uint8_t Y; //index Y
  uint8_t X; //index X
  uint16_t PC; //program counter
  uint8_t S; //Stack pointer
  uint8_t P; //flags
}cpu_registers;

typedef struct{
  cpu_registers *registers;
  uint16_t ea; //effective address
  uint8_t *RAM; //ram address
  uint16_t final_addr; //for logging
}context;


typedef struct{
  void (*func)(context *c);
  char* name;
  void (*addr_mode)(context *c);
}opcode;


/*

  flags register:
  | N | V | 1 | B | D | I | Z | C |
  ^MSB                         LSB^

  N = negative (1 = neg)
  V = Overflow (1 = true)
  1 = undef.
  B = BRK command (1 = BRK, 0 = IRQB)
  D = Decimal mode (1 = true)
  I = IRQB disable (1 = disable)
  Z = zero (1 = true)
  C = carry (1 = true)

  source: https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf
 */

/*

  Dumping grounds for random main helper functions:
  print_registers
  print_stack_addr
  reset
  step
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

#endif
