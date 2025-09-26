#ifndef PINS_6502
#define PINS_6502

uint8_t address_bus[2];
uint8_t data_bus[1];

//TODO: combine these into 1 byte with masks
uint8_t BE; //bus enable
uint8_t IRQB; //interrupt request
uint8_t MLB; //memory lock
uint8_t NMIB; //non-maskable interrupt
uint8_t RWB; //read/write
uint8_t RDY; //ready
uint8_t RESB; //reset
uint8_t SOB; //set overflow
//TODO until here

uint8_t SYNC; //SYNChronize with OpCode fetch

//ignoring ground/VSS because lol

#endif
