#ifndef SIGNAL_H
#define SIGNAL_H
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#define signal_length 280
#define process_length 280
#define frecuency 4
#define scale 256
#define Q_SCALE 14 // utilizo este porque su multiplicacion sigue cabiendo en un int32
#define ONE_Q (1 << Q_SCALE)
#define NMAX 200
// CASO N = 280
#define INV_N 58
#define INV_N_MINUS_1 59    // (1 << 14) / 279
// CASO N = 200
/*
#define N 200
#define INV_N 82
#define INV_N_MINUS_1 82
*/

// CASO N = 100
/*#
define N 100
#define INV_N 164
#define INV_N_MINUS_1 165
*/
extern double signal_decimal[signal_length];
extern int signal[signal_length];
void scale_signal();
#endif
