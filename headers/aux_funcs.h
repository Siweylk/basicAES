#include <stdint.h>

void Transpose(uint8_t*, uint8_t*);
void ShiftN(uint8_t*, int);
uint8_t xtime(uint8_t);
void Gen_random_IV(uint8_t*);
void XOR_blocks(uint8_t*, uint8_t*);
uint8_t Multiply(uint8_t, uint8_t);
int checkExt(char*);