#ifndef PSD_EDA_H
#define PSD_EDA_H

#include "fft.h" // Necesitamos la definición de 'complex'

#define N_BANDS 10 // otra cosa?
void calcular_psd10(complex fft_in[], double psd_out[]);

#endif
