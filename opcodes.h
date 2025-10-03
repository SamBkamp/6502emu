#ifndef OPCODES_6502
#define OPCODES_6502

/* addressing moding functions */
void addr_implied(context *c); // i
void addr_accumulator(context *c); // A
void addr_imm(context *c); // #
void addr_pcr(context *c); // r
void addr_stack(context *c); //s
void addr_zp(context *c); // zp
void addr_zp_idx_x(context *c); // zp,x
void addr_zp_idx_y(context *c); // zp,y
void addr_zp_indirect(context *c); // (zp)
void addr_zp_idx_indirect(context *c); // (zp, x)
void addr_zp_idx_y_indirect(context *c); // (zp), y
void addr_abs_indirect(context *c); // (a)
void addr_abs_idx_indirect(context *c); // (a, x) 
void addr_abs(context *c); // a
void addr_abs_x(context *c); // a,x
void addr_abs_y(context *c); // a,y

/* opcode implementations */
//BRANCHING CALLS
void OP_bcc(context *c);
void OP_bcs(context *c);
void OP_beq(context *c);
void OP_bmi(context *c);
void OP_bne(context *c);
void OP_bpl(context *c);
void OP_bra(context *c);
void OP_bvs(context *c);
void OP_bvc(context *c);
void OP_jmp(context *c);
void OP_jsr(context *c);
void OP_rts(context *c);

//STACK CALLS
void OP_pha(context *c);
void OP_php(context *c);
void OP_phx(context *c);
void OP_phy(context *c);
void OP_plp(context *c);
void OP_plx(context *c);
void OP_ply(context *c);
void OP_pla(context *c);

//ARITHMETIC
void OP_asl(context *c);
void OP_aslA(context *c); //hacky implimentation to use accumulator instead of pointer
void OP_lsr(context *c);
void OP_lsrA(context *c); //hacky implimentation to use accumulator instead of pointer
void OP_rol(context *c);
void OP_rolA(context *c); //hacky implimentation to use accumulator instead of pointer
void OP_ror(context *c);
void OP_rorA(context *c); //hacky implimentation to use accumulator instead of pointer
void OP_and(context *c);
void OP_bit(context *c);
void OP_eor(context *c);
void OP_ora(context *c);
void OP_adc(context *c);
void OP_sbc(context *c);

//COMPARISONS
void OP_cpx(context *c);
void OP_cpy(context *c);
void OP_cmp(context *c);

//STATUS FLAG CHANGES
void OP_clc(context *c);
void OP_cld(context *c);
void OP_cli(context *c);
void OP_clv(context *c);
void OP_sec(context *c);
void OP_sed(context *c);
void OP_sei(context *c);

//STORE CALLS
void OP_sta(context *c);
void OP_stx(context *c);
void OP_sty(context *c);
void OP_stz(context *c);

//LOAD CALLS
void OP_ldx(context *c);
void OP_lda(context *c);
void OP_ldy(context *c);

//MISC
void OP_nop(context *c);
void OP_dex(context *c);
void OP_dey(context *c);
void OP_inx(context *c);
void OP_iny(context *c);
void OP_dec(context *c);
void OP_inc(context *c);
void OP_tay(context *c);
void OP_tya(context *c);
void OP_tax(context *c);
void OP_txa(context *c);
void OP_tsx(context *c);
void OP_txs(context *c);

#endif
