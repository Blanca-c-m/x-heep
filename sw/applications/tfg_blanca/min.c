#include "min.h"

void min(int input[], int length, int *valor, int *indice){
   int min_val = input[0];
   int min_idx = 0;

    for(int i = 1; i < length; i++){
        if(input[i] < min_val){
            min_val = input[i];
            min_idx = i;
        }
    }
        
    *valor = min_val;
    *indice = min_idx;
}
