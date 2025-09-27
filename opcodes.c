#include <stdint.h>
#include <stdio.h>

#include "prot.h"
#include "opcodes.h"

/* addressing moding functions */
void addr_implied(context *c){ //implied i
  c->registers->PC ++;
}
void addr_accumulator(context *c){ //accumulator A
  c->registers->PC ++;
}
void addr_imm(context *c){ //immediate #
  c->ea = c->registers->PC+1; //loads a pointer to the next value (immediate value) into ea
  c->registers->PC += 2; //moves pc forward 2 bytes so it points to next instruction

}
void addr_stack(context *c){ //Stack addressing s (basically the same as implied)
  c->registers->PC ++;
}
void addr_pcr(context *c){ //program counter relative r
  c->ea = c->registers->PC+1 + c->RAM[c->registers->PC+1];
  c->registers->PC += 2;
}
void addr_zp(context *c){ //zero page zp
  c->ea = (uint16_t)c->RAM[c->registers->PC+1]; //loads the next value
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

/* ----------- STACK CALLS ----------- */
void OP_pha(context *c){ //push A to stack
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = c->registers->A;
  c->registers->S --;
}
void OP_php(context *c){ //push P to stack
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = c->registers->P;
  c->registers->S --;
}
void OP_phx(context *c){ //push X to stack
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = c->registers->X;
  c->registers->S --;
}
void OP_phy(context *c){ //push Y to stack
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = c->registers->Y;
  c->registers->S --;
}
void OP_plp(context *c){ //pull P from stack
  c->registers->S ++;
  c->registers->P = c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S];
}
void OP_plx(context *c){ //pull X from stack
  c->registers->S ++;
  c->registers->X = c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S];
}
void OP_ply(context *c){ //pull Y from stack
  c->registers->S ++;
  c->registers->Y = c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S];
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
