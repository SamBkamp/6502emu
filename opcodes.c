#include <stdint.h>
#include <stdio.h>

#include "prot.h"
#include "opcodes.h"

/* addressing moding functions */
void addr_implied(context *c){ //implied i
  c->registers->PC ++;
}
void addr_accumulator(context *c){ //accumulator A
}
void addr_imm(context *c){ //immediate #
  c->ea = c->registers->PC+1; //loads the next value (immediate value) into ea
  c->registers->PC += 2; //moves pc forward 2 bytes so it points to next instruction

}
void addr_stack(context *c){ //Stack addressing s
  c->ea = (STACK_START + (uint16_t)c->registers->S) + (uint16_t)c->registers->PC+1;
  c->registers->PC += 2;
}
void addr_pcr(context *c){ //program counter relative r
  c->ea = c->registers->PC+1 + c->RAM[c->registers->PC+1];
  c->registers->PC += 2;
}

/* opcode implementations */
/*-------- BRANCHING CALLS --------*/
void OP_bcc(context *c){ //branch carry clear
  if((c->registers->P & FLAGS_C_MASK) == 0){
    c->registers->PC = c->ea;
  }
}
void OP_bcs(context *c){ //branch carry set
  if((c->registers->P & FLAGS_C_MASK) != 0){
    c->registers->PC = c->ea;
  }
}
void OP_beq(context *c){ //branch if z set (branch if equal)
  if((c->registers->P & FLAGS_Z_MASK) != 0){
    c->registers->PC = c->ea;
  }

}
void OP_bmi(context *c){ //branch if n set
  if((c->registers->P & FLAGS_N_MASK) != 0){
    c->registers->PC = c->ea;
  }
}
void OP_bne(context *c){ //branch not equal (if z isn't set)
  if((c->registers->P & FLAGS_Z_MASK) == 0){
    c->registers->PC = c->ea;
  }
}
void OP_bpl(context *c){ //branch if n not set
  if((c->registers->P & FLAGS_N_MASK) == 0){
    c->registers->PC = c->ea;
  }
}
void OP_bra(context *c){ //branch always
  c->registers->PC = c->ea;    
}
void OP_bvs(context *c){ //branch if v = 1
  if((c->registers->P & FLAGS_V_MASK) != 0){
    c->registers->PC = c->ea;
  }
}
void OP_bvc(context *c){ //branch if v = 0
  if((c->registers->P & FLAGS_V_MASK) == 0){
    c->registers->PC = c->ea;
  }
}

/*-------- STATUS FLAG CHANGES --------*/
void OP_clc(context *c){ //clear carry flag
  c->registers->P &= ~FLAGS_C_MASK;
}
void OP_cld(context *c){ //clear decimal mode flag
  c->registers->P &= ~FLAGS_D_MASK;
}
void OP_cli(context *c){ //clear interrupt flag
  c->registers->P &= ~FLAGS_I_MASK;
}
void OP_clv(context *c){ //clear overflow flag
  c->registers->P &= ~FLAGS_V_MASK;
}
void OP_sec(context *c){ //set carry flag
  c->registers->P |= FLAGS_C_MASK;
}
void OP_sed(context *c){ //set decimal mode flag
  c->registers->P |= FLAGS_D_MASK;
}
void OP_sei(context *c){ //set interrupt flag
  c->registers->P |= FLAGS_I_MASK;
}


/*-------- LOAD CALLS --------*/
void OP_ldx(context *c){
  c->registers->X = c->RAM[c->ea];
}

/*-------- MISC CALLS --------*/
void OP_nop(context *c){
  //printf("hello from nop!\n");
  return;
}
