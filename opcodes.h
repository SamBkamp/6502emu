#ifndef OPCODES_6502
#define OPCODES_6502

/* addressing moding functions */
void addr_implied(context *c);
void addr_accumulator(context *c);
void addr_imm(context *c);
void addr_pcr(context *c);
void addr_stack(context *c);
void addr_zp(context *c);
void addr_zp_indirect(context *c);
void addr_abs_indirect(context *c);
void addr_abs(context *c);

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

//STACK CALLS
void OP_pha(context *c);
void OP_php(context *c);
void OP_phx(context *c);
void OP_phy(context *c);
void OP_plp(context *c);
void OP_plx(context *c);
void OP_ply(context *c);

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

//LOAD CALLS
void OP_ldx(context *c);

//MISC
void OP_nop(context *c);


#endif
