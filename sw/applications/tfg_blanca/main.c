#include <stdio.h>
#include "signal_EDA_shift.h"

#include "first_derivate.h"
#include "mean.h"
#include "max.h"
#include "min.h"
#include "std_feat.h"
#include "peak_max.h"
#include "peak_min.h"
#include "peak.h"
#include "second_derivate.h"
#include "desprueba.h"
#include "desprueba256.h"
//#include "descompv3.h"
#include "descompv5.h"
#include "descompinversa.h"
#include "fft.h"
#include "PSD.h"

#define OPCION_MEAN 1
#define OPCION_MAX 2
#define OPCION_MIN 3
#define OPCION_1DERIVADA 4
#define OPCION_2DERIVADA 5
#define OPCION_STD 6
#define OPCION_EDA1 7
#define OPCION_EDA2 8
#define OPCION_EDA3 9
#define OPCION_EDA4 10
#define OPCION_EDA5 11
#define OPCION_PSD 12
#define OPCION_PEAK 13
#define OPCION_PEAKMIN 14
#define OPCION_PEAKMAX 15

int lambda = 1;
int diag0[signal_length], diag1[signal_length], diag_1[signal_length], diag2[signal_length], diag_2[signal_length], tonica[signal_length], fasica[signal_length];
static int A_inv[process_length][process_length];
complex resultado_fft[signal_length];
double bandas_potencia[10];



extern int EDA_U_diag0[];
extern int EDA_U_diag1[];
extern int EDA_U_diag2[];
extern int EDA_L_mult_1[];
extern int EDA_L_mult_2[];

extern int tmp_U0[];
extern int tmp_U1[];
extern int tmp_U2[];
extern int tmp_L1[];
extern int tmp_L2[];

void mostrar_menu() {
    printf("  %d Ejecutar media\n", OPCION_MEAN);
    printf("  %d Ejecutar max\n", OPCION_MAX);
    printf("  %d Ejecutar min\n", OPCION_MIN);
    printf("  %d Ejecutar primera derivada\n", OPCION_1DERIVADA);
    printf("  %d Ejecutar segunda derivada\n", OPCION_2DERIVADA);
    printf("  %d Ejecutar desviacion tipica\n", OPCION_STD);
    printf("  %d Ejecutar primera optimizacion de EDA (solo diagonales con division)\n", OPCION_EDA1);
    printf("  %d Ejecutar segunda optimizacion de EDA (diagonales + division por desplazamiento)\n", OPCION_EDA2);
    printf("  %d Ejecutar tercera optimizacion de EDA (diagonales previamente almacenadas y solo se modifica señal)\n", OPCION_EDA3);
    printf("  %d Ejecutar cuarta optimizacion de EDA (se pasa la matriz invertida A directamente)\n", OPCION_EDA4);
    printf("  %d Ejecutar quinta optimizacion de EDA (a partir de la matriz A de 200 conseguir otras)\n", OPCION_EDA5);
    printf("  %d Ejecutar PSD\n", OPCION_PSD);
    printf("  %d Ejecutar peak (maximos y minimos)\n", OPCION_PEAK);
    printf("  %d Ejecutar peak (minimos)\n", OPCION_PEAKMIN);
     printf("  %d Ejecutar peak (maximos)\n", OPCION_PEAKMAX);

    printf("  0. Salir\n");

}
//complex resultado_fft[N_TOTAL];
void system_initialization(){
        EDA_build_A_and_factorize();
        printf("Inicializacion completa\n");
    }


int main(void){

    scale_signal(); 
    printf("Senal escalada correctamente a Q8.\n");
    system_initialization();
    // EN PRINCIPIO ESTA FUNCION SOLO SE EJECUTARIA UNA VEZ ALMACENANDO LOS VALORES DE LAS DIAGONALES PARA LA LONGITUD PREDEFINIDA
   

    int opcion = -1;
    while (opcion != 0) {    
        mostrar_menu();    
        if (scanf("%d", &opcion) != 1) {
            printf("ERROR: Entrada invalida. Ingrese un numero.\n");
            // Limpiar buffer para evitar bucles infinitos
            while (getchar() != '\n'); 
            continue;
        }
    

        switch (opcion) {
            case OPCION_MEAN:
                int media = 0;
                mean(signal, signal_length, &media);
                printf("La media de enteros es: %d\r\n", media);
                break;

            case OPCION_MAX:
                int maximo = 0;
                int indice_max = 0;
                max(signal, signal_length, &maximo, &indice_max);
                printf("El maximo valor es: %d y su indice es: %d\r\n", maximo, indice_max);
                break;

            case OPCION_MIN: 
                int minimo = 0;
                int indice_min = 0;
                min(signal, signal_length, &minimo, &indice_min);
                printf("El minimo valor es: %d y su indice es: %d\r\n", minimo, indice_min);
                break;
            
            case OPCION_1DERIVADA:
                {
                    int first_deriv[signal_length];
                    first_derivate(signal, first_deriv, frecuency, signal_length);

                    FILE *fp = fopen("C:\\tfg\\error funciones\\primera_derivadaC.txt","w");
                    for(int i=0; i < signal_length; i++){
                        fprintf(fp, "%d\n", first_deriv[i]);
                    }
                    fclose(fp);
                }
                break;

            case OPCION_2DERIVADA:
                int seg_deriv[signal_length];
                printf("La segunda derivada de la señal es: \n");
                second_derivate(signal, signal_length, frecuency, seg_deriv);
                for (int i = 0; i < signal_length; i++){
                    printf("%d\n", seg_deriv[i]);
                }
                FILE *fp1 = fopen("C:\\tfg\\error funciones\\segunda_derivadaC.txt","w");
                for(int i=0; i < signal_length; i++){
                    fprintf(fp1, "%d\n", seg_deriv[i]);
                }
                fclose(fp1);
                break;
            
            case OPCION_STD:
                int desv = 0;
                std(signal, signal_length, &desv);
                printf("La desviacion tipica es: %d\r\n", desv);
                break;
            case OPCION_EDA1:
                //Inicializar a cero
                for(int i=0;i<signal_length;i++) { diag0[i]=0; diag_1[i]=0; diag_2[i]=0; diag_1[i] = 0; diag_2[i] = 0; }
                for(int i=0;i<signal_length-1;i++) diag1[i]=0;
                for(int i=0;i<signal_length-2;i++) diag2[i]=0;
                //Llamada a las funciones
                build_diagonales_v1(diag0, diag1, diag_1, diag2, diag_2, lambda);
                solve_v1(diag0, diag1, diag2, diag_1, diag_2, signal, tonica, fasica);
                fflush(stdout);

                for (int i = 0; i < signal_length; i++) {
                    printf("%d\n", tonica[i]);
                }
                //Guardo valores tonica
                FILE *fp2 = fopen("C:\\tfg\\error funciones\\tonicaC.txt","w");
                for(int i = 0; i < signal_length; i++){
                fprintf(fp2, "%d\n", tonica[i]);
                }
                //Guardo valores fasica
                FILE *fp3 = fopen("C:\\tfg\\error funciones\\fasicaC.txt","w");
                for(int i = 0; i < signal_length; i++){
                fprintf(fp3, "%d\n", fasica[i]);
                }
                
                fflush(stdout);
                fclose(fp2);
                fclose(fp3);
                break;
            case OPCION_EDA2:
                //Inicializar a cero
                for(int i=0;i<signal_length;i++) { diag0[i]=0; diag_1[i]=0; diag_2[i]=0; diag_1[i] = 0; diag_2[i] = 0; }
                for(int i=0;i<signal_length-1;i++) diag1[i]=0;
                for(int i=0;i<signal_length-2;i++) diag2[i]=0;
                //Llamada a las funciones
                build_diagonales_v2(diag0, diag1, diag_1, diag2, diag_2, lambda);
                solve_v2(diag0, diag1, diag2, diag_1, diag_2, signal, tonica, fasica);
                fflush(stdout);

                for (int i = 0; i < signal_length; i++) {
                    printf("%d\n", tonica[i]);
                }
                //Guardo valores tonica
                FILE *fp4 = fopen("C:\\tfg\\error funciones\\tonicaC2.txt","w");
                for(int i = 0; i < signal_length; i++){
                fprintf(fp4, "%d\n", tonica[i]);
                }
                fclose(fp4);
                //Guardo valores fasica
                FILE *fp5 = fopen("C:\\tfg\\error funciones\\fasicaC2.txt","w");
                for(int i = 0; i < signal_length; i++){
                fprintf(fp5, "%d\n", fasica[i]);
                }
                fclose(fp5);
                fflush(stdout);
                break;
            
            // case OPCION_EDA3: 
            //     //primero hago solve de 200 muestras               
            //     solve_v3(signal, tonica, fasica);

            //     for (int i = 0; i < signal_length; i++) {
            //         printf("%d\n", tonica[i]);
            //     }
            //     FILE *fp6 = fopen("C:\\tfg\\error funciones\\tonicaC3.txt","w");
            //     for(int i=0; i < signal_length; i++){
            //         fprintf(fp6, "%d\n", tonica[i]);
            //     }
            //     fclose(fp6);
            //     FILE *fp7 = fopen("C:\\tfg\\error funciones\\fasicaC3.txt","w");
            //     for(int i=0; i < signal_length; i++){
            //         fprintf(fp7, "%d\n", fasica[i]);
            //     }
            //     fclose(fp7);
            //     break;
            case OPCION_EDA4:{
                if (cargar_matriz(ARCHIVO_AINV, A_inv, process_length)) {
                    descompEDA(signal, A_inv, tonica, fasica); 
                    for (int i = 0; i < process_length; i++) {
                        printf("%d\n", tonica[i]);
                    } 
                }
                FILE *fp8 = fopen("C:\\tfg\\error funciones\\tonicaC4.txt", "w");
                    if (fp8 != NULL) {
                        for(int i = 0; i < process_length; i++) {
                            int valor_actual = tonica[i]; // Intentamos leer el valor
                            fprintf(fp8, "%d\n", valor_actual);
                        }
                        fclose(fp8);
                    }

                FILE *fp9 = fopen("C:\\tfg\\error funciones\\fasicaC4.txt","w");
                for(int i=0; i < process_length; i++){
                    fprintf(fp9, "%d\n", fasica[i]);
                }      
                fclose(fp9); } break;
                        
            case OPCION_EDA5: 
                int N_master = 200; 
                int N_recorte = 100;
                //primero hago solve de 200 muestras               
                solve_v5(N_master, signal, tonica, fasica, EDA_U_diag0, EDA_U_diag1, EDA_U_diag2, EDA_L_mult_1, EDA_L_mult_2);
                EDA_AdjustMasterMatrix(N_recorte, EDA_U_diag0, EDA_U_diag1, EDA_U_diag2, EDA_L_mult_1, EDA_L_mult_2);
                solve_v5(N_recorte, signal, tonica, fasica, tmp_U0, tmp_U1, tmp_U2, tmp_L1, tmp_L2);
                // for (int i = 0; i < N_recorte; i++) {
                //     printf("T100_adj[%d]: %d\n", i, tonica[i]);
                // }
                for (int i = 0; i < N_recorte; i++) {
                    printf("%d\n", tonica[i]);
                }
                FILE *fp10 = fopen("C:\\tfg\\error funciones\\tonicaC5.txt","w");
                for(int i=0; i < N_recorte; i++){
                    fprintf(fp10, "%d\n", tonica[i]);
                }
                fclose(fp10);
                FILE *fp11 = fopen("C:\\tfg\\error funciones\\fasicaC5.txt","w");
                for(int i=0; i < N_recorte; i++){
                    fprintf(fp11, "%d\n", fasica[i]);
                }
                fclose(fp11);
                break;
            case OPCION_PSD:
                FFT_CooleyTukey(signal, resultado_fft);
                for (int i = 0; i < 15; i++) {
                    printf("Bin %03d | %15.2f | %15.2f\n", i, resultado_fft[i].re, resultado_fft[i].im);
                }
                calcular_psd10(resultado_fft, bandas_potencia);
                for (int i = 0; i < 10; i++) {
                    // Usamos %f porque los valores con escala 256 serán muy grandes
                    printf("Banda %d: Potencia = %f\n", i, bandas_potencia[i]);
                }
                FILE *fp12 = fopen("C:\\tfg\\error funciones\\psdC.txt","w");
                for(int i=0; i < 10; i++){
                    fprintf(fp12, "%f\n", bandas_potencia[i]);
                }
                fclose(fp12);
            break;
            case OPCION_PEAK:
                {
                    int th1 = 0.1*scale;
                    int th2 = 0.01*scale;
                    int w1 = 2;
                    int amp = 4;

                    int indice_max[signal_length];
                    int valor_max[signal_length];
                    int indice_min[signal_length];
                    int valor_min[signal_length];

                    int num_min = 0;
                    int num_max = 0;

                    peaks(signal, signal_length, frecuency, th1, w1, th2, amp, indice_max, valor_max, indice_min, valor_min, &num_max, &num_min);

                    for(int i = 0; i < num_max; i++){
                        printf("%d\n", valor_max[i]);
                    }

                    for(int i = 0; i < num_min; i++){
                        printf("%d\n", valor_min[i]);
                    }

                    FILE *fp = fopen("C:\\tfg\\error funciones\\picosC.txt", "w");
                    if (fp != NULL) {
                        for(int i = 0; i < num_max; i++){
                            fprintf(fp, "%d\n", valor_max[i]);
                        }
                        for(int i = 0; i < num_min; i++){
                            fprintf(fp, "%d\n", valor_min[i]);
                            printf("el valor min %d", valor_min[i]);
                        }
                        fclose(fp);
                    }

                    FILE *fp1 = fopen("C:\\tfg\\error funciones\\indicepicosC.txt", "w");
                    if (fp1 != NULL) {
                        for(int i = 0; i < num_max; i++){
                            fprintf(fp1, "%d\n", indice_max[i] + 1);
                        }
                        for(int i = 0; i < num_min; i++){
                            fprintf(fp1, "%d\n", indice_min[i] + 1);
                            
                        }
                        fclose(fp1);
                    }
            }
                break;
            case OPCION_PEAKMIN: 
            {
                    int th1 =0.1*scale;
                    int th2 = 0.01*scale;
                    int w1 = 2;
                    int amp = 4;

                    int indice_min[signal_length];
                    int valor_min[signal_length];

                    int num_minimos = peakmin(signal, signal_length, frecuency, th1, w1, th2, amp, indice_min,  valor_min);

                    FILE *fp = fopen("C:\\tfg\\error funciones\\picosC.txt", "w");
                    if (fp != NULL) {
                        for(int i = 0; i < num_minimos; i++){
                            fprintf(fp, "%d\n", valor_min[i]);
                            printf("el valor min %d", valor_min[i]);
                        }
                        fclose(fp);
                    }

                    FILE *fp1 = fopen("C:\\tfg\\error funciones\\indicepicosC.txt", "w");
                    if (fp1 != NULL) {
                        for(int i = 0; i < num_minimos; i++){
                            fprintf(fp1, "%d\n", indice_min[i] + 1);
                            
                        }
                        fclose(fp1);
                    }
                
            } 
            break;
            case OPCION_PEAKMAX: 
            {
                    int th1 =0.1*scale;
                    int th2 = 0.01*scale;
                    int w1 = 2;
                    int amp = 4;

                    int indice_max[signal_length];
                    int valor_max[signal_length];

                    int num_max = peakmax(signal, signal_length, frecuency, th1, w1, th2, amp, indice_max,  valor_max);

                    FILE *fp = fopen("C:\\tfg\\error funciones\\picosC.txt", "w");
                    if (fp != NULL) {
                        for(int i = 0; i < num_max; i++){
                            fprintf(fp, "%d\n", valor_max[i]);
                            printf("el valor min %d", valor_max[i]);
                        }
                        fclose(fp);
                    }

                    FILE *fp1 = fopen("C:\\tfg\\error funciones\\indicepicosC.txt", "w");
                    if (fp1 != NULL) {
                        for(int i = 0; i < num_max; i++){
                            fprintf(fp1, "%d\n", indice_max[i] + 1);
                            
                        }
                        fclose(fp1);
                    }
                
            } 
        }
        
     
    }   

    return 0;
}
