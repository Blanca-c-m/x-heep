#include "peak.h"
#define MAX_SIGNAL 1024 // MODIFICAR CUANDO SEPA

void first_derivate(int input[], int output[], int fs, int length){
    output[0] = 0;
    for (int i=1; i < length; i++){
        //output[i] = (input[i] - input[i-1])*fs;
        output[i] = (input[i + 1] - input[i])*fs;
    }


}
//FILTRO ESTADÍSTICO: supera a la "media" FILTRO BINARIO: tiene una altura suficiente
int peaks(int input[], int length, int fs, int th1, int w1, int th2, int amp, int peaks_index_max[], int peaks_values_max[], int peaks_index_min[], int peaks_values_min[], int *num_max_out, int *num_min_out) {
    // RESUMEN PARA RECORDAR th1: umbral estadístico, w1: tamaño de la ventana, th2: umbral binario, amp: distancia máxima para buscar picos

    int fd[length]; // valores primera derivada
    int fdsign[length]; // signo primera derivada
    int candidate_max[MAX_SIGNAL]; // posibles candidatos
    int candidate_min[MAX_SIGNAL]; // posibles candidatos
    int count_max = 0;
    int count_min = 0;


    first_derivate(input, fd, fs, length); // calculo la primera derivada de los valores de la señal y lo almaceno en fd

    // Quiero saber que signo tienen para estudiar un cambio
    for (int i = 0; i < length - 1; i++) {
        if (fd[i] > 0) fdsign[i] = 1;
        else if (fd[i] < 0) fdsign[i] = -1;
        else fdsign[i] = 0;
    }
    
    for (int i = 1; i < length; i++) {
    if (fdsign[i] == 0) {
        fdsign[i] = fdsign[i - 1];
    }
}
    //  ESTUDIO SI HA HABIDO CAMBIO DE SIGNO, si lo hay se considera un posible candidato
    printf("Detecto posibles candidatos por cambio de signo\n");

    for (int i = 1; i < length - 1; i++) {
        if (fdsign[i - 1] > 0 && fdsign[i] < 0) {
            candidate_max[count_max] = i; //guardo el indice
            count_max++;
        }
        if (fdsign[i - 1] < 0 && fdsign[i] > 0) {
            candidate_min[count_min] = i; //guardo el indice
            count_min++;
        }
    }

    printf("Candidatos a maximo por cambio de signo : %d\n", count_max);
    printf("Candidatos a minimo por cambio de signo : %d\n", count_min);

    int first_max[MAX_SIGNAL];
    int first_min[MAX_SIGNAL];

    int num_max = 0;
    int num_min = 0;
    int n = 0;


     // PRIMER FILTRO: ESTADÍSTICO
    for(int i = 0; i < count_max; i++){
        int index = candidate_max[i]; // almaceno el indice de los candidatos que tienen cambio de signo

        int m = 0, s = 0;
        int threshold = 0;

        // definir ventanas
        int ini = (index - w1 >= 0) ? index - w1 : 0; 
        int fin = (index + w1 < length) ? index + w1 : length - 1;
        int win_len = fin - ini + 1;

        // media y desviacion
        mean(&input[ini], win_len, &m);
        std(&input[ini], win_len, &s);

        threshold = m + (s*th1)/1000; // umbral creada a partir de media y desviacion

        printf("idx=%d, input=%.3d, mean=%.3d, std=%.3d, thr=%.3d\n", index, input[index], m, s, threshold);
        
        if (input[index] >= threshold) { // si se supera el umbral propuesto se considera un candidato
            first_max[num_max] = index;
            //first_max_values[num_max] = input[index];
            num_max++;
        }
    

    }
    //MISMO QUE ANTERIOR PERO AHORA PARA MÍNIMOS
    int temp_signal[MAX_SIGNAL]; for (int i = 0; i < length; i++) temp_signal[i] = -input[i]; 
    for(int i = 0; i < count_min; i++){
        int index = candidate_min[i]; // almaceno el indice de los candidatos que tienen cambio de signo

        int m = 0, s = 0;
        int threshold = 0;

        // definir ventanas
        int ini = (index - w1 >= 0) ? index - w1 : 0; 
        int fin = (index + w1 < length) ? index + w1 : length - 1;
        int win_len = fin - ini + 1;

        // media y desviacion
        mean(&temp_signal[ini], win_len, &m);
        std(&temp_signal[ini], win_len, &s);

        threshold = m + (s*th1)/1000; // umbral creada a partir de media y desviacion
    
       
        printf("idx=%d, input=%.3d, mean=%.3d, std=%.3d, thr=%.3d\n", index, input[index], m, s, threshold);
        
        if (temp_signal[index] >= threshold) { 
            first_min[num_min] = index;
            num_min++;
        }

    }
    printf("Candidatos para maximos por calculo estadistico : %d\n", num_max);
    printf("Candidatos para minimos por calculo estadistico : %d\n", num_min);

    fflush(stdout);

    int num_peaks_max = 0;
    int num_peaks_min = 0;


    for (int i = 0; i < num_max; i++) {
        int index = first_max[i]; // ahora como antes, me quedo solo con los seleccionados con el filtro estadistico
        
        // busco el minimo anterior y posterior
        int prev_min = input[index];
        int found_prev = 0;

        for (int m = index - 1; m >= 1 && (index - m) <= amp; m--) { // buscamos anteriores, por eso index - 1 
            if (fdsign[m - 1] < 0 && fdsign[m] > 0) { // ha habido cambio de signo en la derivada?
                prev_min = input[m];
                found_prev = 1;
                break; // cuando lo encuentre, sale del bucle. Así busco el más cercano
            }
        }
        int next_min = input[index];
        int found_next = 0;

        for (int m = index + 1; m < length && (m - index) <= amp; m++) {
            if (fdsign[m - 1] < 0 && fdsign[m] > 0) {
                next_min = input[m];
                found_next = 1;
                break;
            }
        }


        int prev_diff = (found_prev) ? input[index] - prev_min : 0;
        int next_diff = (found_next) ? input[index] - next_min : 0; 

        // Condición binaria
        if ((fabs(prev_diff) > th2 && found_prev) || (fabs(next_diff) > th2 && found_next)) { // si la distancia es mayor que el umbral y de ha obtenido previo
            peaks_index_max[num_peaks_max] = index;
            peaks_values_max[num_peaks_max] = input[index];
            num_peaks_max++;
        }

    }
    printf("Candidatos por calculo binaria: %d\n", num_peaks_max);
    fflush(stdout);

    for (int i = 0; i < num_min; i++) {
        int index = first_min[i]; // ahora como antes, me quedo solo con los seleccionados con el filtro estadistico
        
        // busco el minimo anterior y posterior
        int prev_max = input[index];
        int found_prev = 0;

        for (int m = index - 1; m >= 0 && (index - m) <= amp; m--) { // buscamos anteriores, por eso index - 1 
            if (fdsign[m - 1] > 0 && fdsign[m] < 0) { // ha habido cambio de signo en la derivada?
                prev_max = input[m];
                found_prev = 1;
                break;
            }
        }
        int next_max = input[index];
        int found_next = 0;

        for (int m = index + 1; m < length && (m - index) <= amp; m++) {
            if (fdsign[m - 1] > 0 && fdsign[m] < 0) {
                next_max = input[m];
                found_next = 1;
                break;
            }
        }


    int prev_diff = (found_prev) ? prev_max - input[index] : 0; 
    int next_diff = (found_next) ? next_max - input[index] : 0;

        // Condición binaria
        if ((fabs(prev_diff) > th2 && found_prev) || (fabs(next_diff) > th2 && found_next)) { // si la distancia es mayor que el umbral y de ha obtenido previo
            peaks_index_min[num_peaks_min] = index;
            peaks_values_min[num_peaks_min] = input[index];
            num_peaks_min++;
        }

    }
    printf("Candidatos por calculo binaria: %d\n", num_peaks_min);
    fflush(stdout);

    *num_max_out = num_peaks_max;
    *num_min_out = num_peaks_min;
    return num_peaks_max + num_peaks_min;
}
