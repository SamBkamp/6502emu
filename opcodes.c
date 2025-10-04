#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "prot.h"
#include "opcodes.h"

void bus_write(uint16_t address, uint8_t data, context *c){
  c->RAM[address] = data;
}
uint8_t bus_read(uint16_t address, context *c){
  return c->RAM[address];
}

//helper function to return the 16 bit value stored in little endian. loc points to the lower byte
uint16_t get_16_bit_from(uint16_t loc, context *c){
  uint16_t hi_byte = bus_read(loc+1, c);
  uint16_t lo_byte = bus_read(loc, c);
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
  c->final_addr = bus_read(c->registers->PC+1, c);
  c->registers->PC += 2; //moves pc forward 2 bytes so it points to next instruction
}
void addr_stack(context *c){ //Stack addressing s (basically the same as implied)
  //returns operand address
  c->registers->PC ++;
}
void addr_pcr(context *c){ //program counter relative r
  //returns new PC value 
  c->ea = (uint16_t)(c->registers->PC+2 + (int8_t)bus_read(c->registers->PC+1, c));
  c->final_addr = c->ea;
  c->registers->PC += 2;
}
void addr_zp(context *c){ //zero page zp
  //returns operand address
  c->ea = (uint16_t)bus_read(c->registers->PC+1, c); //loads the next value
  c->final_addr = bus_read(c->registers->PC+1, c);
  c->registers->PC += 2;  
}
void addr_zp_idx_x(context *c){ //zp, x
  //returns operand address
  c->ea = bus_read(c->registers->PC+1, c) + c->registers->X;
  c->registers->PC += 2;
}
void addr_zp_idx_y(context *c){ //zp, y
  //returns operand address
  c->ea = bus_read(c->registers->PC+1, c) + c->registers->Y;
  c->registers->PC += 2;
}
void addr_zp_indirect(context *c){ //zero page indirect (zp)
  //returns operand address
  uint16_t ptr = bus_read(c->registers->PC+1, c);
  c->ea = get_16_bit_from(ptr, c);
  c->registers->PC += 2;
}
void addr_zp_idx_indirect(context *c){ //zero page indexed indirect (zp, x)
  uint16_t ptr = bus_read(c->registers->PC+1, c) + c->registers->X;
  c->ea = get_16_bit_from(ptr, c);
  c->registers->PC += 2;
}
void addr_zp_idx_y_indirect(context *c){ //Zero Page Indirect Indexed with Y (zp), y
  uint16_t ptr = bus_read(c->registers->PC+1, c);
  c->ea = get_16_bit_from(ptr, c);
  c->ea += c->registers->Y;
  c->registers->PC += 2; 
}



/* ------------- opcode implementations -------------*/

/*

  if youre working on the todo:

  - [ ] helper functions for common flag setting (set n flag to b7 of operand/register & set z flag)

  Then you can use this pattern if you want to regex replace it or something:
  ```
  c->registers->P = (OPERAND > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(OPERAND & BIT_7_MASK); //set N (7th bit) to bit 7 of A
  ```
 */

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
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, hi_byte, c);
  c->registers->S--;
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, lo_byte, c);
  c->registers->S--;
  
  c->registers->PC = c->ea;
}
void OP_rts(context *c){ //return from subroutine 
  c->ea = get_16_bit_from(STACK_BOTTOM + c->registers->S+1, c);
  c->registers->S +=2;
  
  c->registers->PC = c->ea;
}


/* ----------- STACK CALLS ----------- */
void OP_pha(context *c){ //push A to stack (registers unaffected)
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, c->registers->A, c);
  c->registers->S --;
}
void OP_php(context *c){ //push P to stack (registers unaffected)
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, c->registers->P, c);
  c->registers->S --;
}
void OP_phx(context *c){ //push X to stack (registers unaffected)
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, c->registers->X, c);
  c->registers->S --;
}
void OP_phy(context *c){ //push Y to stack (registers unaffected)
  bus_write(STACK_BOTTOM + (uint16_t)c->registers->S, c->registers->Y, c);
  c->registers->S --;
}
void OP_plp(context *c){ //pull P from stack (registers affected through pull)
  c->registers->S ++;
  c->registers->P = bus_read(STACK_BOTTOM + (uint16_t)c->registers->S, c);
}
void OP_plx(context *c){ //pull X from stack
  c->registers->S ++;
  c->registers->X = bus_read(STACK_BOTTOM + (uint16_t)c->registers->S, c);
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->X == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->X & BIT_7_MASK; //set Z flag to bit 7 of X
}
void OP_ply(context *c){ //pull Y from stack
  c->registers->S ++;
  c->registers->Y = bus_read(STACK_BOTTOM + (uint16_t)c->registers->S, c);
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->Y == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->Y & BIT_7_MASK; //set Z flag to bit 7 of X
}
void OP_pla(context *c){
  c->registers->S ++;
  c->registers->A = bus_read(STACK_BOTTOM + (uint16_t)c->registers->S, c);
  c->registers->P &= ~FLAGS_Z_MASK; //clear Z flag
  c->registers->P |= c->registers->A == 0 ? FLAGS_Z_MASK : 0; //set zero flag if its 0
  c->registers->P &= ~FLAGS_N_MASK; //clear N flag
  c->registers->P |= c->registers->A & BIT_7_MASK; //set Z flag to bit 7 of X
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
  bus_write(c->ea, c->registers->A, c);
}
void OP_stx(context *c){
  bus_write(c->ea, c->registers->X, c);
}
void OP_sty(context *c){
  bus_write(c->ea, c->registers->Y, c);
}
void OP_stz(context *c){
  bus_write(c->ea, 0, c);
}

/*---- ARITHEMETIC OPERATIONS ------*/
void OP_asl(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK);  //clear old C
  c->registers->P |= (bus_read(c->ea, c) & BIT_7_MASK)>>7; //set c to old bit 7
  c->RAM[c->ea] <<= 1; //do the shift left
  c->registers->P = (bus_read(c->ea, c) > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_aslA(context *c){ //hacky implementation for accumulator-mode addressing for ASL
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); //clear C flag
  c->registers->P |= (c->registers->A & BIT_7_MASK)>>7; //set c to old bit 7
  c->registers->A <<= 1; //do the shift left
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_lsr(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK) | (bus_read(c->ea, c) & BIT_0_MASK); //set c to old bit 0
  bus_write(c->ea, bus_read(c->ea, c)>>1,c); //do the shift right TODO: factor function call out
  c->registers->P = (bus_read(c->ea, c) > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_lsrA(context *c){ //hacky implementation for accumulator-mode addressing for ASL
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK) | (c->registers->A & BIT_0_MASK); //set c to old bit 0
  c->registers->A >>= 1;  //do the shift right
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rol(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); //clear old C
  c->registers->P |= (bus_read(c->ea, c) & BIT_7_MASK)>>7; //set c to old bit 7
  uint8_t mask = (bus_read(c->ea, c)&0x80)>>7;
  c->RAM[c->ea] <<= 1;
  c->RAM[c->ea] += mask;
  c->registers->P = (bus_read(c->ea, c) > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rolA(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); //clear old C
  c->registers->P |= (c->registers->A & BIT_7_MASK)>>7; //set c to old bit 7
  uint8_t mask = (c->registers->A&0x80)>>7;
  c->registers->A <<= 1;
  c->registers->A += mask;
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_ror(context *c){
  uint8_t d = bus_read(c->ea, c); //data to shift
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); //clear old C
  c->registers->P |= (d & BIT_0_MASK); //set c to old bit 0
  uint8_t mask = (d&0x01)<<7; //store old bit 7 and move to 0
  c->RAM[c->ea] >>= 1; //shift right
  c->RAM[c->ea] += mask; //add old bit 7 back
  c->registers->P = (c->registers->P & ~FLAGS_N_MASK); //clear old N
  c->registers->P |= (d & BIT_7_MASK); //set N to bit 7
  c->registers->P = (d > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_rorA(context *c){
  c->registers->P = (c->registers->P & ~FLAGS_C_MASK); //clear old c
  c->registers->P |= (c->registers->A & BIT_0_MASK); //set c to old bit 7
  uint8_t mask = (c->registers->A&0x01)<<7; //store old bit 7 and move to 0th
  c->registers->A >>= 1; //shift
  c->registers->A += mask; //add old bit 7 back
  c->registers->P = (c->registers->P & ~FLAGS_N_MASK); //clear old N
  c->registers->P |= (c->registers->A & BIT_7_MASK); //set N to bit 7
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
}
void OP_and(context *c){
  c->registers->A &= bus_read(c->ea, c);
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P = (c->registers->A & BIT_7_MASK ) > 0 ? c->registers->P & FLAGS_N_MASK : c->registers->P | ~FLAGS_N_MASK; // set negative
}
void OP_bit(context *c){
  uint8_t res = c->registers->A & bus_read(c->ea, c);
 c->registers->P = (res > 0) ? c->registers->P & ~FLAGS_Z_MASK
   : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(bus_read(c->ea, c) & BIT_7_MASK); //set N (7th bit) to bit 7 of memory val
  c->registers->P &= (~FLAGS_V_MASK)+(bus_read(c->ea, c) & BIT_6_MASK); //set V (6th bit) to bit 6 of memory val
}
void OP_eor(context *c){
  c->registers->A ^= bus_read(c->ea, c);
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of memory val
}
void OP_ora(context *c){
  c->registers->A |= bus_read(c->ea, c);
  
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_adc(context *c){
  uint8_t M = bus_read(c->ea, c);
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
  uint8_t M = bus_read(c->ea, c);
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
  uint8_t ret = c->registers->X - bus_read(c->ea, c);
  c->registers->P = (ret & BIT_7_MASK) > 0 ? c->registers->P | FLAGS_N_MASK
    : c->registers->P & ~FLAGS_N_MASK; // set negative if bit 7 set
  
  c->registers->P = c->registers->X >= bus_read(c->ea, c) ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if y >= ms
  
  c->registers->P = c->registers->X == bus_read(c->ea, c) ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if y = m
}

void OP_cpy(context *c){
  uint8_t ret = c->registers->Y - bus_read(c->ea, c);
  c->registers->P = (ret & BIT_7_MASK) > 0 ? c->registers->P | FLAGS_N_MASK
    : c->registers->P & ~FLAGS_N_MASK; // set negative if bit 7 set
  
  c->registers->P = c->registers->Y >= bus_read(c->ea, c) ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if y >= ms
  
  c->registers->P = c->registers->Y == bus_read(c->ea, c) ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if y = m
}

void OP_cmp(context *c){
  uint8_t ret = c->registers->A - bus_read(c->ea, c);
  
  c->registers->P = c->registers->A >= bus_read(c->ea, c) ? c->registers->P | FLAGS_C_MASK
    : c->registers->P & ~FLAGS_C_MASK; // set carry if a >= m
  
  c->registers->P = c->registers->A == bus_read(c->ea, c) ? c->registers->P | FLAGS_Z_MASK
    : c->registers->P & ~FLAGS_Z_MASK;  // set zero if a = m

  c->registers->P = c->registers->P & ~FLAGS_N_MASK; //reset n bit
  c->registers->P |= ret & BIT_7_MASK; //set n bit if bit 7 of ret is set
}

/*-------- LOAD CALLS --------*/
void OP_ldx(context *c){
  c->registers->X = bus_read(c->ea, c);
  c->registers->P = (c->registers->X > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->X & BIT_7_MASK); //set N (7th bit) to bit 7 of X
}
void OP_lda(context *c){
  c->registers->A = bus_read(c->ea, c);
  c->registers->P = (c->registers->A > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->A & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_ldy(context *c){
  c->registers->Y = bus_read(c->ea, c);
  c->registers->P = (c->registers->Y > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->Y & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}

/*-------- MISC CALLS --------*/
void OP_nop(context *c){
  //printf("hello from nop!\n");
  return;
}
void OP_dex(context *c){
  c->registers->X--;
  c->registers->P = (c->registers->X > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->X & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_dey(context *c){
  c->registers->Y--;
  c->registers->P = (c->registers->Y > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->Y & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_inx(context *c){
  c->registers->X++;
  c->registers->P = (c->registers->X > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->X & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_iny(context *c){
  c->registers->Y++;
  c->registers->P = (c->registers->Y > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(c->registers->Y & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_inc(context *c){
  c->RAM[c->ea]++;
  c->registers->P = (bus_read(c->ea, c) > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(bus_read(c->ea, c) & BIT_7_MASK); //set N (7th bit) to bit 7 of A
}
void OP_dec(context *c){
  c->RAM[c->ea]--;
  c->registers->P = (bus_read(c->ea, c) > 0) ? c->registers->P & ~FLAGS_Z_MASK
    : c->registers->P | FLAGS_Z_MASK; // set zero if zero
  c->registers->P &= (~FLAGS_N_MASK)+(bus_read(c->ea, c) & BIT_7_MASK); //set N (7th bit) to bit 7 of A
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
void OP_brk(context *c){ //brk has a 2 byte opcode code
  c->registers->P |= FLAGS_B_MASK; //set break
  uint16_t pc_hi_byte = (c->registers->PC+1)& 0xFF00; //+1 offset to make space for brk pad
  uint16_t pc_lo_byte = (c->registers->PC+1)& 0xFF;
  c->registers->S --;
  bus_write(STACK_BOTTOM + c->registers->S, pc_hi_byte >> 8, c); //push hi byte, c);
  c->registers->S --;
  bus_write(STACK_BOTTOM + c->registers->S, pc_lo_byte, c); //push lo byte, c);
  c->registers->S --;
  bus_write(STACK_BOTTOM + c->registers->S, c->registers->P, c); //push flags to stack
  c->final_addr = bus_read(c->registers->PC, c); //for loggin
  c->registers->PC = get_16_bit_from(IRQB_VEC, c); //jump to vector location
}
void OP_rti(context *c){ //return from interrupt
  c->registers->P = bus_read(STACK_BOTTOM + c->registers->S, c); //pop flags
  c->registers->S ++;
  c->ea = get_16_bit_from(STACK_BOTTOM + c->registers->S, c); //pop address from stack
  c->registers->S += 2;
  c->registers->PC = c->ea;
  c->registers->P &= ~FLAGS_B_MASK; //unset break
}
