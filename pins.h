#ifndef PINS_6502
#define PINS_6502

char address_bus[2];
char data_bus[1];

//TODO: combine these into 1 byte with masks
char BE; //bus enable
char IRQB; //interrupt request
char MLB; //memory lock
char NMIB; //non-maskable interrupt
char RWB; //read/write
char RDY; //ready
char RESB; //reset
char SOB; //set overflow
//TODO until here

char SYNC; //SYNChronize with OpCode fetch

//ignoring ground/VSS because lol

#endif
