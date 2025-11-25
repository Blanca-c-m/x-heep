#include "max.h"

void max(int input[], int length, int *valor, int *indice){
    *valor = input[0];
    *indice= 0;

    for(int i=1; i < length;  i++){
        if(input[i] > *valor){
            *valor= input[i];
            *indice = i;
        }

    }
}