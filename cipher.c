#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "./headers/cipher.h"
#include "./headers/aux_funcs.h"


unsigned char SubBox[256]={
//      0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
/*0*/   0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
/*1*/   0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
/*2*/   0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
/*3*/   0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
/*4*/   0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
/*5*/   0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
/*6*/   0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
/*7*/   0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
/*8*/   0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
/*9*/   0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
/*A*/   0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
/*B*/   0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
/*C*/   0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
/*D*/   0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
/*E*/   0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
/*F*/   0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};


//Bytes transformation
void SubBytes(uint8_t* state){

    int i;
    for(i=0; i<16; i++){
        state[i] = SubBox[state[i]];
    }
}

//Word transformation
uint32_t SubWord(uint32_t value){

    uint8_t byte1, byte2, byte3, byte4;

    //Disassemble word into bytes
    byte1 = (uint8_t)(value >> 24) & 0xFF;
    byte2 = (uint8_t)(value >> 16) & 0xFF;
    byte3 = (uint8_t)(value >> 8) & 0xFF; 
    byte4 = (uint8_t)value & 0xFF;

    //Substitute bytes
    byte1 = SubBox[byte1];
    byte2 = SubBox[byte2];
    byte3 = SubBox[byte3];
    byte4 = SubBox[byte4];

    //Reassemble bytes
    value = ((uint32_t)byte1 << 24);
    value |= ((uint32_t)byte2 << 16);
    value |= ((uint32_t)byte3 << 8);
    value |= (uint32_t)byte4;

    return value;
}

//Row shift
void ShiftRows(uint8_t* block){

    ShiftN(&block[4], 1);
    ShiftN(&block[8], 2);
    ShiftN(&block[12], 3);
}


uint32_t ShiftWord(uint32_t value){

    uint8_t byte1, byte2, byte3, byte4;

    //Disassemble word into bytes
    byte1 = (uint8_t)(value >> 24) & 0xFF;
    byte2 = (uint8_t)(value >> 16) & 0xFF;
    byte3 = (uint8_t)(value >> 8) & 0xFF; 
    byte4 = (uint8_t)value & 0xFF;

    //Reassemble reorderedbytes
    value =  ((uint32_t)byte2 << 24); 
    value |= ((uint32_t)byte3 << 16);
    value |= ((uint32_t)byte4 << 8); 
    value |= (uint32_t)byte1;

    return value;
}

void MixColumns(uint8_t* state){

    int i;
    unsigned char tmpState[16];
    for(i=0; i<4; i++){
        tmpState[i] = xtime(state[i]) ^ (xtime(state[i+4]) ^ state[i+4]) ^ state[i+8] ^ state[i+12];
        tmpState[i+4] = state[i] ^ xtime(state[i+4]) ^ (xtime(state[i+8]) ^ state[i+8])  ^ state[i+12];
        tmpState[i+8] = state[i] ^ state[i+4] ^ xtime(state[i+8])  ^ (xtime(state[i+12]) ^ state[i+12]);
        tmpState[i+12] = (xtime(state[i]) ^ state[i]) ^ state[i+4] ^ state[i+8]  ^ xtime(state[i+12]);
    }

    for(i=0; i<16; i++){
        state[i] = tmpState[i];
    }
}


void KeyExpansion( uint8_t* key, uint32_t* w, int Nk, int Nb, int Nr){
    
    uint32_t temp;
    uint32_t Rcon[11] = {0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
    int i = 0;

    while (i < Nk){
        w[i] = ((uint32_t)key[4*i] << 24);
        w[i] |= ((uint32_t)key[(4*i)+1] << 16);
        w[i] |= ((uint32_t)key[(4*i)+2] << 8);
        w[i] |= (uint32_t)key[(4*i)+3];
        i++;
    }
    
    i = Nk;

    while (i < Nb * (Nr+1)){
        temp = w[i-1];
        if(i % Nk == 0){
            temp = SubWord(ShiftWord(temp)) ^ Rcon[i/Nk];
        }else if ((Nk > 6) && ((i % Nk) == 4)){
            temp = SubWord(temp);
        }
        w[i] = (w[i-Nk] ^ temp);
        i++;
    }
}

void AddRoundKey(uint8_t* state, uint32_t* w, int ind){

    int i, j;
    uint8_t byte1, byte2, byte3, byte4;
    uint8_t prueba[16];

    for(i=0; i<4; i++){
        
        //Disassemble word into bytes
        byte1 = (uint8_t)(w[4*ind] >> (24-8*i)) & 0xFF;
        byte2 = (uint8_t)(w[(4*ind)+1] >> (24-8*i)) & 0xFF;
        byte3 = (uint8_t)(w[(4*ind)+2] >> (24-8*i)) & 0xFF; 
        byte4 = (uint8_t)(w[(4*ind)+3] >> (24-8*i)) & 0xFF;
        
        //Add Rounded Key to state
        state[4*i] ^= byte1;
        state[4*i+1] ^= byte2;
        state[4*i+2] ^= byte3;
        state[4*i+3] ^= byte4;

        //TEST TO FIND ERROR
        prueba[4*i] = byte1;
        prueba[4*i+1] = byte2;
        prueba[4*i+2] = byte3;
        prueba[4*i+3] = byte4;
    }
}

//Add Padding according to ANSI X.923
void AddPadding(uint8_t* input, int size){

    int i;

    for(i=size; i<16; i++){
        input[i] = (uint8_t)(16-size);
    }
}

//Cipher funtion
void Cipher(uint8_t* state, uint32_t* w, int Nr){
    
    int j, i = 0;

    AddRoundKey(state, w, i);

    for(i=1; i<Nr; i++){
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, w, i);
        
    }

    SubBytes(state); 
    ShiftRows(state);
    AddRoundKey(state, w, i);

}