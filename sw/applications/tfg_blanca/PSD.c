#include "PSD.h"

void calcular_psd10(complex fft_in[], double psd_out[]) {
    int n_unilateral = 141; // 280/2 SE DEBE CAMBIAR DEPENDIENDO DE LAS MUESTRAS
    
    // 141 / 10 = 14 bins por banda (el último bin se ignora o se ajusta).
    int bins_per_band = 14;  // estoy agrupando la energia en bandas de frecuencia
    
    for (int banda = 0; banda < N_BANDS; banda++) {
        double suma_potencia = 0;
        
        for (int k = 0; k < bins_per_band; k++) {
            int idx = (banda * bins_per_band) + k; // ejemplo: 1*14 + 1= 15 ok, si banda 5 y k 3 , 5*14 + 3 = 73 ok,
            
            // Verificamos no salirnos del espectro unilateral (0 a 140)
            if (idx < n_unilateral) {
                // Cálculo de la magnitud al cuadrado: Re^2 + Im^2
                double re = fft_in[idx].re;
                double im = fft_in[idx].im;
                double mag_sq = (re * re) + (im * im);
                
                // Los bins que no son DC (0) ni Nyquist (140) deben multiplicarse por 2, el resto si por simetria
                if (idx > 0 && idx < 140) {
                    mag_sq *= 2.0;
                }
                
                suma_potencia += mag_sq;
            }
        }
        
        // Guardamos el acumulado de energía en la banda correspondiente
        psd_out[banda] = suma_potencia;
    }
}
