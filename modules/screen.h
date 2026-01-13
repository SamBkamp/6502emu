#ifndef screen
#define screen

uint8_t *screen_init();
void screen_write(uint16_t address, uint8_t data);
uint8_t screen_read(uint16_t address);

#endif
