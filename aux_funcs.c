#include "./headers/aux_funcs.h"

//Auxiliary functions
void transpose(uint8_t* input, uint8_t* state){

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