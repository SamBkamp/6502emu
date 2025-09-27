#ifndef OPCODES_6502
#define OPCODES_6502

/* addressing moding functions */
void addr_implied(context *c);
void addr_accumulator(context *c);
void addr_imm(context *c);
void addr_pcr(context *c);

/* opcode implementations */
void OP_bcc(context *c);
void OP_bcs(context *c);
void OP_beq(context *c);
void OP_bmi(context *c);
void OP_bne(context *c);
void OP_bpl(context *c);
void OP_bra(context *c);
void OP_bvs(context *c);
void OP_bvc(context *c);
void OP_nop(context *c);
void OP_ldx(context *c);


#endif
