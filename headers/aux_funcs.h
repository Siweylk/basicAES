#include <stdint.h>

void transpose(uint8_t*, uint8_t*);
void ShiftN(uint8_t*, int);
uint8_t xtime(uint8_t);
void gen_random_IV(uint8_t*);
void XOR_blocks(uint8_t*, uint8_t*);