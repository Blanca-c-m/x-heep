#include "signal_EDA_shift.h"

void EDA_build_A_and_factorize();
void forward_elimination_v5(int N, int b[], int l1[], int l2[]);
void back_substitution_v5(int N, int b[], int x[], int u0[], int u1[], int u2[]);
void EDA_AdjustMasterMatrix(int n_target, const int mU0[], const int mU1[], const int mU2[], const int mL1[], const int mL2[]);
void solve_v5(int N, int Xeda[], int Xtonica[], int Xfasica[], int u0[], int u1[], int u2[], int l1[], int l2[]);
