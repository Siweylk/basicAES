#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./headers/aux_funcs.h"

//Auxiliary functions
void Transpose(uint8_t* input, uint8_t* state){

    int i;

    for(i=0; i<16; i++){
        state[i] = input[4 * (i % 4)+(i/4)];
    }
}

void ShiftN(uint8_t* row, int n){
    
    unsigned char aux;
    int i, j;

    //Shift 1 position n times
    for(i=0; i<n; i++){
        aux = row[0];
        for(j=0; j<3; j++){
            row[j]=row[j+1];
        }
        row[j]=aux;
    }
}

uint8_t xtime(uint8_t x){
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

//Generate random IV
void Gen_random_IV(uint8_t* IV){

    int i;
    for(i=0; i<16; i++){
        IV[i]= 0x31;
    }
}


//XOR two blocks(state and IV)
void XOR_blocks(uint8_t* state, uint8_t* IV){

    int i;

    for(i=0; i<16; i++){
        state[i] ^= IV[i];
    }
}

//Multiplies two bytes, stolen from @kokke /tiny-AES-c
uint8_t Multiply(uint8_t x, uint8_t y){
    return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

//Check if extension is .ciph
int checkExt(char* name){
    
    int size = strlen(name);
    
    if(name[size-5] == '.' && name[size-4] == 'c' && name[size-3] == 'i' && name[size-2] == 'p' && name[size-1] == 'h'){
        return 1;
    }
    else{
        printf("The file doesn't have the neccessary extension\n");
        return 0;
    }
}