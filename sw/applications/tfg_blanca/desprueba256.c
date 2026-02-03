#include "desprueba256.h"

//FUNCION CON DOS MEJORAS. SE ALMACENAN SOLO LAS DIAGONALES Y ADEMAS NO SE DIVIDE, SE HACEN DESPLAZAMIENTOS
void print_diagonals_v2(int diag0[], int diag1[], int diag_1[], int diag2[], int diag_2[]) {
    int N = signal_length;

    printf("\nDiagonal Principal:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", diag0[i]);
    }

    printf("\nDiagonal Superior:\n");
    for (int i = 0; i < N - 1; i++) {
        printf("%d ", diag1[i]);
    }

    printf("\nDiagonal Inferior:\n");
    for (int i = 1; i < N; i++) {
        printf("%d ", diag_1[i]);
    }
    
    printf("\nDiagonal Superior Segunda:\n");
    for (int i = 0; i < N - 2; i++) {
        printf("%d ", diag2[i]);
    }

    printf("\nDiagonal Inferior Segunda:\n");
    for (int i = 2; i < N; i++) {
        printf("%d ", diag_2[i]);
    }

}
int shift_mul(int A_Q, int B_Q) {
    // El producto A*B está en Q24 (seguro dentro de 32 bits).
    int prod_double = A_Q * B_Q; 
    // Desescalamos a Q_scale
    return prod_double >> Q_SCALE;
}
int shift_div(int num, int den) {
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

    int termino_correccion_Q = shift_mul(R_over_p2_Q, resultado_base_Q);
    
    int resultado_final_Q = resultado_base_Q - termino_correccion_Q;

    return resultado_final_Q;
}

void build_diagonales_v2(int diag0[], int diag1[], int diag_1[], int diag2[], int diag_2[], int lambda) {
    int lambda_Q = lambda * ONE_Q;
    // Lambda^2 se calcula con multiplicación de punto fijo (Q14 * Q14 -> Q14)
    int lambda_sq_Q = shift_mul(lambda_Q, lambda_Q);
    int N = signal_length;

    for (int i = 0; i < N; i++) {
        int D2tD2 = 0;
        if (i == 0 || i == N - 1) D2tD2 = 1 * ONE_Q;
        else if (i == 1 || i == N - 2) D2tD2 = 5 * ONE_Q;
        else D2tD2 = 6 * ONE_Q;

        diag0[i] = ONE_Q + shift_mul(lambda_sq_Q, D2tD2);
    }

    for (int i = 0; i < N - 1; i++) {

        int D2tD2 = 0;

        if (i == 0 || i == N - 2) D2tD2 = -2 * ONE_Q;
        else D2tD2 = -4 * ONE_Q;

        diag1[i] = shift_mul(lambda_sq_Q, D2tD2);
        diag_1[i + 1] = diag1[i];
    }

    for (int i = 0; i < N - 2; i++) {
        int D2tD2_ip2 = 1 * ONE_Q;
        diag2[i] = shift_mul(lambda_sq_Q, D2tD2_ip2);
        diag_2[i + 2] = diag2[i];
    }
}

void forward_elimination_v2(int diag0[], int diag1[], int diag2[], int b[], int diag_1[], int diag_2[]) {
    int factor;
    int N = signal_length;
    for (int i = 0; i < N - 2; i++) {

        int pivote = diag0[i];

        factor = diag_1[i+1];

        // Calcular L[i+1, i] = factor / pivote 
        int L_i1_i = shift_div(factor, pivote); 

        diag0[i + 1] -= shift_mul(L_i1_i, diag1[i]);
        diag1[i + 1] -= shift_mul(L_i1_i, diag2[i]);
        b[i + 1] -= shift_mul(L_i1_i, b[i]);

        factor = diag_2[i+2];

        // Calcular L[i+2, i] = factor / pivote 
        int L_i2_i = shift_div(factor, pivote);

        // Eliminación usando shift_mul
        diag_1[i + 2] -= shift_mul(L_i2_i, diag1[i]);
        diag0[i + 2] -= shift_mul(L_i2_i, diag2[i]);
        b[i + 2] -= shift_mul(L_i2_i, b[i]);
    }

    // Última eliminación (caso i = N - 2)
    {
        int i = N - 2;
        int pivote = diag0[i];

        factor = diag_1[i+1];
        
        int L_i1_i = shift_div(factor, pivote);

        diag0[i + 1] -= shift_mul(L_i1_i, diag1[i]);
        b[i + 1] -= shift_mul(L_i1_i, b[i]);
    }
}

void back_substitution_v2(int diag0[], int diag1[], int diag2[], int b[], int x[], int ph[]) {

    int N = signal_length;

    x[N - 1] = shift_div(b[N - 1], diag0[N - 1]);

    int term = shift_mul(diag1[N - 2], x[N - 1]);

 
    int numerador = b[N - 2] - term;
    
 
    x[N - 2] = shift_div(numerador, diag0[N - 2]);

    for (int i = N - 3; i >= 0; i--) {

        int sum = 0;

        sum += shift_mul(diag1[i], x[i + 1]); 
        

        sum += shift_mul(diag2[i], x[i + 2]);

        numerador = b[i] - sum;
        
        x[i] = shift_div(numerador, diag0[i]);
    }
}

void solve_v2(int diag0[], int diag1[], int diag2[], int diag_1[], int diag_2[], int b[], int x[], int ph[]) {

    int N = signal_length;
    int b_original[N];

    for (int i = 0; i < N; i++) {
        b_original[i] = b[i];
    }

    forward_elimination_v2(diag0, diag1, diag2, b, diag_1, diag_2);

    back_substitution_v2(diag0, diag1, diag2, b, x, ph);

    for (int i = 0; i < N; i++) {
        ph[i] = b_original[i] - x[i];
    }
}
