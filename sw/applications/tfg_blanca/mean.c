#include "mean.h"
void mean(int input[], int length, int *output){

    int acc = 0;

    for (int i = 0; i < length; i++ ){ 
        acc += input[i];
    }

   *output = (acc * INV_N) >> 14;

}
