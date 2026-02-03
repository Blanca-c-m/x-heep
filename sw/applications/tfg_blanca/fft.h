#ifndef FFT_EDA_H
#define FFT_EDA_H

#include <stdint.h>
#define N_TOTAL 280
// Estructura de complejos
typedef struct {
    double re;
    double im;
} complex;

void FFT_CooleyTukey(int input[], complex fft_output[]);
void DFT(complex input[], complex output[], int N);
complex add(complex a, complex b);
complex multiply(complex a, complex b);

#endif
