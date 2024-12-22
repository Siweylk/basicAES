//AUTHOR: Siweylk

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "./headers/cipher.h"
#include "./headers/invCipher.h"
#include "./headers/aux_funcs.h"

void encrypt(int in_file, int out_file, uint8_t *IV, uint32_t *w, int Nr){

    int bytes_read, i;
    uint8_t state[16], input[16], output[16];

    while(bytes_read = (read(in_file, input, 16))){
        if(bytes_read<16){
            AddPadding(input, bytes_read);
        }

        Transpose(input, state);

        XOR_blocks(state, IV);

        Cipher(state, w, Nr);

        //Update IV for next round
        for(i=0; i<16; i++){
            IV[i] = state[i];
        }

        Transpose(state, output);
        
        write(out_file, output, 16);
    }

}

void decrypt(int in_file, int out_file, uint8_t *IV, uint32_t *w, int Nr){

    int bytes_read, i;
    uint8_t state[16], input[16], output[16], buffer, padding;

    while(bytes_read = (read(in_file, input, 16))){

        Transpose(input, state);

        InvCipher(state, w, Nr);

        XOR_blocks(state, IV);

        Transpose(state, output);

        Transpose(input, IV);

        //CHECK WHAT HAPPENS WHEN IT IS 16 MULTIPLE

        //If not last block write 16 bytes, else, see how many padding bytes were added and write only the ones that were not padded
        size_t next_read = (read(in_file, &buffer, 1));
        if(next_read > 0) {
            lseek(in_file, -next_read, SEEK_CUR);
            write(out_file, output, 16);
        } 
        else{
            padding = read(in_file, &input[15], 1);
            write(out_file, output, 16-padding);

        }
    }

}


int main(int argc, char *argv[])
{
    int in_file;
    int out_file;
    int i=0, Nb=4, Nr, Nk, inputSize, key_length, mode;
    
    uint8_t state[16], IV[16], input[16], output[16];


    uint8_t* key;
    uint32_t* w;

    //ARG CHECK
    //Check correct number of arguments(file, key)
    if(argc != 3){
        printf("Incorrect amount of arguments, introduce a valid file and key\n");
        return 1;
    }



    key_length = strlen(argv[2]);
    key = malloc(key_length);
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

    //Open file to be encrypted and check if exists
    in_file = open(argv[1], O_RDONLY);

    if(in_file == -1){
        perror("Error");
        printf("The file does not exist or can not be opened");
        exit(1);
    }
    if(out_file == -1){
        perror("Error");
        printf("Cannot create output file");
        exit(1);
    }

    key = (uint8_t*)malloc(key_length*sizeof(uint8_t));
    w = (uint32_t*)malloc((4*(Nr+1))*sizeof(uint32_t));

    for(i=0; i<key_length; i++){
        key[i] = argv[2][i];
    }
    


    //SELECT MODE
    printf("1 encrypt, 2 decrypt, 0 exit:\n");
    scanf("%d", &mode); 

    switch(mode){
        case 0:
            return 0;
        case 1:
            
            //Create ciphered file
            char *out_file_enc = malloc(strlen(argv[1]) + 5);
            strcpy(out_file_enc, argv[1]);
            strcat(out_file_enc, ".ciph");
            out_file = open(out_file_enc, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

            KeyExpansion(key, w, Nk, Nb, Nr);
            Gen_random_IV(IV);
            encrypt(in_file, out_file, IV, w, Nr);

            remove(argv[1]);
            break;
        case 2:

            checkExt(argv[1]);
            char *out_file_dec = malloc(strlen(argv[1]) - 5);
            strncpy(out_file_dec, argv[1], strlen(argv[1]) - 5);
            out_file = open(out_file_dec, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

            KeyExpansion(key, w, Nk, Nb, Nr);
            Gen_random_IV(IV);  
            decrypt(in_file, out_file, IV, w, Nr);

            remove(argv[1]);
            break;
        default:
            printf("Unknown mode... Closing\n");
            return 1;
    }

    close(out_file);
    free(w);
    free(key);


    return 0;
}