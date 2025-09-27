#include <stdint.h>
#include <stdio.h>

#include "prot.h"
#include "opcodes.h"

/* addressing moding functions */

/* ---------------------------------------------------------------------------------------
                                   !NOTICE!
   all addr_ functions return the POINTER to the variable they operate on.
   if they operate on immediate values (like eg LDA #$41) a POINTER to the 0x41
   will be returned. If the functions operate on indirect values (0x6c JMP)
   ie. pointers to pointers. They will be given a pointer which points at their
   jump location. ALWAYS FOLLOW THE "ONE-AWAY RULE" (one dereference away from final val).

   ONE EXCEPTION: program counter relative (r) addressing returns the direct address to jump
   to. This is anexception because this addressing mode is only used for branch operations 

   ----------------------------------------------------------------------------------------*/
void addr_implied(context *c){ //implied i
  //doesn't return
  c->registers->PC ++;
}
void addr_accumulator(context *c){ //accumulator A
  //doesn't return
  c->registers->PC ++;
}
void addr_abs_indirect(context *c){ //Absolute Indirect (a)
  //returns pointer to new program counter value
  uint16_t hi_byte = ((uint16_t)c->RAM[c->registers->PC+2]);
  uint16_t lo_byte = (uint16_t)c->RAM[c->registers->PC+1];
  c->ea = (hi_byte<<8)+lo_byte;
  c->registers->PC += 3;
}
void addr_abs(context *c){ //Absolute a
  //returns operand address
  c->ea = c->registers->PC+1;
  c->registers->PC += 3;
}
void addr_imm(context *c){ //immediate #
  //returns operand address
  c->ea = c->registers->PC+1; //loads a pointer to the next value (immediate value) into ea
  c->registers->PC += 2; //moves pc forward 2 bytes so it points to next instruction
}
void addr_stack(context *c){ //Stack addressing s (basically the same as implied)
  //returns operand address
  c->registers->PC ++;
}
void addr_pcr(context *c){ //program counter relative r
  //returns new PC value
  c->ea = c->registers->PC+1 + c->RAM[c->registers->PC+1];
  c->registers->PC += 2;
}
void addr_zp(context *c){ //zero page zp
  //returns operand address
  c->ea = (uint16_t)c->RAM[c->registers->PC+1]; //loads the next value
  c->registers->PC += 2;
}
void addr_zp_indirect(context *c){ //zero page indirect zp
  //returns operand address
  uint16_t hi_byte = c->RAM[c->RAM[c->registers->PC+1]+1];
  uint16_t lo_byte = c->RAM[c->RAM[c->registers->PC+1]];
  c->ea = (hi_byte << 8) + lo_byte;
  printf("ea 0x%x\n", c->ea);
  c->registers->PC += 2;
}



/* ------------- opcode implementations -------------*/
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
void OP_jmp(context *c){ //unconditional jump to EA
  c->registers->PC = (((uint16_t)c->RAM[c->ea+1]) << 8) + c->RAM[c->ea];
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

/*-------- STORE CALLS --------*/
void OP_sta(context *c){
  c->RAM[c->ea] = c->registers->A;
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
