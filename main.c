//AUTHOR: Siweylk

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "./headers/cipher.h"
#include "./headers/invCipher.h"
#include "./headers/aux_funcs.h"


int main(int argc, char *argv[])
{
    FILE* in_file, out_file;
    int i=0, Nb=4, Nr, Nk, inputSize, key_length;
    
    uint8_t state[16], IV[16], input[16], output[16];
    uint8_t mode;

    uint8_t* key;
    uint32_t* w;

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

    key = (uint8_t*)malloc(key_length*sizeof(uint8_t));
    w = (uint32_t*)malloc((4*(Nr+1))*sizeof(uint32_t));

    for(i=0; i<key_length; i++){
        key[i] = argv[2][i];
    }

    //Open file to be encrypted and check if exists
    in_file = fopen(argv[1], "rb");
    if(in_file == NULL){
        perror("Error");
        printf("The file does not exist or can not be opened");
        exit(1);
    }

    //scanf("Select")

    KeyExpansion(key, w, Nk, Nb, Nr);
    Gen_random_IV(IV);



    while(fgets(input, 17, in_file)){
    //while(fread(input, 1, 16, in_file)){ //NO AÑADE PADDING
        inputSize = strlen(input);
        if(inputSize<16){
            AddPadding(input, inputSize);//MIRAR COMO TRATA LOS SALTOS DE LINEA
        }

        Transpose(input, state);
        printf("Texto sin cifrar: ");
        for(i=0; i<16; i++){
            printf("%02X", state[i]);
        }
        printf("\n");
        XOR_blocks(state, IV);

        Cipher(state, w, Nr);

        printf("\n");
        //Update IV for next round
        for(i=0; i<16; i++){
            IV[i] = state[i];
        }

        //transpose(state, output);

        //REstore the IV to decrypt the file
        Gen_random_IV(IV);
        //TEST DECRYPTION
        InvCipher(state, w, Nr);
        XOR_blocks(state, IV);
        printf("Texto descifrado: ");
        for(i=0; i<16; i++){
            printf("%02X", state[i]);
        }
        printf("\n");

        /*for(i=0; i<16; i++){
            printf("%02X", output[i]);
        }
        printf("\n");*/
    }


    return 0;
}