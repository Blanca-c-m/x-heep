#include "peak.h"
#define MAX_SIGNAL 1024 // MODIFICAR CUANDO SEPA

int peakmax(int input[], int length, int fs, int th1, int w1, int th2, int amp, int peaks_index[], int peaks_values[]) {
    // RESUMEN PARA RECORDAR th1: umbral estadístico, w1: tamaño de la ventana, th2: umbral binario, amp: distancia máxima para buscar picos

    int fd[MAX_SIGNAL]; // valores primera derivada
    int fdsign[MAX_SIGNAL]; // signo primera derivada
    int candidate[MAX_SIGNAL]; // posibles candidatos
    candidate[0] = 0;
    int count_candidate = 0;


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

    //  ESTUDIO SI HA HABIDO CAMBIO DE SIGNO, si lo hay se considera un posible candidato
    // AQUÍ SOLO ESTOY ESTUDIANDO MÁXIMOS, TENDRÉ QUE IMPLEMENTAR MÍNIMOS
    printf("Detecto posibles candidatos por cambio de signo\n");

    for (int i = 1; i < length - 1; i++) {
        if (fdsign[i - 1] > 0 && fdsign[i] < 0) {
            candidate[count_candidate] = i; //guardo el indice
            count_candidate++;
        }
    }

    printf("Candidatos por cambio de signo: %d\n", count_candidate);

    int first_candidate[MAX_SIGNAL];
    int num_candidates = 0;

    // PRIMER FILTRO: ESTADÍSTICO
    for(int i = 0; i < count_candidate; i++){
        int index = candidate[i]; // almaceno el indice de los candidatos que tienen cambio de signo

        int m = 0, s = 0;
        int threshold = 0;

        // definir ventanas
        int ini = (index - w1 >= 0) ? index - w1 : 0; 
        int fin = (index + w1 < length) ? index + w1 : length - 1;
        int win_len = fin - ini + 1;

        // media y desviacion
        mean(&input[ini], win_len, &m);
        std(&input[ini], win_len, &s);

        threshold = m + (s* th1)/1000; // umbral creada a partir de media y desviacion

        printf("idx=%d, input=%.3d, mean=%.3d, std=%.3d, thr=%.3d\n", index, input[index], m, s, threshold);
        
        if (input[index] >= threshold) { // si se supera el umbral propuesto se considera un candidato
            first_candidate[num_candidates] = index;
            num_candidates++;
        }
    

    }
    printf("Candidatos por calculo estadistico: %d\n", num_candidates);
    fflush(stdout);

    int num_peaks = 0;

    for (int i = 0; i < num_candidates; i++) {
        int index = first_candidate[i]; // ahora como antes, me quedo solo con los seleccionados con el filtro estadistico
        
        // busco el minimo anterior y posterior
        int prev_min = input[index];
        int found_prev = 0;

        for (int m = index - 1; m >= 0 && (index - m) <= amp; m--) { // buscamos anteriores, por eso index - 1 
            if (m > 0 && fdsign[m - 1] < 0 && fdsign[m] > 0) { // ha habido cambio de signo en la derivada?
                prev_min = input[m];
                found_prev = 1;
                break;
            }
        }
        int next_min = input[index];
        int found_next = 0;
        for (int m = index + 1; m < length && (m - index) <= amp; m++) {
            if (m > 0 && fdsign[m - 1] < 0 && fdsign[m] > 0) {
                next_min = input[m];
                found_next = 1;
                break;
            }
        }


        int prev_diff = (found_prev) ? input[index] - prev_min : 0; 
        int next_diff = (found_next) ? input[index] - next_min : 0; 

        // Condición binaria
        if ((fabs(prev_diff) > th2 && found_prev) || (fabs(next_diff) > th2 && found_next)) { // si la distancia es mayor que el umbral y de ha obtenido previo
            peaks_index[num_peaks] = index;
            peaks_values[num_peaks] = input[index];
            num_peaks++;
        }

    }
    printf("Candidatos por calculo binario: %d\n", num_peaks);
    fflush(stdout);
    return num_peaks;
}
