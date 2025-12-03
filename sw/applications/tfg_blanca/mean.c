#include "mean.h"


void mean(int input[], int length, int *output){
    int mean=0;
    for (int i=0; i < length; i++ ){ //int i=0; i <= length; i++
        mean += input[i];
    }
   *output = mean/length;
}