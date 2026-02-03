#include "signal_EDA_shift.h"

void EDA_build_A_and_factorize();

void debug_ultimo_pivote();
void forward_elimination_v3(int b[]);
void back_substitution_v3(int b[], int x[]);
void solve_v3(int Xeda[], int Xtonica[], int Xfasica[]);
