#include "signal_EDA_shift.h" 
#define ARCHIVO_AINV "C:\\tfg\\error funciones\\matrizAinvertida.txt"
int cargar_matriz(const char *nombre_archivo, int matriz[][process_length], int n);
void descompEDA(const int signal[signal_length], const int matriz[][process_length], int solucionTonica[process_length], int solucionFasica[process_length]) ;
