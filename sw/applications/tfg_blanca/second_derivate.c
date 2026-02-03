#include "second_derivate.h"

void second_derivate(int input[], int length, int fs, int output[]){
    
    if (length < 3) return; // Aseguro que siempre tiene una longitud correcta para hacer la segunda derivada

    long int fs2 = fs * fs;
    output[0] = 0;
    output[1] = 0;
    
    for (int i = 2; i < length; i++){

        int diff = input[i] - 2*input[i - 1] + input[i - 2];

        diff *= fs2;
        output[i] = diff;
    }
}
