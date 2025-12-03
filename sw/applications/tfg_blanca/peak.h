#include "signal_EDA.h"
#include <math.h>
#include "mean.h"
#include "std_feat.h"


int peaks(int input[], int length, int fs, int th1, int w1, int th2, int amp, int peaks_index_max[], int peaks_values_max[], int peaks_index_min[], int peaks_values_min[], int *num_max_out, int *num_min_out);

