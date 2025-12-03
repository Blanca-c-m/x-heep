#include "std_feat.h"
#include <stdint.h>

void std(int input[], int length, int *output){
    int mean = 0;
    int varianza = 0;
    int diff = 0;
    int sum = 0;


    for(int i = 0; i < length; i++){
        sum += input [i];
    }
    mean = sum / length;

    //printf("La media es: %d\r\n",mean);
    for(int i = 0; i < length; i++){
        diff = input[i] - mean;
        varianza += diff * diff;
    }

    varianza = varianza / (length - 1);

    //printf("La varianza es: %d\r\n", varianza);

    *output  = sqrt(varianza);
}
