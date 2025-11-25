#include <stdio.h>
#include "signal_EDA.h"
#include "first_derivate.h"
#include "core_v_mini_mcu.h"
#include "x-heep.h"
#include "csr.h"
#include "mean.h"
#include "max.h"
#include "std_feat.h"
#include "peak.h"
#include "peakmin.h"
#include "peakmax.h"
#include "desprueba.h"

int main(void){
    //PRIMERA DERIVADA
    // // Inicializar a cero
    // long long int diag0[signal_length], diag1[signal_length], diag_1[signal_length], diag2[signal_length], diag_2[signal_length], tonic[signal_length];
    // for(int i=0;i<signal_length;i++) { diag0[i]=0; diag_1[i]=0; diag_2[i]=0; }
    // for(int i=0;i<signal_length-1;i++) diag1[i]=0;
    // for(int i=0;i<signal_length-2;i++) diag2[i]=0;
    // int lambda = 1;
    // long long int signal_ll[signal_length];
    // for(int i=0;i<signal_length;i++) signal_ll[i] = signal[i]; 
    // build_diagonales(diag0, diag1, diag_1, diag2, diag_2, lambda);

    // //print_diagonals(diag0, diag1, diag_1, diag2, diag_2);

    // //solve(diag0, diag1, diag2, diag_1, diag_2, signal_ll, tonic);
    // solve(diag0, diag1, diag2, signal_ll, tonic);
    // printf("Solución x:\n");
    // for (int i = 0; i < signal_length; i++) {
    //     printf("tonic[%d] = %d\r\n", i, tonic[i]);
    // }
    // unsigned int start_cycles, end_cycles, elapsed_cycles;

    // CSR_CLEAR_BITS(CSR_REG_MCOUNTINHIBIT, 0x1);

    // CSR_WRITE(CSR_REG_MCYCLE, 0);
    // CSR_READ(CSR_REG_MCYCLE, &start_cycles);







    // CSR_READ(CSR_REG_MCYCLE, &end_cycles);
    // elapsed_cycles = end_cycles - start_cycles;
    // printf("EXECUTION CYCLES: %d CYCLES \r\n", elapsed_cycles);
    //     //PEAK

    // int th1 = 100; // paso de 0.1 a 100 escala de 1000
    // int th2 = 1;
    // int w1 = 2;
    // int amp = 5;
    // int indice_max[signal_length];
    // int valor_max[signal_length];
    // int indice_min[signal_length];
    // int valor_min[signal_length];
    // int picos;
    // int num_max = 0;
    // int num_min = 0;

    // picos = peaks(signal, signal_length, frecuency, th1, w1, th2, amp, indice_max, valor_max, indice_min, valor_min, &num_max, &num_min);

    // printf("Maximos detectados \n");
    // printf("Total candidatos: %d\r\n", picos);

    // for(int i = 0; i < num_max; i++){
    //     printf("candidato para maximo numero %d es %d \r\n", indice_max[i], valor_max[i]);
    // }

    // for(int i = 0; i < num_min; i++){
    //     printf("candidato para minimo numero %d es %d \r\n", indice_min[i], valor_min[i]);
    // }
    
    // printf("SOLO MAXIMOS");
    // int n_max = peakmax(signal, signal_length, frecuency, th1, w1, th2, amp, indice_max, valor_max);
    // for(int i = 0; i < n_max; i++){  
    //     printf("Maximo en indice %d con valor %d\n", indice_max[i], valor_max[i]);
    // }

    // printf("SOLO MINIMOS");
    // int n_min = peakmin(signal, signal_length, frecuency, th1, w1, th2, amp, indice_min, valor_min);
    // for(int i = 0; i < n_min; i++){
    //     printf("Minimo en indice %d con valor %d\n", indice_min[i], valor_min[i]);
    // }

    //PRIMERA DERIVADA

    int first_deriv[signal_length];
    first_derivate(signal, first_deriv, frecuency, signal_length);
    for(int i = 0; i < 280; i++){
        printf("La primera derivada es: %d\r\n", first_deriv[i]);
    }

    // //MEDIA

    // int media = 0;
    // mean(signal, signal_length, &media);
    // printf("La media de enteros es: %d\r\n", media);

    // //MAX

    // int maximo = 0;
    // int indice_max = 0;
    // max(signal, signal_length, &maximo, &indice_max);
    // printf("El máximo valor es: %d y su indice es: %d\r\n", maximo, indice_max);


    // //MIN

    // int minimo = 0;
    // int indice_min = 0;
    // min(signal, signal_length, &minimo, &indice_min);
    // printf("El minimo valor es: %d y su indice es: %d\r\n", minimo, indice_min);

    //STD

    // int desv = 0;
    // std(signal, signal_length, &desv);
    // printf("La desviacion tipica es es: %d\r\n", desv);

    //PEAK

    // int th1 = 100; // paso de 0.1 a 100 escala de 1000
    // int th2 = 1;
    // int w1 = 4;
    // int amp = 2;
    // int indice[signal_length];
    // int valor_pico[signal_length];
    // int picos;
    // picos = peaks(signal, signal_length, frecuency, th1, w1, th2, amp, indice, valor_pico);

    // printf("Total de candidatos: %d\r\n", picos);
    // for(int i = 0; i < picos; i++){
    //     printf("candidato numero %d es %d \r\n", indice[i], valor_pico[i]);
    // }

    //FASICA Y TONICA

    // int tonica[signal_length];
    // int fasica[signal_length];
    // int valor_lambda = 1;
    // decompose_eda(signal, signal_length, tonica, fasica, valor_lambda);


    return 0;
}