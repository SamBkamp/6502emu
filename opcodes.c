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
  c->final_addr = c->ea;
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
  c->final_addr = c->RAM[c->registers->PC+1];
  c->registers->PC += 2; //moves pc forward 2 bytes so it points to next instruction
}
void addr_stack(context *c){ //Stack addressing s (basically the same as implied)
  //returns operand address
  c->registers->PC ++;
}
void addr_pcr(context *c){ //program counter relative r
  //returns new PC value 
  c->ea = (uint16_t)(c->registers->PC+2 + (int8_t)c->RAM[c->registers->PC+1]);
  c->final_addr = c->ea;
  c->registers->PC += 2;
}
void addr_zp(context *c){ //zero page zp
  //returns operand address
  c->ea = (uint16_t)c->RAM[c->registers->PC+1]; //loads the next value
  c->final_addr = c->RAM[c->registers->PC+1];
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
void OP_jsr(context *c){ //jump to subroutine
  //push pc to bottom of stack
  uint8_t hi_byte = (c->registers->PC >> 8);
  uint8_t lo_byte = c->registers->PC;
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = hi_byte;
  c->registers->S--;
  c->RAM[STACK_BOTTOM + (uint16_t)c->registers->S] = lo_byte;
  c->registers->S--;
  
  c->registers->PC = c->ea;
}
void OP_rts(context *c){ //return from subroutine 
  c->ea = get_16_bit_from(STACK_BOTTOM + c->registers->S+1, c);
  c->registers->S +=2;
  
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

/*---- ARITHEMETIC OPERATIONS ------*/
void OP_asl(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->RAM[c->ea] & BIT_7_MASK)>>7; //set c to old bit 7
  c->RAM[c->ea] <<= 1;
  c->registers->P = (c->RAM[c->ea] > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_aslA(context *c){ //hacky implementation for accumulator-mode addressing for ASL
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->registers->A & BIT_7_MASK)>>7; //set c to old bit 7
  c->registers->A <<= 1;
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_lsr(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK) | (c->RAM[c->ea] & BIT_0_MASK); //set c to old bit 0
  c->RAM[c->ea] >>= 1;
  c->registers->P = (c->RAM[c->ea] > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_lsrA(context *c){ //hacky implementation for accumulator-mode addressing for ASL
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK) | (c->registers->A & BIT_0_MASK); //set c to old bit 0
  c->registers->A >>= 1;  
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rol(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->RAM[c->ea] & BIT_7_MASK)>>7; //set c to old bit 7
  uint8_t mask = (c->RAM[c->ea]&0x80)>>7;
  c->RAM[c->ea] <<= 1;
  c->RAM[c->ea] += mask;
  c->registers->P = (c->RAM[c->ea] > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rolA(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->registers->A & BIT_7_MASK)>>7; //set c to old bit 7
  uint8_t mask = (c->registers->A&0x80)>>7;
  c->registers->A <<= 1;
  c->registers->A += mask;
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_ror(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->RAM[c->ea] & BIT_0_MASK); //set c to old bit 0
  uint8_t mask = (c->RAM[c->ea]&0x01)<<7;
  c->RAM[c->ea] >>= 1;
  c->RAM[c->ea] += mask;
  c->registers->P = (c->registers->P & ~FLAGS_N_MASK); 
  c->registers->P |= (c->RAM[c->ea] & BIT_7_MASK); //set N to bit 7
  c->registers->P = (c->RAM[c->ea] > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rorA(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); 
  c->registers->P |= (c->registers->A & BIT_0_MASK); //set c to old bit 7
  uint8_t mask = (c->registers->A&0x01)<<7;
  c->registers->A >>= 1;
  c->registers->A += mask;
  c->registers->P = (c->registers->P & ~FLAGS_N_MASK); 
  c->registers->P |= (c->registers->A & BIT_7_MASK); //set N to bit 7
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_and(context *c){
  c->registers->A &= c->RAM[c->ea];
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P |= (c->registers->A & BIT_7_MASK ) > 0 ? FLAGS_N_MASK : 0; // set negative
}
void OP_bit(context *c){
  uint8_t res = c->registers->A & c->RAM[c->ea];
 c->registers->P = (res > 0) ? c->registers->P & ~FLAGS_Z_MASK
   : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->RAM[c->ea] & BIT_7_MASK); //set N (7th bit) to bit 7 of memory val
  c->registers->P &= (~FLAGS_V_MASK)+(c->RAM[c->ea] & BIT_6_MASK); //set V (6th bit) to bit 6 of memory val
}
void OP_eor(context *c){
  c->registers->A ^= c->RAM[c->ea];
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of memory val
}
void OP_ora(context *c){
  c->registers->A |= c->RAM[c->ea];
  
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_adc(context *c){
  uint8_t M = c->RAM[c->ea];
  uint8_t C = c->registers->P & FLAGS_C_MASK;
  uint16_t res = c->registers->A + M + C; //size promotion to handle overflows (carries)
  c->registers->A = res & 0xFF;
  
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of A
  c->registers->P = res > 0xff ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; //set carry flag if theres an overflow
}
void OP_sbc(context *c){
  uint8_t M = c->RAM[c->ea];
  uint8_t C = c->registers->P & FLAGS_C_MASK;

  //c->registers->A = c->registers->A + ~(M + (1-C))+1; <-- FULL 2s COMPLEMENT VERSION  
  c->registers->A = c->registers->A + (~M) + C; //simplified version

  
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of A

  c->registers->P = c->registers->A >= M ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; //set carry bit if right side operand M is less than A IN UNSINGED COMPARISON (IE when the result of the subtraction is positive or 0)
  //carry bit only cleared when borrowing from the carry bit occured during the subtraction
}

/*-------- COMPARE CALLS --------*/
void OP_cpx(context *c){
  uint8_t ret = c->registers->X - c->RAM[c->ea];
  c->registers->P = (ret & BIT_7_MASK) > 0 ? c->registers->P | FLAGS_N_MASK
    : c->registers->P & ~FLAGS_N_MASK; // set negative if bit 7 set
  
  c->registers->P = c->registers->X >= c->RAM[c->ea] ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if y >= ms
  
  c->registers->P = c->registers->X == c->RAM[c->ea] ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if y = m
}

void OP_cpy(context *c){
  uint8_t ret = c->registers->Y - c->RAM[c->ea];
  c->registers->P = (ret & BIT_7_MASK) > 0 ? c->registers->P | FLAGS_N_MASK
    : c->registers->P & ~FLAGS_N_MASK; // set negative if bit 7 set
  
  c->registers->P = c->registers->Y >= c->RAM[c->ea] ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if y >= ms
  
  c->registers->P = c->registers->Y == c->RAM[c->ea] ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if y = m
}

void OP_cmp(context *c){
  uint8_t ret = c->registers->A - c->RAM[c->ea];
  
  c->registers->P = c->registers->A >= c->RAM[c->ea] ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if a >= m
  
  c->registers->P = c->registers->A == c->RAM[c->ea] ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if a = m

  c->registers->P = c->registers->P & ~FLAGS_N_MASK; //reset n bit
  c->registers->P |= ret & BIT_7_MASK; //set n bit if bit 7 of ret is set
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
void OP_dex(context *c){
  c->registers->X--;
}
void OP_dey(context *c){
  c->registers->Y--;
}
void OP_inx(context *c){
  c->registers->X++;
}
void OP_iny(context *c){
  c->registers->Y++;
}
void OP_inc(context *c){
  c->RAM[c->ea]++;
}
void OP_dec(context *c){
  c->RAM[c->ea]--;
}
void OP_tay(context *c){
  c->registers->Y = c->registers->A;
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->Y == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->Y & BIT_7_MASK; //set Z flag to bit 7 of Y
}
void OP_tya(context *c){
  c->registers->A = c->registers->Y;
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->A == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->A & BIT_7_MASK; //set Z flag to bit 7 of A
}
void OP_txa(context *c){
  c->registers->A = c->registers->X;
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->A == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->A & BIT_7_MASK; //set Z flag to bit 7 of A
}
void OP_tax(context *c){
  c->registers->X = c->registers->A;
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->X == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->X & BIT_7_MASK; //set Z flag to bit 7 of X
}
void OP_tsx(context *c){
  c->registers->X = c->registers->S;
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->X == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->X & BIT_7_MASK; //set Z flag to bit 7 of X
}
void OP_txs(context *c){ //no flags affected
  c->registers->S = c->registers->X;
}

