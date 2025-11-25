#include "first_derivate.h"

void first_derivate(int input[], int output[], int fs, int length){
    output[0] = 0;
    for (int i=1; i<length; i++){
        output[i] = (input[i] - input[i-1])*fs;
    }


}
