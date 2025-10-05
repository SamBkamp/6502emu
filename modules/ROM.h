#ifndef ROM_6502
#define ROM_6502

uint8_t ROM_read(uint16_t address);
void ROM_write(uint16_t address, uint8_t data);
uint8_t *ROM_init();
#endif
