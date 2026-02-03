#include "std_feat.h"
#include <stdint.h>

int isqrt(int x) {
    if (x <= 0) return 0;

    int res = 0;
    int bit = 1 << 30;

    while (bit > x)
        bit >>= 2;

    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }

    return res;
}

void std(int input[], int length, int *output){
if (length <= 1) { *output = 0; return; }

    int64_t sum = 0;

    // Calculo de la media, mi señal entra en Q8
    for (int i = 0; i < length; i++) sum += input[i];

    // Al hacer la multiplicacion Q8*Q14--> Q22, hago desplazamiento de 8 para volver a Q14
    int mean_Q14 = (sum*INV_N) >> 8; 

    printf("Media (Q14): %d\r\n", mean_Q14);
    int suma_diff_Q7 = 0;
    // Suma de cuadrados
    for (int i = 0; i < length; i++) {
        int val_Q14 = input[i] << 6; // Q8 -> Q14
        int diff = val_Q14 - mean_Q14;           // Q14
        // bajo la precision en diff para evitar desbordamiento
        int diff_Q8 = diff >> 6; 
        int cuadrado_Q16 = diff_Q8 * diff_Q8;
        
        // vuelvo a bajar la precision a Q10 para evitar desbordamiento
        suma_diff_Q7 += (cuadrado_Q16 >> 9);
    }

    // Varianza
    // Q12*Q14 --> Q24
    int varianza_Q21 = (suma_diff_Q7 * INV_N_MINUS_1);

    // Desviación Típica
    // sqrt(Q26) --> Q13
    int std_Q12 = isqrt(varianza_Q21 << 3);

    //  Quiero la salida en Q8
    // De Q13 a Q8 bajamos 5 bits
    *output = std_Q12 >> 4;
}
