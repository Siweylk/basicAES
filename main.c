//AUTHOR: Siweylk

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "./headers/cipher.h"
#include "./headers/aux_funcs.h"


int main(int argc, char *argv[])
{
    FILE* fptr;
    int i=0;
    
    //uint8_t state[17];
    //uint8_t* key;
    int key_length;

    int Nk=4;
    int Nb=4;
    int Nr=10;

    uint32_t w[44]; //44 poreque es 128, ajustar para resto
    //Check correct number of arguments(file, key)
    if(argc != 3){
        printf("Incorrect amount of arguments, introduce a valid file and key\n");
        return 1;
    }

    key_length = strlen(argv[2]);
    //Check key length
    switch(key_length){
        case 16:
            Nk=4;
            Nr=10;
            break;
        case 24:
            Nk=6;
            Nr=12;
            break;
        case 32:
            Nk=8;
            Nr=14;
            break;
        default:
            printf("The length of the key must be of 16, 24 or 32   %d\n", key_length);
            return 1;
    }

    Nb=4;
    for(i=0; i<key_length; i++){
        //key[i] = argv[2][i];
    }


    //Open file to be encrypted and check if exists
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("The file does not exist or can not be opened");
        exit(1);
    }


    //while(fgets(state, 17, fptr)){
        /*for(i=0; i<16; i++){
            printf("%c", block[i]);
        }*/
        //cipher(fptr, state, key);
    //}

    uint8_t input[16]= {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8_t state[16];
    uint8_t key[16]  = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    transpose(input, state);
    KeyExpansion(state, key, w, Nk, Nb, Nr);
    cipher(state, w, 10);

    for(i=0; i<16; i+=4){
        printf("Resultado final: %x%x%x%x, ", state[i], state[i+1], state[i+2], state[i+3]);
    }

    return 0;
}