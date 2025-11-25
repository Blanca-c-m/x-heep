#include "desprueba.h"
#define escala 1000



 
void build_diagonales(long long int diag0[], long long int diag1[], long long int diag_1[], long long int diag2[], long long int diag_2[], int lambda) {
    long long int lambda_sq = (long long int)lambda * lambda;
    int N = signal_length;

    // Diagonal Principal (d0)
    for (int i = 0; i < N; i++) {
        long long int D2tD2_i = 0;

        // Coeficientes de la diagonal de D2^T * D2 (1, 5, 6) escalados.
        if (i == 0 || i == N - 1) D2tD2_i = 1 * escala;
        else if (i == 1 || i == N - 2) D2tD2_i = 5 * escala;
        else D2tD2_i = 6 * escala; 
        
        // diag0[i] = I[i,i] + lambda^2 * D2tD2[i,i]. I[i,i] es 'escala'.
        diag0[i] = escala + (lambda_sq * D2tD2_i) / escala;
    }

    // Diagonal +1 y -1 (d1 y d_1): A[i, i+1] ---
    for (int i = 0; i < N - 1; i++) {
        long long int D2tD2_ip1 = 0;
        // Coeficientes de la diagonal +1 de D2^T * D2 (-2, -4) escalados.
        if (i == 0 || i == N - 2) D2tD2_ip1 = -2 * escala;
        else D2tD2_ip1 = -4 * escala;
        
        diag1[i] = (lambda_sq * D2tD2_ip1) / escala; 
        diag_1[i + 1] = diag1[i]; // Simetría A[i+1, i] = A[i, i+1]
    }
    
    for (int i = 0; i < N - 2; i++) {
        long long int D2tD2_ip2 = 1 * escala; 
        diag2[i] = (lambda_sq * D2tD2_ip2) / escala; 
        diag_2[i+2] = diag2[i];
    }
}
// ELIMINACION POR TRIANGULAR SUPERIOR
void forward_elimination(long long int diag0[], long long int diag1[], long long int diag2[], long long int b[]) {
    long long int factor;
    int N = signal_length;

    for (int i = 0; i < N - 2; i++) { 
        long long int pivote = diag0[i]; 

        // ELIMINAR A[i+1, i] (Usando el factor = diag1[i]) ---
        factor = diag1[i]; // el valor es el mismo que diag_1 que es el que elimino

        // Actualizar A[i+1, i+1] (diag0[i+1])
        diag0[i + 1] -= (factor * diag1[i]) / pivote;

        // Actualizar A[i+1, i+2] (diag1[i+1])
        diag1[i + 1] -= (factor * diag2[i]) / pivote;
        
        // Actualizar b[i+1]
        b[i + 1] -= (factor * b[i]) / pivote;
        
        
        // ELIMINAR A[i+2, i] (Usando el factor = diag2[i]) ---
        factor = diag2[i]; // A[i+2, i]
        
        // Actualizar A[i+2, i+2] (diag0[i+2])
        diag0[i + 2] -= (factor * diag2[i]) / pivote;

        // Actualizar A[i+2, i+3] (diag1[i+2])
        if (i + 3 < N) {
            // Se usa A[i, i+1] = diag1[i] para la actualización.
            diag1[i + 2] -= (factor * diag1[i]) / pivote;
        }

        // Actualizar b[i+2]
        b[i + 2] -= (factor * b[i]) / pivote;
    }
    
    //Última eliminación para A[N-1, N-2] ---
    {
        int i = N - 2;
        long long int pivote = diag0[i];
        
        factor = diag1[i]; // A[N-1, N-2]
        
        // Actualizar diag0[N-1]
        diag0[i + 1] -= (factor * diag1[i]) / pivote;
        
        // Actualizar b[N-1]
        b[i + 1] -= (factor * b[i]) / pivote;
    }
}


// --- FASE 2: SUSTITUCIÓN HACIA ATRÁS (Back Substitution) ---

void back_substitution(long long int diag0[], long long int diag1[], long long int diag2[], long long int b[], long long int x[]) {
    int N = signal_length;

    // Inicializar el último valor
    // x[N-1] = b[N-1] / A'[N-1, N-1]
    x[N - 1] = (b[N - 1] * escala) / diag0[N - 1]; 

    // Inicializar el penúltimo valor
    // x[N-2] = (b[N-2] - A'[N-2, N-1] * x[N-1]) / A'[N-2, N-2]
    long long int term = (diag1[N - 2] * x[N - 1]) / escala; // A'[N-2, N-1] es diag1[N-2]
    x[N - 2] = ((b[N - 2] * escala) - term) / diag0[N - 2]; 

    // Bucle para el resto (desde N-3 hasta 0)
    for (int i = N - 3; i >= 0; i--) {
        long long int sum = 0;

        // Sumar término A'[i, i+1] * x[i+1]
        // A'[i, i+1] es el valor de diag1[i] MODIFICADO
        sum += (diag1[i] * x[i + 1]) / escala;
        
        // Sumar término A'[i, i+2] * x[i+2]
        // A'[i, i+2] es el valor de diag2[i] MODIFICADO
        sum += (diag2[i] * x[i + 2]) / escala;
        
        // x[i] = (b[i] - sum) / A'[i, i]
        x[i] = ((b[i] * escala) - sum) / diag0[i];
    }
}



// Función principal de solución
void solve(long long int diag0[], long long int diag1[], long long int diag2[], long long int b[], long long int x[]) {
    //Ejecutar la fase de eliminación
    forward_elimination(diag0, diag1, diag2, b);
    
    //Ejecutar la sustitución regresiva
    back_substitution(diag0, diag1, diag2, b, x);
}

