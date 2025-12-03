#include "signal_EDA.h"
#include <math.h>
#include "first_derivate.h"
#include "mean.h"
#include "std_feat.h"


int peakmin(int input[], int length, int fs, int th1, int w1, int th2, int amp, int peaks_index[], int peak_values[]);