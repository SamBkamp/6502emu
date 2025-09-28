#include <stdint.h>
#include <stdio.h>

#include "prot.h"
#include "opcodes.h"

//helper function to return the 16 bit value stored in little endian. loc points to the lower byte
uint16_t get_16_bit_from(uint16_t loc, context *c){
  uint16_t hi_byte = c->RAM[loc+1];
  uint16_t lo_byte = c->RAM[loc];
  hi_byte <<= 8;
  hi_byte &= 0xff00; //probably unneeded
  hi_byte += lo_byte;
  return hi_byte;
}

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
  c->ea = get_16_bit_from(get_16_bit_from(c->registers->PC+1, c), c);
  c->registers->PC += 3;
}
void addr_abs_idx_indirect(context *c){ //Absolute idexed Indirect (a, x)
  c->ea = get_16_bit_from(c->registers->PC+1, c) + c->registers->X; //indirect address
  c->ea = get_16_bit_from(c->ea, c); 
  c->registers->PC += 3;
}
void addr_abs(context *c){ //Absolute a
  //returns operand address
  c->ea = get_16_bit_from(c->registers->PC+1, c);
  c->registers->PC += 3;
}
void addr_abs_x(context *c){ //Asbolute index with X a,x
  c->ea = get_16_bit_from(c->registers->PC+1, c);
  c->ea += c->registers->X;
  c->registers->PC += 3;  
}
void addr_abs_y(context *c){ //Asbolute index with Y a,y
  c->ea = get_16_bit_from(c->registers->PC+1, c);
  c->ea += c->registers->Y;
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
void addr_zp_idx_x(context *c){ //zp, x
  //returns operand address
  c->ea = c->RAM[(c->registers->PC+1)] + c->registers->X;
  c->registers->PC += 2;
}
void addr_zp_idx_y(context *c){ //zp, y
  //returns operand address
  c->ea = c->RAM[(c->registers->PC+1)] + c->registers->Y;
  c->registers->PC += 2;
}
void addr_zp_indirect(context *c){ //zero page indirect (zp)
  //returns operand address
  uint16_t ptr = c->RAM[c->registers->PC+1];
  c->ea = get_16_bit_from(ptr, c);
  c->registers->PC += 2;
}
void addr_zp_idx_indirect(context *c){ //zero page indexed indirect (zp, x)
  uint16_t ptr = c->RAM[c->registers->PC+1] + c->registers->X;
  c->ea = get_16_bit_from(ptr, c);
  c->registers->PC += 2;
}
void addr_zp_idx_y_indirect(context *c){ //Zero Page Indirect Indexed with Y (zp), y
  uint16_t ptr = c->RAM[c->registers->PC+1];
  c->ea = get_16_bit_from(ptr, c);
  c->ea += c->registers->Y;
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
  c->registers->PC = c->ea;
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
void OP_stx(context *c){
  c->RAM[c->ea] = c->registers->X;
}
void OP_sty(context *c){
  c->RAM[c->ea] = c->registers->Y;
}
void OP_stz(context *c){
  c->RAM[c->ea] = 0;
}

/*-------- LOAD CALLS --------*/
void OP_ldx(context *c){
  c->registers->X = c->RAM[c->ea];
}
void OP_lda(context *c){
  c->registers->A = c->RAM[c->ea];
}
void OP_ldy(context *c){
  c->registers->Y = c->RAM[c->ea];
}

/*-------- MISC CALLS --------*/
void OP_nop(context *c){
  //printf("hello from nop!\n");
  return;
}
