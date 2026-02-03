#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "signal_EDA.h"
#include "FFT.h"
#ifndef PI
#define PI 3.14159265358979323846
#endif

// Reservo memoria
static complex grid[14][20]; 
static complex temp_buffer[20];

// funcion suma
complex add(complex a, complex b) {
    complex res = {a.re + b.re, a.im + b.im};
    return res;
}
//funcion multiplicación
complex multiply(complex a, complex b) {
    complex res;
    res.re = a.re * b.re - a.im * b.im;
    res.im = a.re * b.im + a.im * b.re;
    return res;
}


// Transformada discreta de Fourier
void DFT(complex input[], complex output[], int N) {
    for (int k = 0; k < N; k++) {
        output[k].re = 0.0;
        output[k].im = 0.0;
        for (int n = 0; n < N; n++) {
            // Fórmula del ángulo: -2 * PI * posición * frecuencia / total_muestras
            double angle = -2.0 * PI * n * k / N;
            complex trig = {cos(angle), sin(angle)};
            
            complex prod = multiply(input[n], trig);
            output[k] = add(output[k], prod);
        }
    }
}

void FFT_CooleyTukey(int input[], complex fft_output[]) {
    int N1 = 14;
    int N2 = 20;
    // recorro columna y fila
    for (int k1 = 0; k1 < N1; k1++) {
        // Preparar buffer para la columna k1
        for (int k2 = 0; k2 < N2; k2++) {

            temp_buffer[k2].re = input[N1 * k2 + k1];
            temp_buffer[k2].im = 0.0;
        }

        // Calcular DFT de la columna y guardar en el grid
        DFT(temp_buffer, grid[k1], N2);

        // Factores de giro
        for (int k2 = 0; k2 < N2; k2++) {
            double angle = -2.0 * PI * k1 * k2 / N_TOTAL;
            complex trig = {cos(angle), sin(angle)};
            grid[k1][k2] = multiply(trig, grid[k1][k2]);
        }
    }

    for (int k2 = 0; k2 < N2; k2++) {
        for (int k1 = 0; k1 < N1; k1++) {
            temp_buffer[k1] = grid[k1][k2];
        }

        complex out[14]; 
        DFT(temp_buffer, out, N1);

        for (int k1 = 0; k1 < N1; k1++) {
            fft_output[N2 * k1 + k2] = out[k1];
        }
    }
}
