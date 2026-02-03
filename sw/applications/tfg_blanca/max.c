#include "max.h"

void max(int input[], int length, int *valor, int *indice){

   int max_val = input[0];
   int max_idx = 0;

    for(int i=1; i < length;  i++){
        if(input[i] > max_val){
            max_val= input[i];
            max_idx = i;
        }

    }
    
    *valor = max_val;
    *indice = max_idx;
}
