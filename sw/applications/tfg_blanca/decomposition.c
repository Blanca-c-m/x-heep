#include "decomposition.h"
#include <stdlib.h>

// declaro las variables como long long int para mejorar 

void buildD2(long long int D2[signal_length - 2][signal_length], int length){
    for (int i = 0; i < length - 2; i++){
        for (int j = 0; j< length; j++) D2[i][j] = 0;
        D2[i][i] = 1 * scale;
        D2[i][i+1] = -2 * scale;
        D2[i][i+2] = 1 * scale;
    }
}

//COMPRUEBO QUE LA MATRIZ SE HA CONSTRUIDO DE FORMA CORRECTA
void print_D2(long long int D2[signal_length - 2][signal_length], int length){
    printf("\nMatriz D2 (%d x %d):\n", length-2, length);
    for (int i = 0; i < length-2; i++) {
        for (int j = 0; j < length; j++) {
            printf("%12lld ", D2[i][j]);
        }
        printf("\n");
    }
}

void buildD2t(long long int D2[signal_length - 2][signal_length], long long int D2t[signal_length ][signal_length - 2], int length){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length - 2; j++){
            D2t[i][j] = D2[j][i];
        }
    }
}

//COMPRUEBO QUE LA MATRIZ SE HA CONSTRUIDO DE FORMA CORRECTA
void print_D2t(long long int D2t[signal_length][signal_length - 2], int length){
    printf("\nMatriz D2t (%d x %d):\n", length, length-2);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length - 2; j++) {
            printf("%12lld ", D2t[i][j]);
        }
        printf("\n");
    }
}

void multiplyD2tD2(long long int D2t[signal_length][signal_length - 2], long long int D2[signal_length - 2][signal_length], long long int mat[signal_length][signal_length], int length){
    long long int sum = 0;
    for (int i = 0; i < length; i++){ // recorro fila de D2t
        for(int j = 0; j < length; j++){
            sum = 0;// recorro columna de D2
            for(int k = 0; k < length - 2; k++){ // recorre la fila D2t y la columna D2
                sum += (D2t[i][k] * D2[k][j]) / scale;  // mantener punto fijo
            }
            mat[i][j] = sum;
        }
    }
}

void print_multiplcar(long long int mat[signal_length][signal_length], int length){
    printf("\nMatriz D2tD (%d x %d):\n", length, length);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            printf("%12lld ", mat[i][j]);
        }
        printf("\n");
    }
}

void buildA(long long int mat[signal_length][signal_length], int length, int lambda, long long int A[signal_length][signal_length]){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){
            if(i == j) A[i][j] = scale + ((long long int)lambda * lambda * mat[i][j]);
            else A[i][j] = ((long long int)lambda * lambda * mat[i][j]);
        }
    }
}

void print_A(long long int A[signal_length][signal_length], int length) {
    printf("\nMatriz A:\n");
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            printf("%12lld ", A[i][j]);
        }
        printf("\n");
    }
}

void print_system(long long int sist[signal_length][signal_length + 1], int length) {
    printf("\nSistema aumentado (A|b):\n");
    for (int i = 0; i < length; i++) {
        for (int j = 0; j <= length; j++) {
            printf("%12lld ", sist[i][j]);
        }
        printf("\n");
    }
}

// ELIMINACIÓN DE GAUSS POR PIVOTEO PARCIAL
int Gauss(long long int sist[signal_length][signal_length + 1], int length){
    const long long int tolerancia = 1;

    for (int Col = 0; Col < length; Col++){
        int maxfila = Col;
        long long int maxvalor = llabs(sist[Col][Col]); // mejor valor diagonal llabs es lo mismo que fabs pero para long long int

        for(int fila = Col + 1; fila < length; fila++){
            long long int valor = llabs(sist[fila][Col]);
            if(valor > maxvalor){
                maxvalor = valor;
                maxfila = fila;
            }
        }

        if(maxvalor < tolerancia){
            printf("ERROR: LA COLUMNA %d ES CASI NULA, MATRIZ SINGULAR", Col);
            return -1;
        }

        // INTERCAMBIO FILAS
        if (maxfila != Col){
            for (int j = 0; j <= length; j++){
                long long int temporal = sist[Col][j];
                sist[Col][j] = sist[maxfila][j];
                sist[maxfila][j] = temporal;
            }
        }

        // NORMALIZACIÓN DE LA FILA
        long long int pivote= sist[Col][Col];
        for (int j = 0; j <= length; j++){
            sist[Col][j] = (sist[Col][j] * scale) / pivote;  // mantener decimales
        }

        // ELIMINAR TRIANGULAR INFERIOR
        for(int fila = Col + 1; fila < length; fila ++){
            long long int factor = sist[fila][Col];
            for (int j = Col; j <= length; j++){
                //sist[fila][j] -= (factor * sist[Col][j]) / scale;
                long long int delta = (factor * sist[Col][j]) / scale;
                sist[fila][j] -= delta;
            }
        }
    }

    // RESOLVER DE ARRIBA A ABAJO
    for(int Col = length - 1; Col >= 0; Col--){
        for(int fila = 0; fila < Col; fila++){
            sist[fila][length] -= (sist[fila][Col] * sist[Col][length]) / scale;
            sist[fila][Col] = 0;
        }
    }

    return 0;
}

void decompose_eda(int input[], int length, int tonic[], int phasic[], int lambda){
    
    long long int D2[length -2][length];
    long long int D2t[length][length - 2];
    long long int mat_multiplicada[length][length];
    long long int mat_A[length][length];
    long long int sistema[length][length + 1];

    // //contruyo cada matriz e imprimo por pantalla para comprobar que se ha hecho bien
    buildD2(D2, length);
    //print_D2(D2, length);

    buildD2t(D2, D2t, length);
    //print_D2t(D2t, length);

    multiplyD2tD2(D2t, D2, mat_multiplicada, length);
    //print_multiplcar(mat_multiplicada, length);

    buildA(mat_multiplicada, length, lambda, mat_A);
    //print_A(mat_A, length);
    
    //Añado a la matriz A el vector b (mi input)
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            sistema[i][j] = mat_A[i][j];
        }
        sistema[i][length] = input[i]; 
    }

    //print_system(sistema, length);

    //Resuelvo el sistema por Gauss
    if (Gauss(sistema, length) == 0) {
        for (int i = 0; i < length; i++) {
            tonic[i] = sistema[i][length];        // Tónica (SCL)
            phasic[i] = input[i] - tonic[i];             // Fásica (SCR)
        }
    }

   // printf("\nSISTEMA RESUELTO\n"); 
   // print_system(sistema, length); 
   //printf("\nSoluciones (x):\n"); 
    // for (int i = 0; i < length; i++) { 
    //     printf("tonic[%d] = %d\r\n", i, tonic[i]); 
    // }
    for (int i = 0; i < length; i++) { 
        printf("phasic[%d] = %d\r\n", i, phasic[i]); 
    }
}
