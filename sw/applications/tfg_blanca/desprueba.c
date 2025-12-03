#include "desprueba.h"
#define escala 1000


void print_diagonals(long long int diag0[], long long int diag1[], long long int diag_1[], long long int diag2[], long long int diag_2[]) {
    int N = signal_length;

    printf("\n1. Diagonal Principal (diag0) [0..N-1]:\n");
    printf("   [ ");
    for (int i = 0; i < N; i++) {
        printf("%lld ", diag0[i]);
    }
    printf("]\n");

    printf("\n2. Diagonal Superior +1 (diag1) [0..N-2]:\n");
    printf("   [ ");
    for (int i = 0; i < N - 1; i++) {
        printf("%lld ", diag1[i]);
    }
    printf("]\n");

    printf("\n3. Diagonal Inferior -1 (diag_1) [1..N-1]:\n");
    printf("   [ - ");
    for (int i = 1; i < N; i++) {
        printf("%lld ", diag_1[i]);
    }
    printf("]\n");
    
    printf("\n4. Diagonal Superior +2 (diag2) [0..N-3]:\n");
    printf("   [ ");
    for (int i = 0; i < N - 2; i++) {
        printf("%lld ", diag2[i]);
    }
    printf("]\n");

    printf("\n5. Diagonal Inferior -2 (diag_2) [2..N-1]:\n");
    printf("   [ - - ");
    for (int i = 2; i < N; i++) {
        printf("%lld ", diag_2[i]);
    }
    printf("]\n");
    

}

 
void build_diagonales(long long int diag0[], long long int diag1[], long long int diag_1[], long long int diag2[], long long int diag_2[], int lambda) {

    long long int lambda_sq = (long long int)lambda * lambda;

    int N = signal_length;
    // Diagonal Principal (d0)
    for (int i = 0; i < N; i++) {
        long long int D2tD2 = 0;
        // Coeficientes de la diagonal de D2^T * D2 (1, 5, 6) escalados.
        if (i == 0 || i == N - 1) D2tD2 = 1 * escala;
        else if (i == 1 || i == N - 2) D2tD2 = 5 * escala;
        else D2tD2 = 6 * escala;

        // diag0[i] = I[i,i] + lambda^2 * D2tD2[i,i]. I[i,i] es 'escala'.
        diag0[i] = escala + (lambda_sq * D2tD2);
    }
    // Diagonal +1 y -1 (d1 y d_1): A[i, i+1] ---
    for (int i = 0; i < N - 1; i++) {

        long long int D2tD2 = 0;
        // Coeficientes de la diagonal +1 de D2^T * D2 (-2, -4) escalados.

        if (i == 0 || i == N - 2) D2tD2 = -2 * escala;
        else D2tD2 = -4 * escala;
        diag1[i] = (lambda_sq * D2tD2);
        diag_1[i + 1] = diag1[i]; // Simetría A[i+1, i] = A[i, i+1]

    }
    for (int i = 0; i < N - 2; i++) {

        long long int D2tD2_ip2 = 1 * escala;
        diag2[i] = (lambda_sq * D2tD2_ip2);
        diag_2[i+2] = diag2[i];

    }

}


// ELIMINACION POR TRIANGULAR SUPERIOR
void forward_elimination(long long int diag0[], long long int diag1[], long long int diag2[], long long int b[], long long int diag_1[], long long int diag_2[]) {
    long long int factor;
    int N = signal_length;
    for (int i = 0; i < N - 2; i++) {

        long long int pivote = diag0[i];

        factor = diag_1[i+1]; 
        // ACTUALIZO i + 1
        diag0[i + 1] -= (factor * diag1[i]) / pivote;
        diag1[i + 1] -= (factor * diag2[i]) / pivote;
        b[i + 1] -= (factor * b[i]) / pivote;

        factor = diag_2[i+2];
        //ACTUALIZO i + 2
        diag_1[i + 2] -= (factor * diag1[i]) / pivote;
        diag0[i + 2] -= (factor * diag2[i]) / pivote;
        b[i + 2] -= (factor * b[i]) / pivote;
        
    }

   

    //Última eliminación

    {
        int i = N - 2;

        long long int pivote = diag0[i];

        factor = diag_1[i+1]; // A[N-1, N-2]
        // Actualizar diag0[N-1]

        diag0[i + 1] -= (factor * diag1[i]) / pivote;

        // Actualizar b[N-1]
        b[i + 1] -= (factor * b[i]) / pivote;

    }

}

// SUSTITUCIÓN HACIA ATRÁS

void back_substitution(long long int diag0[], long long int diag1[], long long int diag2[], long long int b[], long long int x[], long long int ph[]) {
    int N = signal_length;

    // x[N-1] = b[N-1] / A'[N-1, N-1]
    x[N - 1] = (b[N - 1]*escala) / diag0[N - 1];

    long long int term = (diag1[N - 2] * x[N - 1]) / escala; 
    x[N - 2] = ((b[N - 2] - term)*escala) / diag0[N - 2];

    // Bucle para el resto (desde N-3 hasta 0)
    for (int i = N - 3; i >= 0; i--) {
        long long int sum = 0;

        sum += (diag1[i] * x[i + 1]) / escala;

        sum += (diag2[i] * x[i + 2]) / escala;

        x[i] = ((b[i] - sum))*escala/ diag0[i];

    }
}


void solve(long long int diag0[], long long int diag1[], long long int diag2[], long long int diag_1[], long long int diag_2[],long long int b[], long long int x[], long long int ph[]) {
    int N = signal_length;
    long long int b_original[N];

    for (int i = 0; i < N; i++) {
        b_original[i] = b[i];
    }
    //Ejecutar la fase de eliminación
    forward_elimination(diag0, diag1, diag2, b, diag_1,diag_2);
    //Ejecutar la sustitución regresiva
    back_substitution(diag0, diag1, diag2, b, x, ph);


    for (int i = 0; i < N; i++) {
        // Asumiendo que b_original y x están ambos escalados por 'escala'
        ph[i] = b_original[i] - x[i]; 
    }

}
