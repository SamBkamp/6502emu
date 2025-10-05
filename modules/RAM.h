#ifndef RAM_6502
#define RAM_6502

uint8_t RAM_read(uint16_t address);
void RAM_write(uint16_t address, uint8_t data);
uint8_t *RAM_init();
#endif
