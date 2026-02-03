#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "descompv3.h"

#define LAMBDA 1 
int shift_mul3(int A_Q, int B_Q) {
    // El producto A*B está en Q24 (seguro dentro de 32 bits).
    int prod_double = A_Q * B_Q; 
    // Desescalamos a Q_scale
    return prod_double >> Q_SCALE;
}

int shift_div3(int num, int den) {
    if (den == 0) return 0;
    
    // Encontrar la potencia de 2 más cercana (p2)
    int k = 0;
    int p2 = 1;
    // Buscamos p2 <= den < 2*p2
    while ((p2 << 1) > 0 && (p2 << 1) <= den) {
        p2 <<= 1;
        k++;
    }

    int R = den - p2; 

    int num_escalado_Q = num << Q_SCALE;
    int resultado_base_Q = num_escalado_Q >> k; // Q14
    
    int R_temp_Q = R << Q_SCALE; 
    int R_over_p2_Q = R_temp_Q >> k; 

    int termino_correccion_Q = shift_mul3(R_over_p2_Q, resultado_base_Q);
    
    int resultado_final_Q = resultado_base_Q - termino_correccion_Q;

    return resultado_final_Q;
}

// Matriz Triangular Superior U
int EDA_U_diag0[signal_length];
int EDA_U_diag1[signal_length - 1]; 
int EDA_U_diag2[signal_length - 2]; 

// Multiplicadores de la Matriz Inferior L
int EDA_L_mult_1[signal_length - 1]; // Diagonal -1
int EDA_L_mult_2[signal_length - 2]; // Diagonal -2


// esto se hace una vez y ya se queda almacenado mientras el sistema esté en uso
void EDA_build_A_and_factorize() {
    int lambda_Q = LAMBDA * ONE_Q;
    int lambda_sq = shift_mul3(lambda_Q, lambda_Q);
    int N = signal_length;

    int diag_1[signal_length];
    int diag_2[signal_length];

    for (int i = 0; i < N; i++) {
        int D2tD2_0= 0;
        if (i == 0 || i == N - 1) D2tD2_0 = 1*ONE_Q;
        else if (i == 1 || i == N - 2) D2tD2_0 = 5*ONE_Q;
        else D2tD2_0 = 6*ONE_Q;
        EDA_U_diag0[i] = ONE_Q + shift_mul3(lambda_sq, D2tD2_0);
    }

    for (int i = 0; i < N - 1; i++) {
        int D2tD2_1=0;
        if (i == 0 || i == N - 2) D2tD2_1 = -2*ONE_Q;
        else D2tD2_1 = -4*ONE_Q;
        EDA_U_diag1[i] = shift_mul3(lambda_sq, D2tD2_1);
        diag_1[i + 1] = EDA_U_diag1[i];
    }

    for (int i = 0; i < N - 2; i++) {
        int D2tD2_2 = 1 * ONE_Q;
        EDA_U_diag2[i] = shift_mul3(lambda_sq, D2tD2_2);
        diag_2[i + 2] = EDA_U_diag2[i];
    }

    for (int i = 0; i < N - 2; i++) {
        int pivote = EDA_U_diag0[i];

        int factor1 = diag_1[i + 1];
        EDA_L_mult_1[i] = shift_div3(factor1, pivote); // comprobar
        EDA_U_diag0[i + 1] -= shift_mul3(EDA_L_mult_1[i], EDA_U_diag1[i]);
        EDA_U_diag1[i + 1] -= shift_mul3(EDA_L_mult_1[i], EDA_U_diag2[i]);

        int factor2 = diag_2[i + 2];
        EDA_L_mult_2[i] = shift_div3(factor2, pivote);
        diag_1[i + 2] -= shift_mul3(EDA_L_mult_2[i], EDA_U_diag1[i]);
        EDA_U_diag0[i + 2] -= shift_mul3(EDA_L_mult_2[i], EDA_U_diag2[i]);
    }

    {
        int i = N - 2;
        int pivote = EDA_U_diag0[i];
        int factor = diag_1[i + 1];
        EDA_L_mult_1[i] = shift_div3(factor, pivote);
        EDA_U_diag0[i + 1] -= shift_mul3(EDA_L_mult_1[i], EDA_U_diag1[i]);
    }
}


void forward_elimination_v3(int b[]) {
    int N = signal_length;
    for (int i = 0; i < N - 2; i++) {
        b[i + 1] -= shift_mul3(EDA_L_mult_1[i], b[i]);
        b[i + 2] -= shift_mul3(EDA_L_mult_2[i], b[i]);
    }
    {
        int i = N - 2;
        b[i + 1] -= shift_mul3(EDA_L_mult_1[i], b[i]);;
    }
}


void back_substitution_v3(int b[], int x[]) {

    int N = signal_length;

    x[N - 1] = shift_div3(b[N - 1], EDA_U_diag0[N - 1]);

    int term = shift_mul3(EDA_U_diag1[N - 2],x[N - 1]) ;
    int numerador = (b[N - 2] - term);
    x[N - 2] = shift_div3(numerador, EDA_U_diag0[N - 2]);

    for (int i = N - 3; i >= 0; i--) {
    int sum = 0; 

    sum += shift_mul3(EDA_U_diag1[i] , x[i + 1]);
    sum += shift_mul3(EDA_U_diag2[i] , x[i + 2]);
    numerador = (b[i] - sum);
    x[i] = shift_div3(numerador, EDA_U_diag0[i]);
}
}


void solve_v3(int Xeda[], int Xtonica[], int Xfasica[]) {
    int N = signal_length;
    int b_temp[N]; 

    for (int i = 0; i < N; i++) {
        b_temp[i] = Xeda[i];
    }

    forward_elimination_v3(b_temp);

    back_substitution_v3(b_temp, Xtonica); 

    for (int i = 0; i < N; i++) {
        Xfasica[i] = Xeda[i] - Xtonica[i];
    }
}
