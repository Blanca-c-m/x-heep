#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "descompinversa.h"

int cargar_matriz(const char *nombre_archivo, int matriz[][process_length], int n) {
    FILE *file = fopen(nombre_archivo, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de la matriz");
        return 0;
    }

    for (int i = 0; i < process_length; i++) { // Filas
        for (int j = 0; j < process_length; j++) { // Columnas
            // Ahora leemos directamente como entero (%d)
            if (fscanf(file, "%d", &matriz[i][j]) != 1) {
                fprintf(stderr, "Error leyendo el elemento [%d][%d] de la matriz en %s\n", i, j, nombre_archivo);
                fclose(file);
                return 0;
            }
        }
    }
    fclose(file);
    printf("Matriz %s cargada exitosamente. (Formato entero simple)\n", nombre_archivo);
    return 1;
}


void descompEDA(const int signal[signal_length], const int matriz[][process_length], int solucionTonica[process_length], int solucionFasica[process_length]) {
    int temp_sum;
    for (int i = 0; i < process_length; i++) { 
        temp_sum = 0; 
        for (int j = 0; j < process_length; j++) { 
            temp_sum += matriz[i][j] * signal[j]; 
        }
        solucionTonica[i] = temp_sum / scale;  //divido entre scale para que tenga escala correcta
    }
        for (int i = 0; i < process_length; i++) {
        solucionFasica[i] = signal[i] - solucionTonica[i];
    }
}
