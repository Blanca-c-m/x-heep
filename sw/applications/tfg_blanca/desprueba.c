#include "desprueba.h" 

// DESCOMPOSICION DE LA SEÑAL CON LA MEJORA DE QUE ALMACENA SOLO LAS DIAGONALES, EN ESTA FUNCION SE SIGUE DIVIDIENDO.
void print_diagonals_v1(int diag0[], int diag1[], int diag_1[], int diag2[], int diag_2[]) {
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

void build_diagonales_v1(int diag0[], int diag1[], int diag_1[], int diag2[], int diag_2[], int lambda) {

    int lambda_sq = (int)lambda * lambda;

    int N = signal_length;

    for (int i = 0; i < N; i++) {
        int D2tD2 = 0;

        if (i == 0 || i == N - 1) D2tD2 = 1 * scale;
        else if (i == 1 || i == N - 2) D2tD2 = 5 * scale;
        else D2tD2 = 6 * scale;

        diag0[i] = scale + (lambda_sq * D2tD2);
    }

    for (int i = 0; i < N - 1; i++) {

        int D2tD2 = 0;

        if (i == 0 || i == N - 2) D2tD2 = -2 * scale;
        else D2tD2 = -4 * scale;

        diag1[i] = (lambda_sq * D2tD2);
        diag_1[i + 1] = diag1[i];
    }

    for (int i = 0; i < N - 2; i++) {

        int D2tD2_ip2 = 1 * scale;
        diag2[i] = (lambda_sq * D2tD2_ip2);
        diag_2[i + 2] = diag2[i];
    }
}

void forward_elimination_v1(int diag0[], int diag1[], int diag2[], int b[], int diag_1[], int diag_2[]) {
    int factor;
    int N = signal_length;

    for (int i = 0; i < N - 2; i++) {

        int pivote = diag0[i];

        factor = diag_1[i + 1];
        diag0[i + 1] -= (factor * diag1[i]) / pivote;
        diag1[i + 1] -= (factor * diag2[i]) / pivote;
        b[i + 1] -= (factor * b[i]) / pivote;

        factor = diag_2[i + 2];
        diag_1[i + 2] -= (factor * diag1[i]) / pivote;
        diag0[i + 2] -= (factor * diag2[i]) / pivote;
        b[i + 2] -= (factor * b[i]) / pivote;
    }

    {
        int i = N - 2;

        int pivote = diag0[i];

        factor = diag_1[i + 1];
        diag0[i + 1] -= (factor * diag1[i]) / pivote;
        b[i + 1] -= (factor * b[i]) / pivote;
    }
}

void back_substitution_v1(int diag0[], int diag1[], int diag2[], int b[], int x[], int ph[]) {
    int N = signal_length;

    x[N - 1] = (b[N - 1] * scale) / diag0[N - 1];

    int term = (diag1[N - 2] * x[N - 1]) / scale;
    x[N - 2] = ((b[N - 2] - term) * scale) / diag0[N - 2];

    for (int i = N - 3; i >= 0; i--) {
        int sum = 0;

        sum += (diag1[i] * x[i + 1]) / scale;
        sum += (diag2[i] * x[i + 2]) / scale;

        x[i] = ((b[i] - sum) * scale) / diag0[i];
    }
}

void solve_v1(int diag0[], int diag1[], int diag2[], int diag_1[], int diag_2[], int b[], int x[], int ph[]) {
    int N = signal_length;
    int b_original[N];

    for (int i = 0; i < N; i++) {
        b_original[i] = b[i];
    }

    forward_elimination_v1(diag0, diag1, diag2, b, diag_1, diag_2);
    back_substitution_v1(diag0, diag1, diag2, b, x, ph);

    for (int i = 0; i < N; i++) {
        ph[i] = b_original[i] - x[i];
    }
}

