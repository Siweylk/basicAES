#include <stdint.h>

void SubBytes(uint8_t*);
uint32_t SubWord(uint32_t);
void ShiftRows(uint8_t *);
uint32_t ShiftWord(uint32_t);
void MixColumns(uint8_t*);
void KeyExpansion(uint8_t*, uint8_t*, uint32_t*, int, int, int);
void AddRoundKey(uint8_t*, uint32_t*, int);
void cipher(uint8_t*, uint32_t*, int);
